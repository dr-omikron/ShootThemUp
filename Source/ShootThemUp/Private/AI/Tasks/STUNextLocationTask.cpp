
#include "AI/Tasks/STUNextLocationTask.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

USTUNextLocationTask::USTUNextLocationTask():
    Radius(1000.f),
    SelfCenter(true)
{
    NodeName = "Next Location";
}

EBTNodeResult::Type USTUNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if(!Controller || !Blackboard) return  EBTNodeResult::Failed;
    const auto Pawn = Controller->GetPawn();
    if(!Pawn) return  EBTNodeResult::Failed;
    const auto NavigationSystem = UNavigationSystemV1::GetCurrent(Pawn);
    if(!NavigationSystem) return  EBTNodeResult::Failed;
    FNavLocation NavigationLocation;
    auto Location = Pawn->GetActorLocation();
    if(!SelfCenter)
    {
        const auto CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName));
        if(!CenterActor) return EBTNodeResult::Failed;
        Location = CenterActor->GetActorLocation();
    }
    const auto Found = NavigationSystem->GetRandomReachablePointInRadius(Location, Radius, NavigationLocation);
    if(!Found) return  EBTNodeResult::Failed;
    Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavigationLocation.Location);
    return  EBTNodeResult::Succeeded;
}