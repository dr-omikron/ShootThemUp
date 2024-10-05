
#include "UI/STUPlayerHUDWidget.h"

#include "STUHeathComponent.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    const auto Player = GetOwningPlayerPawn();
    if(!Player) return 0.f;
    const auto Component = Player->GetComponentByClass(USTUHeathComponent::StaticClass());
    const auto HealthComponent = Cast<USTUHeathComponent>(Component);
    if(!HealthComponent) return 0.f;
    return HealthComponent->GetHealthPercent();
}