
#include "STUGameHUD.h"

#include "STUGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameHUD, All, All);

void ASTUGameHUD::BeginPlay()
{
    Super::BeginPlay();

    GameWidgets.Add(ESTUMatchState::InProgress, CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass));
    GameWidgets.Add(ESTUMatchState::Pause, CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass));
    GameWidgets.Add(ESTUMatchState::GameOver, CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass));

    for (const auto WidgetPair : GameWidgets)
    {
        const auto GameWidget = WidgetPair.Value;
        if(!GameWidget) continue;
        GameWidget->AddToViewport();
        GameWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if(GetWorld())
    {
        if(const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()))
        {
            GameMode->OnMatchStateChange.AddUObject(this, &ASTUGameHUD::OnMatchStateChange);
        }
    }
}

void ASTUGameHUD::DrawHUD()
{
    Super::DrawHUD();
    //DrawCrosshair();
}

void ASTUGameHUD::DrawCrosshair()
{
    const TInterval<float> Center(Canvas->SizeX * 0.5, Canvas->SizeY * 0.5);
    constexpr float HalfLineSize = 10.f;
    constexpr float LineThickness = 2.f;
    const FLinearColor LineColor = FLinearColor::Green;
    DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
    DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}

void ASTUGameHUD::OnMatchStateChange(const ESTUMatchState State)
{
    if(CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if(GameWidgets.Contains(State))
    {
        CurrentWidget = GameWidgets[State];
    }

    if(CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
    }

    UE_LOG(LogSTUGameHUD, Display, TEXT("Match state changed: %s"), *UEnum::GetValueAsString(State));
}