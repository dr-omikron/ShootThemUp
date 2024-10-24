#include "STUPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "STURespawnComponent.h"
#include "EnhancedInputComponent.h"
#include "STUGameModeBase.h"
#include "STUPlayerCharacter.h"
#include "STUWeaponComponent.h"
#include "GameFramework/GameModeBase.h"

ASTUPlayerController::ASTUPlayerController()
{
    RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("Respawn Component");
}

void ASTUPlayerController::BeginPlay()
{
    Super::BeginPlay();
    if(GetWorld())
    {
        if(const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()))
        {
            GameMode->OnMatchStateChange.AddUObject(this, &ASTUPlayerController::OnMatchStateChange);
        }
    }
}

void ASTUPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    OnNewPawn.Broadcast(InPawn);
    SetupInputComponent();
}


void ASTUPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    
    if(!InputComponent) return;
    
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(STUMappingContext, 0);
    }
    
    const auto STUCharacter = Cast<ASTUPlayerCharacter>(GetPawn());
    if(!STUCharacter || !STUCharacter->GetWeaponComponent()) return;
    
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, STUCharacter, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, STUCharacter, &ACharacter::StopJumping);
        EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ASTUPlayerController::OnPauseGame);
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, STUCharacter, &ASTUPlayerCharacter::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, STUCharacter, &ASTUPlayerCharacter::Look);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, STUCharacter, &ASTUPlayerCharacter::OnBeginSprint);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, STUCharacter, &ASTUPlayerCharacter::OnEndSprint);
        EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, STUCharacter->GetWeaponComponent(), &USTUWeaponComponent::StartFire);
        EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, STUCharacter->GetWeaponComponent(), &USTUWeaponComponent::StopFire);
        EnhancedInputComponent->BindAction(NextWeaponAction, ETriggerEvent::Started, STUCharacter->GetWeaponComponent(), &USTUWeaponComponent::NextWeapon);
        EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, STUCharacter->GetWeaponComponent(), &USTUWeaponComponent::ReloadWeapon);
    }
}

void ASTUPlayerController::OnPauseGame()
{
    if(!GetWorld() || !GetWorld()->GetAuthGameMode()) return;
    GetWorld()->GetAuthGameMode()->SetPause(this);
}

void ASTUPlayerController::OnMatchStateChange(ESTUMatchState State)
{
    if(State == ESTUMatchState::InProgress)
    {
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;
    }
    else
    {
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
    }
}