#include "Components/STURespawnComponent.h"

#include "STUGameModeBase.h"

USTURespawnComponent::USTURespawnComponent():
    RespawnCountdown(0)
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool USTURespawnComponent::IsRespawnInProgress() const
{
    return GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle);
}

void USTURespawnComponent::Respawn(int32 RespawnTime)
{
    if(!GetWorld()) return;
    RespawnCountdown = RespawnTime;
    GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &USTURespawnComponent::RespawnTimerUpdate, 1.f, true);
}

void USTURespawnComponent::RespawnTimerUpdate()
{
    if(--RespawnCountdown == 0)
    {
        if(!GetWorld()) return;
        GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
        if(!GameMode) return;
        GameMode->RespawnRequest(Cast<AController>(GetOwner()));
    }
}