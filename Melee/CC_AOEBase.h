// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CardBase.h"
#include "../../Status/ApplicationConditionEnum.h"
#include "../../Status/DamageManagers/DamageManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CC_AOEBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPHEONIX_API UCC_AOEBase : public UCardBase
{
	GENERATED_BODY()
	
public:
	virtual bool ActivateCardComponent(AActor* owningActor, AActor* objectCausingHit = nullptr,  AActor* hitActor = nullptr, FVector impactPoint = FVector::Zero()) override;

protected:
	UFUNCTION(BlueprintCallable)
		void ActivateAOE(AActor* owningActor);

	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		float AOERadius = 500.0f;
};
