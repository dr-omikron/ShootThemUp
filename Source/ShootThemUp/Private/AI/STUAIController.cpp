#include "AI/STUAIController.h"

#include "STUAICharacter.h"
#include "STUAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ASTUAIController::ASTUAIController() :
    FocusOnKeyName("EnemyActor")
{
    AIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("AI Perception Component");
    SetPerceptionComponent(*AIPerceptionComponent);
}

void ASTUAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    const auto AimActor = GetFocusOnActor();
    SetFocus(AimActor);
}

void ASTUAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if(const auto AICharacter = Cast<ASTUAICharacter>(InPawn))
    {
        RunBehaviorTree(AICharacter->BehaviorTreeAsset);
    }
}

AActor* ASTUAIController::GetFocusOnActor()
{
    if(!GetBlackboardComponent()) return nullptr;
    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}