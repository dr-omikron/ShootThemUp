
#include "STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "STUCharacterMovementComponent.h"
#include "STUHeathComponent.h"
#include "STUWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All)

ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)),
    LandedDamageVelocity(900.f, 1200.f),
    LandedDamage(10.f, 100.f),
    LifeSpanOnDeath(5.f),
    MaterialColorName("Paint Color")
{
    PrimaryActorTick.bCanEverTick = true;
    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("Weapon Component");
    HeathComponent = CreateDefaultSubobject<USTUHeathComponent>("Heath Component");
}

void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
    check(HeathComponent);
    check(WeaponComponent);
    check(GetCharacterMovement());
    check(GetMesh());
    OnHealthChanged(HeathComponent->GetHealth(), 0.f);
    HeathComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
    HeathComponent->OnHealthChange.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);
    LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);
}

void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

float ASTUBaseCharacter::GetMovementDirection() const
{
    if(GetVelocity().IsZero()) return 0.f;
    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
    return  CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ASTUBaseCharacter::SetPlayerColor(const FLinearColor& LinearColor) const
{
    const auto MaterialInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
    if(!MaterialInstance) return;
    MaterialInstance->SetVectorParameterValue(MaterialColorName, LinearColor);
}

void ASTUBaseCharacter::OnHealthChanged(float Health, float HealthDelta) const
{

}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    const auto FallVelocityZ = -GetVelocity().Z;
    UE_LOG(LogBaseCharacter, Display, TEXT("On landed: %f"), FallVelocityZ);
    if(FallVelocityZ < LandedDamageVelocity.X) return;
    const float FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
    UE_LOG(LogBaseCharacter, Display, TEXT("Final Damage: %f"), FinalDamage);
    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

void ASTUBaseCharacter::OnDeath()
{
    //PlayAnimMontage(DeathAnimation);
    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(LifeSpanOnDeath);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
    WeaponComponent->StopFire();
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
}
