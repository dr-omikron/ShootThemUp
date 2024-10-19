#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "STUHeathPercentDecorator.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUHeathPercentDecorator : public UBTDecorator
{
    GENERATED_BODY()

public:
    USTUHeathPercentDecorator();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float HeathPercent = 0.6f;

    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};