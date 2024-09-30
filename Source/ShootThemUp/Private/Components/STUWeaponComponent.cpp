
#include "STUWeaponComponent.h"
#include "STUBaseWeapon.h"
#include "GameFramework/Character.h"

USTUWeaponComponent::USTUWeaponComponent():
    WeaponAttachPointName("WeaponSocket"),
    CurrentWeapon(nullptr)
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();
    SpawnWeapon();
}


void USTUWeaponComponent::SpawnWeapon()
{
    if(!GetWorld()) return;
    const ACharacter* Character = Cast<ACharacter>(GetOwner());
    if(!Character) return;
    CurrentWeapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
    if(!CurrentWeapon) return;
    const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);
}

void USTUWeaponComponent::Fire()
{
    if(!CurrentWeapon) return;
    CurrentWeapon->FireWeapon();
}
