#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class USTUWeaponComponent;
class USTUHeathComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    FORCEINLINE USTUWeaponComponent* GetWeaponComponent() const { return WeaponComponent; }
    ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer);
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    virtual bool IsSprinting() const { return false; };

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

    void SetPlayerColor(const FLinearColor& LinearColor) const;

protected:
    UFUNCTION()
    void OnHealthChanged(float Health, float HealthDelta) const;
    
    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);
    
    virtual void BeginPlay() override;
    virtual void OnDeath();

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USTUHeathComponent* HeathComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USTUWeaponComponent* WeaponComponent;

    UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
    FVector2D LandedDamageVelocity;

    UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
    FVector2D LandedDamage;

    UPROPERTY(EditDefaultsOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
    float LifeSpanOnDeath;

    UPROPERTY(EditDefaultsOnly, Category = "Material", meta = (AllowPrivateAccess = "true"))
    FName MaterialColorName;

    UPROPERTY(EditDefaultsOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    UAnimMontage* DeathAnimation;
};
