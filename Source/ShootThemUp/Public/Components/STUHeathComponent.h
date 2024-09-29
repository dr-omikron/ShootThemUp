

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUHeathComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUHeathComponent : public UActorComponent
{
    GENERATED_BODY()

public:	
    USTUHeathComponent();

    FORCEINLINE float GetHeath() const { return Heath; }
protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "1.0", ClampMax = "1000.0"))
    float MaxHeath;

    float Heath;
};
