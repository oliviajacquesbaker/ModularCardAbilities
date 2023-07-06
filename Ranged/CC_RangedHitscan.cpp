// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_RangedHitscan.h"

bool UCC_RangedHitscan::ActivateCardComponent(AActor* owningActor, AActor* objectCausingHit,  AActor* hitActor, FVector impactPoint)
{
	if (IsValid(projectileType))
	{
		SetLaunchInfo(owningActor);
		SpawnProjectile(owningActor);
		HitscanDamage(owningActor);
		LaunchProjectile(owningActor, false);
	}
	else HitscanDamage(owningActor);
	return false;
}

void UCC_RangedHitscan::HitscanDamage(AActor* owningActor)
{
	ACharacter* characterOwner = Cast<ACharacter>(owningActor);
	if (characterOwner == nullptr) return;

	float traceRadiusIncrease = (maxTraceRadius - minTraceRadius) / (spreadSegments - 1);
	float traceSegmentDistance = maxShootDistance / spreadSegments;
	FHitResult outHit;

	for (int i = 0; i < spreadSegments; ++i) {
		UKismetSystemLibrary::SphereTraceSingle(GetWorld(), launchPosition + (launchDirection * (traceSegmentDistance * i + minTraceRadius)), launchPosition + (launchDirection * (traceSegmentDistance * (i + 1)  + minTraceRadius)), minTraceRadius + traceRadiusIncrease * i, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, { owningActor, projectile }, EDrawDebugTrace::ForDuration, outHit, true, FLinearColor::Red, FLinearColor::Green, 2.0f);
		if (outHit.bBlockingHit && outHit.GetActor())
		{
			float distance = (outHit.GetActor()->GetActorLocation() - launchPosition).Length();
			if (projectile) {
				projectile->OverrideMaxDistance(distance);
				projectile->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(launchPosition + (launchDirection * traceSegmentDistance * i), launchPosition + (launchDirection * traceSegmentDistance * (i + 1))));
			}

			// Will only matter if a damage component on this card opts to use this overridden amount
			if(hasFalloffDamage) {
				float functionPower = 1.0f / 3.0f;
				float averageDamage = (maxDamage + minDamage) / 2;
				float falloffCenter = maxFallOffDistance / 2;
				float scalingFactor = ((maxDamage - minDamage)/2) / FMath::Pow(falloffCenter, functionPower);
				float offset = scalingFactor * FMath::Pow(FMath::Abs(distance - falloffCenter), functionPower);
				float damage = FMath::RoundToInt(averageDamage + offset * FMath::Sign(falloffCenter - distance));
				OverrideCardDamage(FMath::Max(damage, minDamage));
			}

			UDamageManager* healthOfHitActor = Cast<UDamageManager>(outHit.GetActor()->GetComponentByClass(UDamageManager::StaticClass()));
			if (healthOfHitActor) RaiseDamageableHitEvent(owningActor, outHit.GetActor(), outHit.ImpactPoint);
			else RaiseStaticHitEvent(owningActor, outHit.GetActor(), outHit.ImpactPoint);
			if(!isPiercing) return;
		}
	}
	return;
}
