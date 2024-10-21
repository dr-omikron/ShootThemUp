#include "UI/STUGameDataWidget.h"

#include "STUGameModeBase.h"
#include "STUPlayerState.h"

int32 USTUGameDataWidget::GetKillsNumber() const
{
    const auto PlayerState = GetSTUPlayerState();
    return PlayerState ? PlayerState->GetKillsNumber() : 0;
}

int32 USTUGameDataWidget::GetCurrentRoundNumber() const
{
    const auto GameMode = GetSTUGameMode();
    return GameMode ? GameMode->GetCurrentRoundNumber() : 0;
}

int32 USTUGameDataWidget::GetTotalRoundNumber() const
{
    const auto GameMode = GetSTUGameMode();
    return GameMode ? GameMode->GetGameData().RoundsNumber : 0;
}

int32 USTUGameDataWidget::GetRoundSecondsRemaining() const
{
    const auto GameMode = GetSTUGameMode();
    return GameMode ? GameMode->GetCurrentSecondsRemaining() : 0;
}

ASTUGameModeBase* USTUGameDataWidget::GetSTUGameMode() const
{
    return GetWorld() ? Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}

ASTUPlayerState* USTUGameDataWidget::GetSTUPlayerState() const
{
    return GetOwningPlayer() ? Cast<ASTUPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
}