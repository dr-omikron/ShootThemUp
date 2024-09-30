
#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()
public:
    ASTURifleWeapon();
    virtual void StartFireWeapon() override;
    virtual void StopFireWeapon() override;
    
protected:
    virtual void MakeShot() override;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;
    
private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot Settings", meta = (AllowPrivateAccess = "true"))
    float TimeBetweenShots;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot Settings", meta = (AllowPrivateAccess = "true"))
    float BulletSpread;

    FTimerHandle ShotTimerHandle;
};
