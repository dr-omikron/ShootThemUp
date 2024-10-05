#pragma once

#include "CoreMinimal.h"
#include "Pickups/STUBasePickup.h"
#include "STUAmmoPickup.generated.h"

class ASTUBaseWeapon;

UCLASS()
class SHOOTTHEMUP_API ASTUAmmoPickup : public ASTUBasePickup
{
    GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1", ClampMax = "10"), meta = (AllowPrivateAccess = "true"))
    int32 ClipsAmount = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<ASTUBaseWeapon> WeaponType;
    
    virtual bool GivePickupTo(APawn* PlayerPawn) override;
};