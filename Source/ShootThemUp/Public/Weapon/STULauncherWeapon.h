
#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STULauncherWeapon.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTULauncherWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()
public:
    virtual void StartFireWeapon();
    virtual void StopFireWeapon();

protected:
    virtual void MakeShot() override;
};
