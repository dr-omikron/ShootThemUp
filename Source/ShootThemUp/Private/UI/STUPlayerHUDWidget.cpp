
#include "UI/STUPlayerHUDWidget.h"
#include "STUHeathComponent.h"
#include "STUUtils.h"
#include "STUWeaponComponent.h"
#include "Components/ProgressBar.h"

int32 USTUPlayerHUDWidget::GetKillsNumber() const
{
    const auto Controller = GetOwningPlayer();
    if(!Controller) return 0;
    const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
    return PlayerState ? PlayerState->GetKillsNumber() : 0;
}

FString USTUPlayerHUDWidget::FormatBullets(const int32 BulletsNumber) const
{
    constexpr int32 MaxLength = 3;
    auto BulletString = FString::FromInt(BulletsNumber);
    const auto SymbolsNumberToAdd = MaxLength - BulletString.Len();
    if(SymbolsNumberToAdd > 0)
    {
        constexpr TCHAR PrefixSymbol = '0';
        BulletString = FString::ChrN(SymbolsNumberToAdd, PrefixSymbol).Append(BulletString);
    }
    return BulletString;
}

void USTUPlayerHUDWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if(GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &USTUPlayerHUDWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
}

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    if(const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHeathComponent>(GetOwningPlayerPawn()))
    {
        return HealthComponent->GetHealthPercent();
    }
    return 0.f;
}

bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    if(const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn()))
    {
        return WeaponComponent->GetCurrentWeaponUIData(UIData);
    }
    return false;
}

bool USTUPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    if(const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn()))
    {
        return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
    }
    return false;
}

bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
    if(const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHeathComponent>(GetOwningPlayerPawn()))
    {
        return HealthComponent && !HealthComponent->IsDead();
    }
    return false;
}

bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}

void USTUPlayerHUDWidget::OnHealthChange(float Health, float HealthDelta)
{
    if(HealthDelta < 0.f)
    {
        OnTakeDamage();
        if(!IsAnimationPlaying(DamageAnimation))
        {
            PlayAnimation(DamageAnimation);
        }
    }
    UpdateHealthBar();
}

void USTUPlayerHUDWidget::OnNewPawn(APawn* Pawn)
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHeathComponent>(Pawn);
    if(HealthComponent && !HealthComponent->OnHealthChange.IsBoundToObject(this))
    {
        HealthComponent->OnHealthChange.AddUObject(this, &USTUPlayerHUDWidget::OnHealthChange);
    }
    UpdateHealthBar();
}

void USTUPlayerHUDWidget::UpdateHealthBar() const
{
    if(HealthProgressBar)
    {
        HealthProgressBar->SetFillColorAndOpacity(GetHealthPercent() > PercentColorThreshold ? GoodColor : BadColor);
    }
}
