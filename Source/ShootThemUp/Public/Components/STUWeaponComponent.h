
#pragma once

#include "CoreMinimal.h"
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
    void StartFire();
    void StopFire();
    void NextWeapon();
    void ReloadWeapon();
    bool GetWeaponUIData(FWeaponUIData& UIData) const;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

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
    ASTUBaseWeapon* CurrentWeapon;

    UPROPERTY()
    TArray<ASTUBaseWeapon*> Weapons;

    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage;

    int32 CurrentWeaponIndex;
    bool bEquipAnimInProgress;
    bool bReloadAnimInProgress;

    void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
    void OnReloadFinished(USkeletalMeshComponent* MeshComponent);
    void EquipWeapon(int32 WeaponIndex);
    void AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
    void SpawnWeapons();
    void PlayAnimMontage(UAnimMontage* AnimMontage) const;
    void InitAnimations();
    bool CanFire() const;
    bool CanEquip() const;
    bool CanReload() const;
    void OnEmptyClip();
    void ChangeClip();
};
