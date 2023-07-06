// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Ranged/CC_MultiStepActivationBase.h"
#include "MultiStepActivationObject.generated.h"

UCLASS()
class PROJECTPHEONIX_API AMultiStepActivationObject : public AActor
{
	GENERATED_BODY()
	
public:	
	AMultiStepActivationObject();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Object Activation")
		virtual void AdvanceActivationProcess(AActor* owningActor);
	UFUNCTION(BlueprintCallable, Category = "Object Activation")
		virtual void RegisterCardOwner(UCC_MultiStepActivationBase* owningCard);

protected:
	virtual void BeginPlay() override;
	virtual void ForceAdvancement();

	UPROPERTY(VisibleAnywhere, Category = "Object Activation")
		int activationSteps = 2;

	int currentStep = 0;
	UCC_MultiStepActivationBase* owningCard;
};
