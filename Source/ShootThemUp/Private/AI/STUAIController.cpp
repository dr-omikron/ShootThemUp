#include "AI/STUAIController.h"
#include "STUAICharacter.h"
#include "STUAIPerceptionComponent.h"
#include "STURespawnComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ASTUAIController::ASTUAIController() :
    FocusOnKeyName("EnemyActor")
{
    AIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("AI Perception Component");
    SetPerceptionComponent(*AIPerceptionComponent);
    RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("Respawn Component");
    bWantsPlayerState = true;
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