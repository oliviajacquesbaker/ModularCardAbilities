// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CC_RangeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectileObject.h"
#include "Math/UnrealMathUtility.h"
#include "CC_RangedHitscan.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPHEONIX_API UCC_RangedHitscan : public UCC_RangeBase
{
	GENERATED_BODY()
	
public:
	virtual bool ActivateCardComponent(AActor* owningActor, AActor* objectCausingHit = nullptr,  AActor* hitActor = nullptr, FVector impactPoint = FVector::Zero()) override;

protected:
	void HitscanDamage(AActor* owningActor);

	//The Range where the damage has fully fallen off
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		float maxFallOffDistance = 500.0f;
	//The maximum distance of the attack
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		float maxShootDistance = 1000.0f;
	//the number of times the full distance is split, each time the cast is steadily spreading (growing)
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		int spreadSegments = 10;
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		float minTraceRadius = 5.0f;
	//maxTraceRadius is the radius on the last trace segment (furthest away)
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		float maxTraceRadius = 50.0f;
	//if true, this card will override the damage variable in the card parent, though that won't do anything unless a damage component fetches that variable
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		bool hasFalloffDamage = true;
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		bool isPiercing = false;
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		float maxDamage = 10.0f;
	//minDamage would be what's applied when at max spread (fall off)
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		float minDamage = 1.0f;
};
