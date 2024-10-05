
#include "STUHeathComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

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
        OnDeath.Broadcast();
    }
    else if(AutoHeal)
    {
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHeathComponent::HealUpdate, HealUpdateTime, true, HealDelay);
    }

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
    Health = FMath::Clamp(NewHealth, 0.f, MaxHeath);
    OnHealthChange.Broadcast(Health);
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
