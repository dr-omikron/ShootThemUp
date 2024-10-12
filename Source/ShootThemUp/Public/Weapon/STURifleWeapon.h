
#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

class USTUWeaponFXComponent;

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()
public:
    ASTURifleWeapon();
    virtual void BeginPlay() override;
    virtual void StartFireWeapon() override;
    virtual void StopFireWeapon() override;
    
protected:
    virtual void MakeShot() override;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;
    void MakeDamage(const FHitResult& HitResult);
private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot Settings", meta = (AllowPrivateAccess = "true"))
    float TimeBetweenShots;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot Settings", meta = (AllowPrivateAccess = "true"))
    float BulletSpread;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot Settings", meta = (AllowPrivateAccess = "true"))
    float DamageAmount;

    UPROPERTY(VisibleAnywhere, Category = "VFX", meta = (AllowPrivateAccess = "true"))
    USTUWeaponFXComponent* WeaponFXComponent;
    
    FTimerHandle ShotTimerHandle;
};
