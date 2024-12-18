#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBasePickup.generated.h"

class USphereComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBasePickup : public AActor
{
    GENERATED_BODY()

public:
    ASTUBasePickup();
    virtual void Tick(float DeltaTime) override;
    bool CouldBeTaken() const;

protected:
    virtual void BeginPlay() override;
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
    virtual bool GivePickupTo(APawn* PlayerPawn);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds")
    USoundBase* PickupTakenSound;

private:
    UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USphereComponent* CollisionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
    float RespawnTime;

    FTimerHandle RespawnTimerHandle;
    float RotationYaw = 0.f;

    void PickupWasTaken();
    void Respawn();
    void GenerateRotationYaw();
};