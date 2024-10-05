
#include "UI/STUPlayerHUDWidget.h"
#include "STUHeathComponent.h"
#include "STUUtils.h"
#include "STUWeaponComponent.h"

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
