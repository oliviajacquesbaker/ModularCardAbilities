// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_Shotgun.h"

void AProjectile_Shotgun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Spread();
}

void AProjectile_Shotgun::BeginPlay()
{
	Super::BeginPlay();
	if (collider == nullptr) return;

	minSpeed = minSpeedPercent * movementSpeed;

	TArray<UActorComponent*> meshes;
	GetComponents(UStaticMeshComponent::StaticClass(), meshes);
	for (UActorComponent* item : meshes)
	{
		UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(item);
		if (mesh != nullptr)
		{
			mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			mesh->SetMobility(EComponentMobility::Movable);
			pelletMeshes.Add(mesh);
		}
	}
}

void AProjectile_Shotgun::Spread()
{
	if(movementSpeed > minSpeed) movementSpeed -= speedDecay;

	if (currentSpread >= maxSpread) return;
	for (UStaticMeshComponent* pellet : pelletMeshes)
	{
		pellet->SetRelativeLocation(pellet->GetRelativeLocation() * (1 + spreadRate));
	}

	if (collider != nullptr) collider->SetSphereRadius(collider->GetUnscaledSphereRadius() * (1 + spreadRate));
	currentSpread += spreadRate;
}
