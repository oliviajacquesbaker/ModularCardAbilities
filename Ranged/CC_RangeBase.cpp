// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_RangeBase.h"
#include "ProjectileObject.h"
#include "Kismet/KismetSystemLibrary.h"

bool UCC_RangeBase::ActivateCardComponent(AActor* owningActor, AActor* objectCausingHit,  AActor* hitActor, FVector impactPoint)
{
	if (IsValid(projectileType))
	{
		SetLaunchInfo(owningActor);
		SpawnProjectile(owningActor);
		LaunchProjectile(owningActor);
	}
	return false;
}

void UCC_RangeBase::SpawnProjectile(AActor* owningActor)
{
	FTransform spawnTransform;
	if (owningActor != nullptr)
	{

		FQuat rotation = launchDirection.ToOrientationQuat();
		spawnTransform.SetLocation(launchPosition);
		spawnTransform.SetRotation(rotation);
	}

	projectile = GetWorld()->SpawnActor<AProjectileObject>(projectileType, spawnTransform);
	if (projectile != nullptr)
	{
		projectile->RegisterCardOwner(this);
		projectile->RegisterActorOwner(owningActor);	

		TFunction<void(AActor*, FVector)> hitDamageableFunc = ([this](AActor* hitActor, FVector impactPoint) { RaiseDamageableHitEvent(projectile, hitActor, impactPoint); });
		TFunction<void(AActor*, FVector)> hitStaticFunc = ([this](AActor* hitActor, FVector impactPoint) { RaiseStaticHitEvent(projectile, hitActor, impactPoint); });
		projectile->SetHitDamageableResponse(hitDamageableFunc);
		projectile->SetHitStaticResponse(hitStaticFunc);

		if (hasGravitationalInfluence) projectile->ForceProjectileMovement(velocityForGravityMovement);
	}
}

void UCC_RangeBase::LaunchProjectile(AActor* owningActor, bool isFunctional)
{
	if (!projectile || !IsValid(owningActor)) return;
	projectile->Launch(launchPosition, launchDirection, isFunctional);
}

void UCC_RangeBase::SetLaunchInfo(AActor* owningActor)
{
	UDamageManager* damageManager = Cast<UDamageManager>(owningActor->GetComponentByClass(UDamageManager::StaticClass()));
	FVector position = (damageManager) ? damageManager->GetProjectileLaunchPosition() : owningActor->GetActorLocation();
	FVector launchDir = (damageManager) ? damageManager->GetLookDirection() : owningActor->GetActorForwardVector();

	if (damageManager && damageManager->GetProjectileOffset() != FVector::ZeroVector)
	{
		FVector stablePos = damageManager->GetNonOffsetProjectileLaunchPosition();
		FHitResult outHit;
		UKismetSystemLibrary::SphereTraceSingle(GetWorld(), stablePos + launchDir * 100, stablePos + (launchDir * lookCalculationDist), lookCalculationRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, {owningActor}, EDrawDebugTrace::None, outHit, true, FLinearColor::Red, FLinearColor::Green, 1.0f);
		if (outHit.bBlockingHit) launchDir = (outHit.ImpactPoint - position).GetSafeNormal();
	}

	if (coneAngleForInaccuracy > 0) launchDir = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(launchDir, coneAngleForInaccuracy);
	launchDirection = launchDir;
	launchPosition = position;
}
