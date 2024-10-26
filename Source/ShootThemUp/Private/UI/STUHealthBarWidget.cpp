#include "UI/STUHealthBarWidget.h"
#include "Components/ProgressBar.h"

void USTUHealthBarWidget::SetHeathPercent(const float Percent) const
{
    if(!HeathProgressBar) return;
    const auto HealthBarVisibility = (Percent > PercentVisibilityThreshold || FMath::IsNearlyZero(Percent))
                                        ? ESlateVisibility::Hidden
                                        : ESlateVisibility::Visible;
    HeathProgressBar->SetVisibility(HealthBarVisibility);
    const auto HealthBarColor = Percent > PercentColorThreshold ? GoodColor : BadColor;
    HeathProgressBar->SetFillColorAndOpacity(HealthBarColor);
    HeathProgressBar->SetPercent(Percent);
}