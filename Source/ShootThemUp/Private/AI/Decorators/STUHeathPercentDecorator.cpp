#include "AI/Decorators/STUHeathPercentDecorator.h"

#include "AIController.h"
#include "STUHeathComponent.h"
#include "STUUtils.h"

USTUHeathPercentDecorator::USTUHeathPercentDecorator()
{
    NodeName = "HeathPercent";
}

bool USTUHeathPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if(!Controller) return false;
    const auto HeathComponent = STUUtils::GetSTUPlayerComponent<USTUHeathComponent>(Controller->GetPawn());
    if(!HeathComponent || HeathComponent->IsDead()) return false;
    return HeathComponent->GetHealthPercent() <= HeathPercent;
}