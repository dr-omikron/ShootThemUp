#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "STUPlayerCharacter.generated.h"

class USphereComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerCharacter : public ASTUBaseCharacter
{
    GENERATED_BODY()
public:
    ASTUPlayerCharacter(const FObjectInitializer& ObjectInitializer);
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void OnBeginSprint();
    void OnEndSprint();

protected:
    virtual bool IsSprinting() const override;
    virtual void BeginPlay() override;
    virtual void OnDeath() override;

    UFUNCTION()
    void OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USphereComponent* CameraCollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* SpringArmComponent;

    void CheckCameraOverlap() const;

    bool bWantsToRun;
    bool bIsMovingForward;
};