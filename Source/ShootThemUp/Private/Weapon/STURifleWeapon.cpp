
#include "Weapon/STURifleWeapon.h"

#include "STUWeaponFXComponent.h"
#include "Engine/DamageEvents.h"

ASTURifleWeapon::ASTURifleWeapon():
    TimeBetweenShots(0.1f),
    BulletSpread(1.5f),
    DamageAmount(10.f)
{
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("FX Component");
}

void ASTURifleWeapon::BeginPlay()
{
    Super::BeginPlay();
    check(WeaponFXComponent);
}

void ASTURifleWeapon::StartFireWeapon()
{
    Super::StartFireWeapon();
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
    MakeShot();
}

void ASTURifleWeapon::StopFireWeapon()
{
    Super::StopFireWeapon();
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ASTURifleWeapon::MakeShot()
{
    Super::MakeShot();
    if(!GetWorld() || IsAmmoEmpty())
    {
        StopFireWeapon();
        return;
    }
    FVector TraceStart, TraceEnd;
    if(!GetTraceData(TraceStart, TraceEnd))
    {
        StopFireWeapon();
        return;
    }
    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);
    if(HitResult.bBlockingHit)
    {
        MakeDamage(HitResult);
        //DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.f, 0, 3.f);
        //DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 24, FColor::Red, false, 5.f);
        WeaponFXComponent->PlayImpactFX(HitResult);
    }
    else
    {
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.f, 0, 3.f);
    }
    DecreaseAmmo();
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    Super::GetTraceData(TraceStart, TraceEnd);
    FVector ViewLocation;
    FRotator ViewRotation;
    if(!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;
    TraceStart = ViewLocation;
    const float HalfRad = FMath::DegreesToRadians(BulletSpread);
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad) ;
    TraceEnd = TraceStart + ShootDirection * GetTraceMaxDistance();
    return true;
}

void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult)
{
    const auto DamagedActor = HitResult.GetActor();
    if(!DamagedActor) return;
    DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}