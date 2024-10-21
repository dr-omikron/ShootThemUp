#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "STUPlayerState.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerState : public APlayerState
{
    GENERATED_BODY()
public:
    FORCEINLINE void SetTeamID(const int32 ID) { TeamID = ID; }
    FORCEINLINE int32 GetTeamID() const { return TeamID; }
    FORCEINLINE void SetTeamColor(const FLinearColor Color) { TeamColor = Color; }
    FORCEINLINE FLinearColor GetTeamColor() const { return TeamColor; }
    FORCEINLINE void AddKill() { ++Kills; }
    FORCEINLINE void AddDeath() { ++Deaths; }
    FORCEINLINE int32 GetKillsNumber() const { return Kills; }
    FORCEINLINE int32 GetDeathsNumber() const { return Deaths; }
    void LogInfo() const;

private:
    int32 TeamID;
    FLinearColor TeamColor;
    int32 Kills = 0;
    int32 Deaths = 0;
};