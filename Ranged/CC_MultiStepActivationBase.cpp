// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_MultiStepActivationBase.h"
#include "MultiStepActivationObject.h"

bool UCC_MultiStepActivationBase::ActivateCardComponent(AActor* owningActor, AActor* objectCausingHit,  AActor* hitActor, FVector impactPoint)
{
	++currentStep;
	if (currentStep > activationSteps) currentStep = 1;

	if (IsValid(controlledObjectType)) HandleControlledObject(owningActor);

	if (currentStep < activationSteps) return true;
	else return false;
}

void UCC_MultiStepActivationBase::HandleControlledObject(AActor* owningActor)
{
	if (currentStep == 1)
	{
		FTransform spawnTransform;
		if(owningActor != nullptr) spawnTransform.SetLocation(owningActor->GetActorLocation());

		controlledObject = GetWorld()->SpawnActor<AMultiStepActivationObject>(controlledObjectType, spawnTransform);
		if (controlledObject != nullptr) controlledObject->RegisterCardOwner(this);
	}

	if (controlledObject != nullptr) controlledObject->AdvanceActivationProcess(owningActor);
}

void UCC_MultiStepActivationBase::ForceAdvancement()
{
	if (owningDeck != nullptr) owningDeck->UseTopCard();
}
