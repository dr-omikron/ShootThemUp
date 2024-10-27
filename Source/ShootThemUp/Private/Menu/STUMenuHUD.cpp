#include "Menu/STUMenuHUD.h"

#include "STUBaseWidget.h"
#include "Blueprint/UserWidget.h"

void ASTUMenuHUD::BeginPlay()
{
    Super::BeginPlay();
    if(MenuWidgetClass)
    {
        if(const auto MenuWidget = CreateWidget<USTUBaseWidget>(GetWorld(), MenuWidgetClass))
        {
            MenuWidget->AddToViewport();
            MenuWidget->Show();
        }
    }
}