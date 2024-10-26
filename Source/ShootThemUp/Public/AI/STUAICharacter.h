#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "STUAICharacter.generated.h"

class UWidgetComponent;
class UBehaviorTree;

UCLASS()
class SHOOTTHEMUP_API ASTUAICharacter : public ASTUBaseCharacter
{
    GENERATED_BODY()
public:
    ASTUAICharacter(const FObjectInitializer& ObjectInitializer);

    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    UBehaviorTree* BehaviorTreeAsset;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UWidgetComponent* HealthWidgetComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    float HealthVisibilityDistance = 1000.f;

    virtual void BeginPlay() override;
    virtual void OnDeath() override;
    virtual void OnHealthChanged(float Health, float HealthDelta) const override;

private:
    void UpdateHealthWidgetVisibility() const;
};