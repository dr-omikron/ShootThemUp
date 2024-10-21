#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STURespawnComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTURespawnComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USTURespawnComponent();

    FORCEINLINE int32 GetRespawnCountdown() const { return RespawnCountdown; }
    bool IsRespawnInProgress() const;
    void Respawn(int32 RespawnTime);

private:
    FTimerHandle RespawnTimerHandle;
    int32 RespawnCountdown;

    void RespawnTimerUpdate();
};