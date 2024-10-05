#pragma once

#include "CoreMinimal.h"
#include "Pickups/STUBasePickup.h"
#include "STUHealthPickup.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUHealthPickup : public ASTUBasePickup
{
    GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "100.0"), meta = (AllowPrivateAccess = "true"))
    float HealthAmount = 100.f;
    
    virtual bool GivePickupTo(APawn* PlayerPawn) override;
};