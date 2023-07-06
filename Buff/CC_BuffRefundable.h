// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CC_BuffBase.h"
#include "CC_BuffRefundable.generated.h"

UCLASS()
class PROJECTPHEONIX_API UCC_BuffRefundable : public UCC_BuffBase
{
	GENERATED_BODY()
	
public:
	virtual bool ActivateCardComponent(AActor* owningActor, AActor* objectCausingHit = nullptr,  AActor* hitActor = nullptr, FVector impactPoint = FVector::Zero()) override;
	virtual void RespondToStatusSuccess(bool statusWasSuccessful) override;

protected:
	//If the card action is successful, how should the cooldown of the deck be manipulated?
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		float coolDownMultiplier = 0.5f;
	//status events that should cause a refund
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		TArray<TEnumAsByte<EStatusSuccessEvents>> eventsToSubscribeTo;
};
