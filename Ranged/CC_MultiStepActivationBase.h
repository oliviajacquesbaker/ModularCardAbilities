// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CardBase.h"
#include "../CardDeck.h"
#include "CC_MultiStepActivationBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTPHEONIX_API UCC_MultiStepActivationBase : public UCardBase
{
	GENERATED_BODY()

public:
	virtual bool ActivateCardComponent(AActor* owningActor, AActor* objectCausingHit = nullptr,  AActor* hitActor = nullptr, FVector impactPoint = FVector::Zero()) override;

	UFUNCTION(BlueprintCallable, Category = "Card Behavior")
		void ForceAdvancement();
	
protected:
	void HandleControlledObject(AActor* owningActor);

	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		int activationSteps = 2;

	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		TSubclassOf<class AMultiStepActivationObject> controlledObjectType;

	UPROPERTY(VisibleAnywhere, Category = "Card Behavior")
		AMultiStepActivationObject* controlledObject;

	int currentStep = 0;
};
