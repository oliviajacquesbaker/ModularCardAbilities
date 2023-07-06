// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CardBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../Status/DamageManagers/DamageManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "CC_RangeBase.generated.h"

UCLASS(Blueprintable)
class PROJECTPHEONIX_API UCC_RangeBase : public UCardBase
{
	GENERATED_BODY()
	
public:
	virtual bool ActivateCardComponent(AActor* owningActor, AActor* objectCausingHit = nullptr,  AActor* hitActor = nullptr, FVector impactPoint = FVector::Zero()) override;

protected:
	void SpawnProjectile(AActor* owningActor);
	void LaunchProjectile(AActor* owningActor, bool isFunctional = true);
	void SetLaunchInfo(AActor* owningActor);

	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		TSubclassOf<class AProjectileObject> projectileType;
	//at 0, this will shoot straight ahead. otherwise it represents the radius of the cone it will choose a random direction to shoot in
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		float coneAngleForInaccuracy = 0.0f;
	//check this box if you want the projectile to fall with gravity like real projectile motion
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		bool hasGravitationalInfluence = false;
	//if you choose to use the projectile motion, what velocity does it start with?
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		float velocityForGravityMovement = 1000.0f;

	UPROPERTY(VisibleAnywhere, Category = "Card Behavior")
		AProjectileObject* projectile;

	FVector launchPosition;
	FVector launchDirection;
	float lookCalculationDist = 100000.0f;
	float lookCalculationRadius = 10.0f;
};
