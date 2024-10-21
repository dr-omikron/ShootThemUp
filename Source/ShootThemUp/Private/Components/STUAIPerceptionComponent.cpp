#include "Components/STUAIPerceptionComponent.h"

#include "AIController.h"
#include "STUHeathComponent.h"
#include "STUUtils.h"
#include "Perception/AISense_Sight.h"

AActor* USTUAIPerceptionComponent::GetClosestEnemy() const
{
    TArray<AActor*> PerceivedActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
    if(PerceivedActors.Num() == 0) return nullptr;
    const auto Controller = Cast<AAIController>(GetOwner());
    if(!Controller) return nullptr;
    const auto Pawn = Controller->GetPawn();
    if(!Pawn) return nullptr;
    float BestDistance = MAX_FLT;
    AActor* BestPawn = nullptr;
    for (auto PerceivedActor : PerceivedActors)
    {
        const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHeathComponent>(PerceivedActor);
        const auto PerceivePawn = Cast<APawn>(PerceivedActor);
        const auto AreEnemies = PerceivePawn && STUUtils::AreEnemies(Controller, PerceivePawn->Controller);
        if(HealthComponent && !HealthComponent->IsDead() && AreEnemies)
        {
            const auto CurrentDistance = (PerceivedActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
            if(CurrentDistance < BestDistance)
            {
                BestDistance = CurrentDistance;
                BestPawn = PerceivedActor;
            }
        }
    }
    return BestPawn;
}