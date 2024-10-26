#include "AI/STUAICharacter.h"
#include "BrainComponent.h"
#include "STUAIController.h"
#include "STUAIWeaponComponent.h"
#include "STUHealthBarWidget.h"
#include "STUHeathComponent.h"
#include "Components/WidgetComponent.h"
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
    HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Health Widget Component");
    HealthWidgetComponent->SetupAttachment(GetRootComponent());
    HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    HealthWidgetComponent->SetDrawAtDesiredSize(true);
}

void ASTUAICharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    UpdateHealthWidgetVisibility();
}

void ASTUAICharacter::BeginPlay()
{
    Super::BeginPlay();
    check(HealthWidgetComponent);
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

void ASTUAICharacter::OnHealthChanged(float Health, float HealthDelta) const
{
    Super::OnHealthChanged(Health, HealthDelta);
    const auto HealthBarWidget = Cast<USTUHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
    if(!HealthBarWidget) return;
    HealthBarWidget->SetHeathPercent(HeathComponent->GetHealthPercent());
}

void ASTUAICharacter::UpdateHealthWidgetVisibility() const
{
    if(!GetWorld() ||
       !GetWorld()->GetFirstPlayerController() ||
       !GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()) return;
    const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation();
    const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation());
    HealthWidgetComponent->SetVisibility(Distance < HealthVisibilityDistance, true);
}