#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"

USTUWeaponFXComponent::USTUWeaponFXComponent():
    DefaultEffect(nullptr)
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& HitResult) const
{
    auto Effect = DefaultEffect;
    if(HitResult.PhysMaterial.IsValid())
    {
        const auto PhysMat = HitResult.PhysMaterial.Get();
        if(EffectsMap.Contains(PhysMat))
        {
            Effect = EffectsMap[PhysMat];
        }
    }
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation()); 
}
