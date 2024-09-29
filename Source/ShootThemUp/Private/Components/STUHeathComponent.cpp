
#include "STUHeathComponent.h"

USTUHeathComponent::USTUHeathComponent():
    MaxHeath(100.f),
    Heath(0.f)
{
    PrimaryComponentTick.bCanEverTick = false;

}

void USTUHeathComponent::BeginPlay()
{
    Super::BeginPlay();
    Heath = MaxHeath;
}

