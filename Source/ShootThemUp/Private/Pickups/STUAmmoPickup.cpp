
#include "Pickups/STUAmmoPickup.h"
#include "STUHeathComponent.h"
#include "STUUtils.h"
#include "STUWeaponComponent.h"

bool ASTUAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
    Super::GivePickupTo(PlayerPawn);
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHeathComponent>(PlayerPawn);
    if(!HealthComponent || HealthComponent->IsDead()) return false;
    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(PlayerPawn);
    if(!WeaponComponent) return false;
    return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);
}