// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiStepActivationObject.h"

AMultiStepActivationObject::AMultiStepActivationObject()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMultiStepActivationObject::BeginPlay()
{
	Super::BeginPlay();	
}

void AMultiStepActivationObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMultiStepActivationObject::AdvanceActivationProcess(AActor* owningActor)
{
	++currentStep;
}

void AMultiStepActivationObject::ForceAdvancement()
{
	if (owningCard != nullptr) owningCard->ForceAdvancement();
}

void AMultiStepActivationObject::RegisterCardOwner(UCC_MultiStepActivationBase* _owningCard)
{
	owningCard = _owningCard;
	if(owningCard != nullptr) SetActorLocation(owningCard->GetOwner()->GetActorLocation());
}
