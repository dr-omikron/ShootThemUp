


#include "Pickups/STUHealthPickup.h"

#include "STUHeathComponent.h"
#include "STUUtils.h"

bool ASTUHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
    Super::GivePickupTo(PlayerPawn);
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHeathComponent>(PlayerPawn);
    if(!HealthComponent) return false;
    return HealthComponent->TryToAddHealth(HealthAmount);
}