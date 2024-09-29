#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUDevDamageActor.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUDevDamageActor : public AActor
{
    GENERATED_BODY()

public:	
    ASTUDevDamageActor();
    virtual void Tick(float DeltaTime) override;
    
protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USceneComponent* SceneComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug Sphere", meta = (AllowPrivateAccess = "true"))
    float Radius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug Sphere", meta = (AllowPrivateAccess = "true"))
    FColor SphereColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
    float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
    bool bDoFullDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UDamageType> DamageType;
};
