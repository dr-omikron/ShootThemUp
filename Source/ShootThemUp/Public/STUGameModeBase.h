#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "GameFramework/GameModeBase.h"
#include "STUGameModeBase.generated.h"

class AAIController;

UCLASS()
class SHOOTTHEMUP_API ASTUGameModeBase : public AGameModeBase
{
    GENERATED_BODY()
public:
    ASTUGameModeBase();

    virtual void StartPlay() override;
    virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
    void Killed(const AController* KillerController, AController* VictimController);

    FORCEINLINE FGameData GetGameData() const { return GameData; }
    FORCEINLINE int32 GetCurrentRoundNumber() const { return CurrentRound; }
    FORCEINLINE int32 GetCurrentSecondsRemaining() const { return RoundCountDown; }
    void RespawnRequest(AController* Controller);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<AAIController> AIControllerClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<APawn> AIPawnClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FGameData GameData;

private:
    int32 CurrentRound = 1;
    int32 RoundCountDown = 0;
    FTimerHandle GameRoundTimerHandle;

    void SpawnBots();
    void StartRound();
    void GameTimerUpdate();
    void ResetPlayers();
    void ResetOnePlayer(AController* Controller);
    void CreateTeamsInfo();
    FLinearColor DeterminateColorByTeamID(int32 TeamID) const;
    void SetPlayerColor(const AController* Controller);
    void LogPlayerInfo() const;
    void StartRespawn(AController* Controller) const;
    void GameOver() const;
};