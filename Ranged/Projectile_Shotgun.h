// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileObject.h"
#include "Components/StaticMeshComponent.h"
#include "Projectile_Shotgun.generated.h"

/**
 *  THIS CLASS CURRENTLY MISSING DAMAGE FALL OFF - it doesnt fit here anymore with the system rework, and we weren't actually using this card
 *  for functionality, so i've left it out. if it becomes necessary, i'll create a way to report falloff adjustments to the damage component later.
 */
UCLASS()
class PROJECTPHEONIX_API AProjectile_Shotgun : public AProjectileObject
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	void Spread();

	UPROPERTY(EditAnywhere, Category = "Projectile Options")
		float spreadRate = 0.01f;
	UPROPERTY(EditAnywhere, Category = "Projectile Options")
		float maxSpread = 10.0f;
	UPROPERTY(EditAnywhere, Category = "Projectile Options")
		float speedDecay = 0.01f;
	UPROPERTY(EditAnywhere, Category = "Projectile Options")
		float minSpeedPercent = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Projectile Options")
		TArray<UStaticMeshComponent*> pelletMeshes;

	float minSpeed;
	float minDamage;
	float currentSpread = 0.0f;
};
