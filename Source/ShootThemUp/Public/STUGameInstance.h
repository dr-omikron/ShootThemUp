#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "Engine/GameInstance.h"
#include "STUGameInstance.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    FORCEINLINE FLevelData GetStartupLevel() const { return StartupLevel; }
    FORCEINLINE void SetStartupLevel(const FLevelData& Data) { StartupLevel = Data; }
    FORCEINLINE TArray<FLevelData> GetLevelsData() const { return LevelsData; }
    FORCEINLINE FName GetMenuLevelName() const { return MenuLevelName; }

    void ToggleVolume() const;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game", meta = (ToolTip = "Level names must be unique!"))
    TArray<FLevelData> LevelsData;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName MenuLevelName = NAME_None;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    USoundClass* MasterSoundClass;

private:
    FLevelData StartupLevel;
};