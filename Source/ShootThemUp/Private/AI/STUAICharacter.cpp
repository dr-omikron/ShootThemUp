#include "AI/STUAICharacter.h"

#include "BrainComponent.h"
#include "STUAIController.h"
#include "STUAIWeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer.SetDefaultSubobjectClass<USTUAIWeaponComponent>("Weapon Component")),
    BehaviorTreeAsset(nullptr)
{
    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = ASTUAIController::StaticClass();
    bUseControllerRotationYaw = false;
    if(GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.f, 200.f, 0.f);
    }
}

void ASTUAICharacter::OnDeath()
{
    Super::OnDeath();
    const auto AIController = Cast<AAIController>(Controller);
    if(AIController && AIController->BrainComponent)
    {
        AIController->BrainComponent->Cleanup();
    }
}