// Fill out your copyright notice in the Description page of Project Settings.

#include "CC_SpawnObject.h"
#include "../../Misc/SpawnableObject.h"
#include "../../Misc/EnviroStatusObstacle.h"
#include "../../Misc/Trap.h"

bool UCC_SpawnObject::ActivateCardComponent(AActor* owningActor, AActor* objectCausingHit, AActor* hitActor, FVector impactPoint)
{
	if (IsValid(objectType)) SpawnObject(owningActor, impactPoint);
	return false;
}

void UCC_SpawnObject::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (FTimerHandle& handle : spawnedDestructionTimers)
	{
		if(IsValid(GetWorld()) && handle.IsValid()) GetWorld()->GetTimerManager().ClearTimer(handle);
	}

	Super::EndPlay(EndPlayReason);
}

void UCC_SpawnObject::SpawnObject(AActor* owningActor, FVector impactPoint)
{
	FTransform spawnTransform;
	if (owningActor != nullptr)
	{
		bool spawnedOnImpactPoint = ((activateOnDamageHitsFromOtherComponents || activateOnStaticHitsFromOtherComponents) && impactPoint != FVector::ZeroVector);		
		FQuat rotation = (inheritOwningActorRotation) ? owningActor->GetActorRotation().Quaternion() : FRotator::ZeroRotator.Quaternion();
		FVector position =  spawnedOnImpactPoint ? impactPoint : owningActor->GetActorLocation();

		if (!allowedToFloat && GetWorld()) {
			FHitResult hitResult;
			GetWorld()->LineTraceSingleByChannel(hitResult, position, position - FVector(0, 0, floorTraceDistance), ECC_Visibility);
			position = hitResult.ImpactPoint;
		}

		if (spawnedOnImpactPoint) position += spawnOffset;
		else 
		{
			position += spawnOffset.X * owningActor->GetActorForwardVector();
			position += spawnOffset.Y * owningActor->GetActorRightVector();
			position += spawnOffset.Z * owningActor->GetActorUpVector();
		}		

		spawnTransform.SetLocation(position);
		spawnTransform.SetRotation(rotation);
	}

	AActor* spawnedObject = GetWorld()->SpawnActor<AActor>(objectType, spawnTransform);
	if (IsValid(spawnedObject))
	{
		if (characterSpawningThisIsImmune) AttemptToAddImmunity(owningActor, spawnedObject);
		if (allowedTimeAlive > 0)
		{
			FTimerHandle handle;
			FTimerDelegate del;
			del.BindUFunction(this, "CleanUpObject", spawnedObject);
			{
				if (IsValid(GetWorld())) GetWorld()->GetTimerManager().SetTimer(handle, del, allowedTimeAlive, false);
				spawnedDestructionTimers.Add(handle);
			}
		}

		AttemptToActivate(spawnedObject);
	}
}

void UCC_SpawnObject::AttemptToActivate(AActor* spawnedObject)
{
	if (!IsValid(spawnedObject)) return;

	AEnviroStatusObstacle* enviro = Cast<AEnviroStatusObstacle>(spawnedObject);
	if (IsValid(enviro)) enviro->Activate();

	ATrap* trap = Cast<ATrap>(spawnedObject);
	if (IsValid(trap)) trap->Activate();
}

void UCC_SpawnObject::AttemptToAddImmunity(AActor* owningActor, AActor* spawnedObject)
{
	if (!IsValid(spawnedObject)) return;

	AEnviroStatusObstacle* enviro = Cast<AEnviroStatusObstacle>(spawnedObject);
	if (IsValid(enviro)) enviro->GiveActorImmunity(owningActor);

	ATrap* trap = Cast<ATrap>(spawnedObject);
	if (IsValid(trap)) trap->GiveActorImmunity(owningActor);
}

void UCC_SpawnObject::CleanUpObject(AActor* spawnedObject)
{
	if (IsValid(spawnedObject)) {
		ASpawnableObject* spawnHandle = Cast<ASpawnableObject>(spawnedObject);
		if (spawnHandle) spawnHandle->Cleanup();
		spawnedObject->Destroy();
	}
}
