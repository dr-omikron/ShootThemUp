
#include "STUWeaponComponent.h"
#include "STUBaseWeapon.h"
#include "STUEquipFinishedNotify.h"
#include "STUReloadFinishedAnimNotify.h"
#include "Animations/AnimUtils.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(WeaponComponentLog, All, All);

constexpr static int32 WeaponNumbers = 2;

USTUWeaponComponent::USTUWeaponComponent():
    WeaponEquipSocketName("WeaponSocket"),
    WeaponArmorySocketName("ArmorySocket"),
    EquipAnimMontage(nullptr),
    CurrentWeapon(nullptr),
    CurrentReloadAnimMontage(nullptr),
    CurrentWeaponIndex(0),
    bEquipAnimInProgress(false),
    bReloadAnimInProgress(false)
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();
    checkf(WeaponDates.Num() == WeaponNumbers, TEXT("Character can hold only %i weapon items!"), WeaponNumbers);
    InitAnimations();
    SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon = nullptr;
    for (const auto Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();
    Super::EndPlay(EndPlayReason);
}

void USTUWeaponComponent::SpawnWeapons()
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if(!GetWorld() || !Character) return;
    for (auto OneWeaponData : WeaponDates)
    {
        const auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass);
        if(!Weapon) continue;
        Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnEmptyClip);
        Weapon->SetOwner(Character);
        Weapons.Add(Weapon);
        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
    }
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
    if(!Weapon || !SceneComponent) return;
    const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void USTUWeaponComponent::NextWeapon()
{
    if(!CanEquip()) return;
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EquipWeapon(const int32 WeaponIndex)
{
    if(WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
    {
        UE_LOG(WeaponComponentLog, Warning, TEXT("Invalid Weapon Index!"))
        return;
    }
    const ACharacter* Character = Cast<ACharacter>(GetOwner());
    if(!Character) return;
    if(CurrentWeapon)
    {
        CurrentWeapon->StopFireWeapon();
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }
    CurrentWeapon = Weapons[WeaponIndex];
    const auto CurrentWeaponData = WeaponDates.FindByPredicate([&](const FWeaponData& Data) {
        return Data.WeaponClass == CurrentWeapon->GetClass();
    });
    CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
    bEquipAnimInProgress = true;
    PlayAnimMontage(EquipAnimMontage);
}

void USTUWeaponComponent::StartFire()
{
    if(!CanFire()) return;
    CurrentWeapon->StartFireWeapon();
}

void USTUWeaponComponent::StopFire()
{
    if(!CurrentWeapon) return;
    CurrentWeapon->StopFireWeapon();
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* AnimMontage) const
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if(!Character) return;
    Character->PlayAnimMontage(AnimMontage);
}

void USTUWeaponComponent::InitAnimations()
{
    if(const auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedNotify>(EquipAnimMontage))
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
    }
    else
    {
        UE_LOG(WeaponComponentLog, Error, TEXT("Equip Anim Notify is Forgotten to Set!"));
        checkNoEntry();
    }
    for (auto OneWeaponData : WeaponDates)
    {
        const auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<USTUReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
        if(!ReloadFinishedNotify)
        {
            UE_LOG(WeaponComponentLog, Error, TEXT("Reload Anim Notify is Forgotten to Set!"));
            checkNoEntry();
        }
        ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
    }
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
    const ACharacter* Character = Cast<ACharacter>(GetOwner());
    if(!Character || Character->GetMesh() != MeshComponent) return;
    bEquipAnimInProgress = false;
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
    const ACharacter* Character = Cast<ACharacter>(GetOwner());
    if(!Character || Character->GetMesh() != MeshComponent) return;
    bReloadAnimInProgress = false;
}

bool USTUWeaponComponent::CanFire() const
{
    return CurrentWeapon && !bEquipAnimInProgress && !bReloadAnimInProgress;
}

bool USTUWeaponComponent::CanEquip() const
{
    return !bEquipAnimInProgress && !bReloadAnimInProgress;
}

bool USTUWeaponComponent::CanReload() const
{
    return CurrentWeapon
    && !bEquipAnimInProgress
    && !bReloadAnimInProgress
    && CurrentWeapon->CanReload();
}

void USTUWeaponComponent::ReloadWeapon()
{
    ChangeClip();
}

void USTUWeaponComponent::OnEmptyClip()
{
    ChangeClip();
}

void USTUWeaponComponent::ChangeClip()
{
    if(!CanReload()) return;
    CurrentWeapon->StopFireWeapon();
    CurrentWeapon->ChangeClip();
    bReloadAnimInProgress = true;
    PlayAnimMontage(CurrentReloadAnimMontage);
}


bool USTUWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    if(!CurrentWeapon) return false;
    UIData = CurrentWeapon->GetUIData();
    return true;
}

bool USTUWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    if(!CurrentWeapon) return false;
    AmmoData = CurrentWeapon->GetAmmoData();
    return true;
}
