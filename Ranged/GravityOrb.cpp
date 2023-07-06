// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityOrb.h"

AGravityOrb::AGravityOrb()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGravityOrb::BeginPlay()
{
	Super::BeginPlay();	
}

void AGravityOrb::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld() && destructionTimerHandle.IsValid()) GetWorld()->GetTimerManager().ClearTimer(destructionTimerHandle);

	Super::EndPlay(EndPlayReason);
}

void AGravityOrb::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (currentStep == 1) Move(deltaTime);
	else if (currentStep > 1) Pull(deltaTime);
}

void AGravityOrb::AdvanceActivationProcess(AActor* owningActor)
{
	Super::AdvanceActivationProcess(owningActor);

	if (currentStep == 1)
	{
		ACharacter* characterOwner = Cast<ACharacter>(owningActor);
		if (characterOwner == nullptr) return;

		UCameraComponent* characterCamera = Cast<UCameraComponent>(characterOwner->GetComponentByClass(UCameraComponent::StaticClass()));
		if (characterCamera != nullptr) Launch(characterOwner->GetActorLocation(), characterCamera->GetForwardVector());
		else Launch(characterOwner->GetActorLocation(), characterOwner->GetActorForwardVector());
	}
	else BeginPull();
}

void AGravityOrb::Launch(FVector position, FVector direction)
{
	SetActorLocation(position + direction);
	destinationLocation = position + (direction * maxMovementDistance);

	allowedMoveTime = FVector::Distance(GetActorLocation(), destinationLocation);
	if (movementSpeed > 0) allowedMoveTime /= movementSpeed;
}

void AGravityOrb::BeginPull()
{
	BPFXSpawnEvent();
	FVector attackOrigin = GetActorLocation();
	TArray<FHitResult> hitActors;
	TArray<AActor*> objectsToIgnore;
	objectsToIgnore.Add(this);

	bool didHit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), attackOrigin, attackOrigin, captureRadius, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2), false, objectsToIgnore, EDrawDebugTrace::ForDuration, hitActors, true, FLinearColor::Red, FLinearColor::Green, 0.1f);
	for (FHitResult hitResult : hitActors)
	{
		UCharacterMovementComponent* movementComponent = Cast<UCharacterMovementComponent>(hitResult.GetActor()->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
		if (movementComponent != nullptr)
		{
			movementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
			caughtObjects.Add(hitResult.GetActor());
		}
	}

	GetWorld()->GetTimerManager().SetTimer(destructionTimerHandle, this, &AGravityOrb::Cleanup, gravitationalPullTime, false);
}

void AGravityOrb::Move(float deltaTime)
{
	elapsedMoveTime += deltaTime;

	FVector newPosition = FMath::Lerp(GetActorLocation(), destinationLocation, elapsedMoveTime / allowedMoveTime);
	SetActorLocation(newPosition, true);

	if (FVector::Distance(destinationLocation, newPosition) < acceptanceRadius) ForceAdvancement();
}

void AGravityOrb::Pull(float deltaTime)
{
	for (AActor* object : caughtObjects)
	{
		if (IsValid(object))
		{
			FVector lookDirection = GetActorLocation() - object->GetActorLocation();
			lookDirection.Normalize();

			FVector newPosition = object->GetActorLocation() + (lookDirection * gravitationalForce);
			object->SetActorLocation(newPosition, true);

			if (FVector::Distance(object->GetActorLocation(), newPosition) > errorAcceptance)
			{
				lookDirection.Z = 0;
				newPosition = object->GetActorLocation() + (lookDirection * gravitationalForce);
				object->SetActorLocation(newPosition, true);
			}			
		}
	}
	SetActorScale3D(GetActorScale() + GetActorScale() * scaleSpeed);
}

void AGravityOrb::Cleanup()
{
	for (AActor* object : caughtObjects)
	{
		if (IsValid(object))
		{
			UCharacterMovementComponent* movementComponent = Cast<UCharacterMovementComponent>(object->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
			if (movementComponent != nullptr) movementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
		}
	}

	CleanupBP();
}

bool AGravityOrb::CleanupBP_Implementation()
{
	Destroy(); //ML: Allow me to control how FX spawn
	return true;
}

bool AGravityOrb::BPFXSpawnEvent_Implementation()
{
	return true;
}


