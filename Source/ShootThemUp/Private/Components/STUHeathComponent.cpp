
#include "STUHeathComponent.h"

#include "STUGameModeBase.h"
#include "GameFramework/Pawn.h"

USTUHeathComponent::USTUHeathComponent():
    MaxHeath(100.f), AutoHeal(true),
    HealUpdateTime(1.f),
    HealDelay(3.f),
    HealModifier(5.f),
    Health(0.f)
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUHeathComponent::BeginPlay()
{
    Super::BeginPlay();
    check(MaxHeath > 0);
    SetHealth(MaxHeath);
    if(AActor* ComponentOwner = GetOwner())
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHeathComponent::OnTakeAnyDamage);
    }
}

void USTUHeathComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
    AActor* DamageCauser)
{
    if(Damage <= 0.f || IsDead() || !GetWorld()) return;
    SetHealth(Health - Damage);
    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    if(IsDead())
    {
        Killed(InstigatedBy);
        OnDeath.Broadcast();
    }
    else if(AutoHeal)
    {
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHeathComponent::HealUpdate, HealUpdateTime, true, HealDelay);
    }
    PlayCameraShake();

    //Apply Damage Type Example:
    /*UE_LOG(LogHealthComponent, Display, TEXT("Damage: %f"), Damage);

    if(DamageType)
    {
        if(DamageType->IsA<USTUFireDamageType>())
        {
            UE_LOG(LogHealthComponent, Display, TEXT("Fire Damage!"));
        }
        else if(DamageType->IsA<USTUIceDamageType>())
        {
            UE_LOG(LogHealthComponent, Display, TEXT("Ice Damage!"));
        }
    }*/
}

void USTUHeathComponent::SetHealth(const float NewHealth)
{
    const float NextHealth = FMath::Clamp(NewHealth, 0.f, MaxHeath);
    const float HealthDelta = NextHealth - Health;
    Health = NextHealth;
    OnHealthChange.Broadcast(Health, HealthDelta);
}

void USTUHeathComponent::HealUpdate()
{
    SetHealth(Health + HealModifier);
    if(IsHealthFull() && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}

bool USTUHeathComponent::TryToAddHealth(const float HealthAmount)
{
    if(IsDead() || IsHealthFull()) return false;
    SetHealth(Health + HealthAmount);
    return true;
}

bool USTUHeathComponent::IsHealthFull() const
{
    return FMath::IsNearlyEqual(Health, MaxHeath);
}


void USTUHeathComponent::PlayCameraShake() const
{
    if(IsDead()) return;
    const APawn* PlayerPawn = Cast<APawn>(GetOwner());
    if(!PlayerPawn) return;
    const auto Controller = PlayerPawn->GetController<APlayerController>();
    if(!Controller || !Controller->PlayerCameraManager) return;
    Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

void USTUHeathComponent::Killed(const AController* KillerController) const
{
    if(!GetWorld()) return;
    const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
    if(!GameMode) return;
    const auto Player = Cast<APawn>(GetOwner());
    const auto VictimController = Player ? Player->Controller : nullptr;
    GameMode->Killed(KillerController, VictimController);
}
