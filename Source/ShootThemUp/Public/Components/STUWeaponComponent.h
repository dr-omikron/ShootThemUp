
#pragma once

#include "CoreMinimal.h"
#include "STUAmmoPickup.h"
#include "STUCoreTypes.h"
#include "Components/ActorComponent.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USTUWeaponComponent();

    FORCEINLINE ASTUBaseWeapon* GetCurrentWeapon() const { return CurrentWeapon; }
    virtual void StartFire();
    void StopFire();
    virtual void NextWeapon();
    void ReloadWeapon();
    bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;
    bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;
    bool TryToAddAmmo(const TSubclassOf<ASTUBaseWeapon>& WeaponType, int32 ClipsAmount);

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    bool CanFire() const;
    bool CanEquip() const;
    void EquipWeapon(int32 WeaponIndex);

    UPROPERTY()
    ASTUBaseWeapon* CurrentWeapon;

    UPROPERTY()
    TArray<ASTUBaseWeapon*> Weapons;
    int32 CurrentWeaponIndex;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    TArray<FWeaponData> WeaponDates;
    
    UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    FName WeaponEquipSocketName;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    FName WeaponArmorySocketName;

    UPROPERTY(EditDefaultsOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    UAnimMontage* EquipAnimMontage;

    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage;

    bool bEquipAnimInProgress;
    bool bReloadAnimInProgress;

    void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
    void OnReloadFinished(USkeletalMeshComponent* MeshComponent);
    void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
    void SpawnWeapons();
    void PlayAnimMontage(UAnimMontage* AnimMontage) const;
    void InitAnimations();
    bool CanReload() const;
    void OnEmptyClip(ASTUBaseWeapon* EmptyWeapon);
    void ChangeClip();
};
