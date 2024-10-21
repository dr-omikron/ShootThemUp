#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUGameDataWidget.generated.h"

class ASTUPlayerState;
class ASTUGameModeBase;

UCLASS()
class SHOOTTHEMUP_API USTUGameDataWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    int32 GetKillsNumber() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    int32 GetCurrentRoundNumber() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    int32 GetTotalRoundNumber() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    int32 GetRoundSecondsRemaining() const;

private:
    ASTUGameModeBase* GetSTUGameMode() const;
    ASTUPlayerState* GetSTUPlayerState() const;

};