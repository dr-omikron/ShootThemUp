#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "Components/ActorComponent.h"
#include "STUWeaponFXComponent.generated.h"

class UNiagaraSystem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponFXComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USTUWeaponFXComponent();

    void PlayImpactFX(const FHitResult& HitResult) const;

private:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX", meta = (AllowPrivateAccess = "true"))
    FImpactData DefaultImpactData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX", meta = (AllowPrivateAccess = "true"))
    TMap<UPhysicalMaterial*, FImpactData> ImpactDataMap;
};