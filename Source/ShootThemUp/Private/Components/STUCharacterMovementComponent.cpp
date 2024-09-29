
#include "STUCharacterMovementComponent.h"

#include "STUBaseCharacter.h"

float USTUCharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed();
    const ASTUBaseCharacter* Player = Cast<ASTUBaseCharacter>(GetPawnOwner());
    return Player && Player->IsSprinting() ? MaxSpeed * SprintModifier : MaxSpeed;
}