#include "STUGameModeBase.h"
#include "STUBaseCharacter.h"
#include "STUGameHUD.h"
#include "STUPlayerController.h"

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
}