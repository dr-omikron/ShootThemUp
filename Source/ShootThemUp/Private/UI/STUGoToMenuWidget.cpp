#include "UI/STUGoToMenuWidget.h"

#include "STUGameInstance.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGoToMenuWidget, All, All);

void USTUGoToMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if(GoToMenuButton)
    {
        GoToMenuButton->OnClicked.AddDynamic(this, &USTUGoToMenuWidget::OnGoToMenu);
    }
}

void USTUGoToMenuWidget::OnGoToMenu()
{
    if(!GetWorld()) return;
    const auto GInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
    if(!GInstance) return;
    if(GInstance->GetMenuLevelName().IsNone())
    {
        UE_LOG(LogSTUGoToMenuWidget, Error, TEXT("Menu Level Name Is NONE"));
        return;
    }
    UGameplayStatics::OpenLevel(this, GInstance->GetMenuLevelName());
}