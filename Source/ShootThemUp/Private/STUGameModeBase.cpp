#include "STUGameModeBase.h"
#include "AIController.h"
#include "EngineUtils.h"
#include "STUBaseCharacter.h"
#include "STUGameHUD.h"
#include "STUPlayerController.h"
#include "STUPlayerState.h"
#include "STURespawnComponent.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All);

constexpr static int32 MinRoundTimeForRespawn = 10;

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
    PlayerStateClass = ASTUPlayerState::StaticClass();
}

void ASTUGameModeBase::StartPlay()
{
    Super::StartPlay();
    SpawnBots();
    CreateTeamsInfo();
    CurrentRound = 1;
    StartRound();
    SetMatchState(ESTUMatchState::InProgress);
}

UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if(InController && InController->IsA<AAIController>())
    {
        return AIPawnClass;
    }
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

bool ASTUGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);
    if(PauseSet)
    {
        SetMatchState(ESTUMatchState::Pause);
    }
    return PauseSet;
}

bool ASTUGameModeBase::ClearPause()
{
    const auto PauseCleared = Super::ClearPause();
    if(PauseCleared)
    {
        SetMatchState(ESTUMatchState::InProgress);
    }
    return PauseCleared;
}

void ASTUGameModeBase::SpawnBots()
{
    if(!GetWorld()) return;
    for(int i = 0; i < GameData.PlayersNumber - 1; ++i)
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        const auto AIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
        RestartPlayer(AIController);
    }
}

void ASTUGameModeBase::StartRound()
{
    RoundCountDown = GameData.RoundTime;
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ASTUGameModeBase::GameTimerUpdate, 1.f, true);
}

void ASTUGameModeBase::GameTimerUpdate()
{
    UE_LOG(LogSTUGameModeBase, Display, TEXT("Time: %i / Round: %i/%i"), RoundCountDown, CurrentRound, GameData.RoundsNumber);
    if(--RoundCountDown == 0)
    {
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);
        if(CurrentRound + 1 <= GameData.RoundsNumber)
        {
            ++CurrentRound;
            ResetPlayers();
            StartRound();
        }
        else
        {
            GameOver();
        }
    }
}

void ASTUGameModeBase::ResetPlayers()
{
    if(!GetWorld()) return;
    for(auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        ResetOnePlayer(It->Get());
    }
}

void ASTUGameModeBase::ResetOnePlayer(AController* Controller)
{
    if(Controller && Controller->GetPawn())
    {
        Controller->GetPawn()->Reset();
    }
    RestartPlayer(Controller);
    SetPlayerColor(Controller);
}

void ASTUGameModeBase::CreateTeamsInfo()
{
    if(!GetWorld()) return;
    int32 TeamID = 1;
    for(auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if(!Controller) continue;
        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if(!PlayerState) continue;
        PlayerState->SetTeamID(TeamID);
        PlayerState->SetTeamColor(DeterminateColorByTeamID(TeamID));
        PlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : "Bot");
        SetPlayerColor(Controller);
        TeamID = TeamID == 1 ? 2 : 1;
    }
}

FLinearColor ASTUGameModeBase::DeterminateColorByTeamID(const int32 TeamID) const
{
    if(TeamID - 1 < GameData.TeamColors.Num())
    {
        return GameData.TeamColors[TeamID - 1];
    }
    UE_LOG(LogSTUGameModeBase, Warning, TEXT("No color for team id: %i, set to default: %s"), TeamID, *GameData.DefaultTeamColor.ToString());
    return GameData.DefaultTeamColor;
}

void ASTUGameModeBase::SetPlayerColor(const AController* Controller)
{
    if(!Controller) return;
    const auto Character = Cast<ASTUBaseCharacter>(Controller->GetPawn());
    if(!Character) return;
    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
    if(!PlayerState) return;
    Character->SetPlayerColor(PlayerState->GetTeamColor());
}

void ASTUGameModeBase::Killed(const AController* KillerController, AController* VictimController)
{
    const auto KillerPlayerState = KillerController ? Cast<ASTUPlayerState>(KillerController->PlayerState) : nullptr;
    const auto VictimPlayerState = VictimController ? Cast<ASTUPlayerState>(VictimController->PlayerState) : nullptr;
    if(KillerPlayerState)
    {
        KillerPlayerState->AddKill();
    }
    if(VictimPlayerState)
    {
        VictimPlayerState->AddDeath();
    }
    StartRespawn(VictimController);
}

void ASTUGameModeBase::LogPlayerInfo() const
{
    if(!GetWorld()) return;
    for(auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if(!Controller) continue;
        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if(!PlayerState) continue;
        PlayerState->LogInfo();
    }
}

void ASTUGameModeBase::RespawnRequest(AController* Controller)
{
    ResetOnePlayer(Controller);
}

void ASTUGameModeBase::StartRespawn(AController* Controller) const
{
    const auto RespawnAvailable = RoundCountDown > MinRoundTimeForRespawn + GameData.RespawnTime;
    if(!RespawnAvailable) return;
    const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(Controller);
    if(!RespawnComponent) return;
    RespawnComponent->Respawn(GameData.RespawnTime);
}

void ASTUGameModeBase::GameOver()
{
    UE_LOG(LogSTUGameModeBase, Display, TEXT("=============== GAME OVER =============="))
    LogPlayerInfo();
    for(const auto Pawn : TActorRange<APawn>(GetWorld()))
    {
        if(Pawn)
        {
            Pawn->TurnOff();
            Pawn->DisableInput(nullptr);
        }
    }
    SetMatchState(ESTUMatchState::GameOver);
}

void ASTUGameModeBase::SetMatchState(const ESTUMatchState State)
{
    if(MatchState == State) return;
    MatchState = State;
    OnMatchStateChange.Broadcast(MatchState);
}