
#include "STUDevDamageActor.h"

#include "Kismet/GameplayStatics.h"

ASTUDevDamageActor::ASTUDevDamageActor():
    Radius(300.f),
    SphereColor(FColor::Red),
    Damage(10.f),
    bDoFullDamage(false)
{
    PrimaryActorTick.bCanEverTick = true;
    SceneComponent = CreateDefaultSubobject<USceneComponent>("Scene Component");
    SetRootComponent(SceneComponent);
}

void ASTUDevDamageActor::BeginPlay()
{
    Super::BeginPlay();

}

void ASTUDevDamageActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 24, SphereColor);
    UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radius,
        DamageType, {}, this, nullptr, bDoFullDamage);
}

