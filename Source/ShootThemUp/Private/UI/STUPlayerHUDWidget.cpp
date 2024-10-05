
#include "UI/STUPlayerHUDWidget.h"

#include "STUHeathComponent.h"
#include "STUWeaponComponent.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    const auto Player = GetOwningPlayerPawn();
    if(!Player) return 0.f;
    const auto Component = Player->GetComponentByClass(USTUHeathComponent::StaticClass());
    const auto HealthComponent = Cast<USTUHeathComponent>(Component);
    if(!HealthComponent) return 0.f;
    return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& UIData) const
{
    const auto Player = GetOwningPlayerPawn();
    if(!Player) return false;
    const auto Component = Player->GetComponentByClass(USTUWeaponComponent::StaticClass());
    const auto WeaponComponent = Cast<USTUWeaponComponent>(Component);
    if(!WeaponComponent) return false;
    return WeaponComponent->GetWeaponUIData(UIData);
}