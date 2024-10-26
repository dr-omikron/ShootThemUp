#include "Menu/STULevelItemWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void USTULevelItemWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if(LevelSelectButton)
    {
        LevelSelectButton->OnClicked.AddDynamic(this, &USTULevelItemWidget::OnLevelItemClicked);
    }
}

void USTULevelItemWidget::SetLevelData(const FLevelData& Data)
{
    LevelData = Data;
    if(LevelNameTextBlock)
    {
        LevelNameTextBlock->SetText(FText::FromName(Data.LevelDisplayName));
    }
    if(LevelImage)
    {
        LevelImage->SetBrushFromTexture(Data.LevelThumbnail);
    }
}

void USTULevelItemWidget::SetSelected(const bool bIsSelected) const
{
    if(FrameImage)
    {
        FrameImage->SetVisibility(bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

void USTULevelItemWidget::OnLevelItemClicked()
{
    OnLevelSelected.Broadcast(LevelData);
}