// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CardBase.h"
#include "CC_SpawnObject.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPHEONIX_API UCC_SpawnObject : public UCardBase
{
	GENERATED_BODY()

public:
	virtual bool ActivateCardComponent(AActor* owningActor, AActor* objectCausingHit = nullptr,  AActor* hitActor = nullptr, FVector impactPoint = FVector::Zero()) override;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void SpawnObject(AActor* owningActor, FVector impactPoint);
	void AttemptToActivate(AActor* spawnedObject);
	void AttemptToAddImmunity(AActor* owningActor, AActor* spawnedObject);

	UFUNCTION(BlueprintCallable)
		void CleanUpObject(AActor* spawnedObject);

	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		TSubclassOf<class AActor> objectType;
	//keep set == -1 for it to live indefinitely
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		float allowedTimeAlive = -1.0f;	
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		FVector spawnOffset = FVector(0,0,0);
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		bool inheritOwningActorRotation = true;
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		bool allowedToFloat = true;
	//for traps and enviro obstacles, is the character spawning the object immune to it?
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		bool characterSpawningThisIsImmune = false;

	float floorTraceDistance = 100000.0f;
	TArray<FTimerHandle> spawnedDestructionTimers;
};
