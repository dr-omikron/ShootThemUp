#include "Pickups/STUBasePickup.h"
#include "Components/SphereComponent.h"

ASTUBasePickup::ASTUBasePickup():
    RespawnTime(5.f)
{
    PrimaryActorTick.bCanEverTick = true;
    CollisionComponent = CreateDefaultSubobject<USphereComponent>("Sphere Collision");
    CollisionComponent->InitSphereRadius(50.f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
    SetRootComponent(CollisionComponent);
}

void ASTUBasePickup::BeginPlay()
{
    Super::BeginPlay();
    check(CollisionComponent);
    GenerateRotationYaw();
}

void ASTUBasePickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    AddActorLocalRotation(FRotator(0.f, RotationYaw, 0.f));
}

void ASTUBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);
    const auto Pawn = Cast<APawn>(OtherActor);
    if(GivePickupTo(Pawn))
    {
        PickupWasTaken();
    }
}

bool ASTUBasePickup::GivePickupTo(APawn* PlayerPawn)
{
    return false;
}

void ASTUBasePickup::PickupWasTaken()
{
    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    if(!GetRootComponent()) return;
    GetRootComponent()->SetVisibility(false, true);
    FTimerHandle RespawnTimerHandle;
    GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASTUBasePickup::Respawn, RespawnTime);
}

void ASTUBasePickup::Respawn()
{
    GenerateRotationYaw();
    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
    if(!GetRootComponent()) return;
    GetRootComponent()->SetVisibility(true, true);
}

void ASTUBasePickup::GenerateRotationYaw()
{
    const float Direction = FMath::RandBool() ? 1.f : -1.f;
    RotationYaw = FMath::RandRange(1.f, 2.f) * Direction;
}
