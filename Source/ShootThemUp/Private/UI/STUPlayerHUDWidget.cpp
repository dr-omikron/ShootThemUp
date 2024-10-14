
#include "UI/STUPlayerHUDWidget.h"
#include "STUHeathComponent.h"
#include "STUUtils.h"
#include "STUWeaponComponent.h"

bool USTUPlayerHUDWidget::Initialize()
{
    if(const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHeathComponent>(GetOwningPlayerPawn()))
    {
        HealthComponent->OnHealthChange.AddUObject(this, &USTUPlayerHUDWidget::OnHealthChange);
    }
    return Super::Initialize();
}

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    if(const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHeathComponent>(GetOwningPlayerPawn()))
    {
        return HealthComponent->GetHealthPercent();
    }
    return 0.f;
}

bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    if(const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn()))
    {
        return WeaponComponent->GetCurrentWeaponUIData(UIData);
    }
    return false;
}

bool USTUPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    if(const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn()))
    {
        return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
    }
    return false;
}

bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
    if(const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHeathComponent>(GetOwningPlayerPawn()))
    {
        return HealthComponent && !HealthComponent->IsDead();
    }
    return false;
}

bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}

void USTUPlayerHUDWidget::OnHealthChange(float Health, float HealthDelta)
{
    if(HealthDelta < 0.f)
    {
        OnTakeDamage();
    }
}
