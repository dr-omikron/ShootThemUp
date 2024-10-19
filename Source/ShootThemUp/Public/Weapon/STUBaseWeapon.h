
#pragma once

#include "CoreMinimal.h"
#include "NiagaraCommon.h"
#include "STUCoreTypes.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ASTUBaseWeapon();
    virtual void StartFireWeapon();
    virtual void StopFireWeapon();
    FORCEINLINE float GetTraceMaxDistance() const { return TraceMaxDistance; }
    FORCEINLINE FWeaponUIData GetUIData() const { return UIData; }
    FORCEINLINE FAmmoData GetAmmoData() const { return CurrentAmmo; }
    void ChangeClip();
    bool CanReload() const;
    bool TryToAddAmmo(int32 ClipsAmount);
    FOnClipEmptySignature OnClipEmpty;
    bool IsAmmoEmpty();
    bool IsAmmoFull() const;

protected:
    virtual void BeginPlay() override;
    virtual void MakeShot();
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
    APlayerController* GetPlayerController() const;
    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
    FVector GetMuzzleWorldLocation() const;
    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;
    void DecreaseAmmo();
    bool IsClipEmpty() const;
    void SpawnMuzzleFX() const;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* MuzzleFX;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    FName MuzzleSocketName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot Settings", meta = (AllowPrivateAccess = "true"))
    float TraceMaxDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
    FWeaponUIData UIData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    FAmmoData DefaultAmmo;

    FAmmoData CurrentAmmo;
};
