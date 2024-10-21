#include "Player/STUPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "STUWeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

ASTUPlayerCharacter::ASTUPlayerCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer),
    CameraComponent(nullptr),
    bWantsToRun(false),
    bIsMovingForward(false)
{
    PrimaryActorTick.bCanEverTick = true;
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("Spring Arm Component");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0.f, 100.f, 80.f);
    CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera Component");
    CameraComponent->SetupAttachment(SpringArmComponent);
    CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("Camera Collision Component");
    CameraCollisionComponent->SetupAttachment(CameraComponent);
    CameraCollisionComponent->SetSphereRadius(10.f);
    CameraCollisionComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void ASTUPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    check(CameraCollisionComponent);
    CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionBeginOverlap);
    CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionEndOverlap);
}

void ASTUPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    check(PlayerInputComponent);

    if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(STUMappingContext, 0);
        }
    }

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::Look);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::OnBeginSprint);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASTUPlayerCharacter::OnEndSprint);
        EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, GetWeaponComponent(), &USTUWeaponComponent::StartFire);
        EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, GetWeaponComponent(), &USTUWeaponComponent::StopFire);
        EnhancedInputComponent->BindAction(NextWeaponAction, ETriggerEvent::Started, GetWeaponComponent(), &USTUWeaponComponent::NextWeapon);
        EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, GetWeaponComponent(), &USTUWeaponComponent::ReloadWeapon);
    }
}

void ASTUPlayerCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();
    if(MovementVector.IsZero()) return;
    bIsMovingForward = MovementVector.Y > 0.f;
    if (Controller != nullptr)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void ASTUPlayerCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void ASTUPlayerCharacter::OnBeginSprint()
{
    bWantsToRun = true;
}

void ASTUPlayerCharacter::OnEndSprint()
{
    bWantsToRun = false;
}

bool ASTUPlayerCharacter::IsSprinting() const
{
    return bWantsToRun && bIsMovingForward && !GetVelocity().IsZero();
}

void ASTUPlayerCharacter::OnDeath()
{
    Super::OnDeath();
    if(Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}

void ASTUPlayerCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    CheckCameraOverlap();
}

void ASTUPlayerCharacter::OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    CheckCameraOverlap();
}

void ASTUPlayerCharacter::CheckCameraOverlap() const
{
    const auto HideMesh = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
    GetMesh()->SetOwnerNoSee(HideMesh);
    TArray<USceneComponent*> MeshChildren;
    GetMesh()->GetChildrenComponents(true, MeshChildren);
    for (const auto MeshChild : MeshChildren)
    {
        if(const auto MeshChildGeometry = Cast<UPrimitiveComponent>(MeshChild))
        {
            MeshChildGeometry->SetOwnerNoSee(HideMesh);
        }
    }
}