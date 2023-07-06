// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CardBase.h"
#include "../../Status/StatusEnum.h"
#include "../../Status/ApplicationConditionEnum.h"
#include "CC_Damage.generated.h"

UENUM(BlueprintType)
enum EConditionalDamageAmount : int
{
	CD_None				UMETA(DisplayName = "No condition, damage explicit."),
	CD_Block			UMETA(DisplayName = "Equal to user's current block amount."),
	CD_Override			UMETA(DisplayName = "Depend on another component to override amount.")
};

UCLASS()
class PROJECTPHEONIX_API UCC_Damage : public UCardBase
{
	GENERATED_BODY()
	
public:
	virtual bool ActivateCardComponent(AActor* owningActor, AActor* objectCausingHit = nullptr,  AActor* hitActor = nullptr, FVector impactPoint = FVector::Zero()) override;

protected:
	float GetDamageAmount(AActor* owningActor);

	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		float damageAmount = 0.0f;
	//setting this to anything but "No Condition" will OVERRIDE the above damage amount variable
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		TEnumAsByte<EConditionalDamageAmount> damageDoneOnHitCondition = EConditionalDamageAmount::CD_None;
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		TArray<TEnumAsByte<EStatus>> damageType = { EStatus::SE_None };

	//all conditions in this list must be met for the damage to be applied - conditions will be checked against the character getting the damage applied to them.
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		TArray<TEnumAsByte<EApplicationCondition>> applicationConditions = { };
	//if you have a health based condition in the action conditions list, set this variable as the reference. should be considered as a percent (0.5 = half health)
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		float healthConditionalThreshold = 0.0f;
};
