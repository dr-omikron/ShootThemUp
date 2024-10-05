

#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "STUHealthPickup.h"
#include "Components/ActorComponent.h"
#include "STUHeathComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUHeathComponent : public UActorComponent
{
    GENERATED_BODY()

public:	
    USTUHeathComponent();

    FOnDeathSignature OnDeath;
    FOnHealthChangeSignature OnHealthChange;

    FORCEINLINE float GetHealth() const { return Health; }

    UFUNCTION(BlueprintCallable, Category = "Health")
    bool IsDead() const { return FMath::IsNearlyZero(Health); }

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealthPercent() const { return Health / MaxHeath; }

    bool TryToAddHealth(float HealthAmount);
    bool IsHealthFull() const;

protected:
    virtual void BeginPlay() override;
    
    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
    
private:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "1.0", ClampMax = "1000.0"))
    float MaxHeath;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (AllowPrivateAccess = "true"))
    bool AutoHeal;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (AllowPrivateAccess = "true"), meta = (EditCondition = "AutoHeal"))
    float HealUpdateTime;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (AllowPrivateAccess = "true"), meta = (EditCondition = "AutoHeal"))
    float HealDelay;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (AllowPrivateAccess = "true"), meta = (EditCondition = "AutoHeal"))
    float HealModifier;
    
    float Health;
    FTimerHandle HealTimerHandle;

    void HealUpdate();
    void SetHealth(float NewHealth);
};
