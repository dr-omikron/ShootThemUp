#include "Menu/STUMenuHUD.h"

#include "Blueprint/UserWidget.h"

void ASTUMenuHUD::BeginPlay()
{
    Super::BeginPlay();
    if(MenuWidgetClass)
    {
        if(const auto MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass))
        {
            MenuWidget->AddToViewport();
        }
    }
}