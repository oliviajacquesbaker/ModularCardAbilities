// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CardBase.h"
#include "CC_AttackBase.generated.h"

UCLASS(Blueprintable)
class PROJECTPHEONIX_API UCC_AttackBase : public UCardBase
{
	GENERATED_BODY()
	
public:
	virtual bool ActivateCardComponent(AActor* owningActor, AActor* objectCausingHit = nullptr,  AActor* hitActor = nullptr, FVector impactPoint = FVector::Zero()) override;

private:
	void Attack(AActor* owningActor);

	//How many units forward can this attack reach.
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		float hitRangeForwardReach = 200.0f;

	//How many units up or down can this attack reach.
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		float hitRangeVerticalReach = 100.0f;

	//How many units left or right can this attack reach.
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		float hitRangeSideReach = 100.0f;
};
