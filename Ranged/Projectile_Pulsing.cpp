// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_Pulsing.h"


void AProjectile_Pulsing::BeginPlay()
{
	Super::BeginPlay();

	if (pulseScaleTime > 0) scaleRate = pulseFinalScale / pulseScaleTime;
	if (GetWorld()) GetWorld()->GetTimerManager().SetTimer(pulseTimerHandle, this, &AProjectile_Pulsing::Pulse, pulseInterval, true);
}

void AProjectile_Pulsing::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (FTimerHandle& handle : spawnedDestructionTimers)
	{
		if (IsValid(GetWorld()) && handle.IsValid()) GetWorld()->GetTimerManager().ClearTimer(handle);
	}

	Super::EndPlay(EndPlayReason);
}

void AProjectile_Pulsing::Cleanup()
{
	if (GetWorld()) GetWorld()->GetTimerManager().ClearTimer(pulseTimerHandle);
	for (AActor* pulse : activePulseObjects) {
		if (pulse) pulse->Destroy();
	}
	Super::Cleanup();
}

void AProjectile_Pulsing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (AActor* pulse : activePulseObjects) {
		if(pulse) pulse->SetActorScale3D(pulse->GetActorScale3D() + (DeltaTime * scaleRate));
	}
}

void AProjectile_Pulsing::Pulse()
{
	if (!GetWorld() || pulseScaleTime <= 0 || !IsValid(pulseType)) return;

	AActor* sphereActor = GetWorld()->SpawnActor<AActor>(pulseType);
	if (sphereActor != nullptr)
	{
		activePulseObjects.Add(sphereActor);
		sphereActor->SetActorLocation(GetActorLocation());
		BPFXSpawnEvent();

		USphereComponent* pulseSphere = Cast<USphereComponent>(sphereActor->GetComponentByClass(USphereComponent::StaticClass()));
		if (pulseSphere) pulseSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile_Pulsing::PulseOverlap);

		FTimerHandle handle;
		FTimerDelegate del;
		del.BindUFunction(this, "ForceDestroy", sphereActor);
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(handle, del, pulseScaleTime, false);
			spawnedDestructionTimers.Add(handle);
		}
	}
}

void AProjectile_Pulsing::PulseOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (otherActor == nullptr) return;
	if (!canHurtOwningActor && otherActor == owningActor) return;

	UDamageManager* healthOfHitActor = Cast<UDamageManager>(otherActor->GetComponentByClass(UDamageManager::StaticClass()));
	if (healthOfHitActor && hitDamageResponse.operator bool()) hitDamageResponse(otherActor, sweepResult.ImpactPoint);
	else if (!healthOfHitActor && hitStaticResponse.operator bool()) hitStaticResponse(otherActor, sweepResult.ImpactPoint);
}

void AProjectile_Pulsing::ForceDestroy(AActor* toDestroy)
{
	activePulseObjects.Remove(toDestroy);
	if (IsValid(toDestroy)) toDestroy->Destroy();
}
bool AProjectile_Pulsing::BPFXSpawnEvent_Implementation()
{
	return true;
}