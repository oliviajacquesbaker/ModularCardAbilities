// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CardBase.h"
#include "../../Status/StatusPreset_Base.h"
// TODO: Can this one be removed? I deleted it and it still built
#include "UObject/UObjectGlobals.h"
#include "CC_BuffBase.generated.h"

UCLASS(Blueprintable)
class PROJECTPHEONIX_API UCC_BuffBase : public UCardBase
{
	GENERATED_BODY()

public:
	virtual bool ActivateCardComponent(AActor* owningActor, AActor* objectCausingHit = nullptr,  AActor* hitActor = nullptr, FVector impactPoint = FVector::Zero()) override;

protected:
	// TODO: "other" is not very descriptive, maybe rename it
	void AddStatusToActor(AActor* owningActor, AActor* other);

	//if true, apply status to character that owns the card. otherwise, apply it to the "other" (i.e. something that's been identified as hit in an event)
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		bool applyStatusToSelf = true;

	//selecting anything OTHER than "Basic Nothing" status here will mean the applied status will be based on the presets of the affected status manager
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		TEnumAsByte<EStatus> presetStatus;
	//if using a preset time, set this to override how long that preset lasts for
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		float presetStatusApplicationTime = 7.0f;
	//if the preset status is set to "Basic Nothing", then it will apply this custom status instead.
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		FStatusPreset_Base customStatus;

	//all conditions in this list must be met for the status to be applied
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		TArray<TEnumAsByte<EApplicationCondition>> applicationConditions = { };
	//check the conditions against the character getting the status applied to them, otherwise to the 'other'
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		bool checkConditionAgainstThis = true;
	//if you have a health based condition in the action conditions list, set this variable as the reference. should be considered as a percent (0.5 = half health)
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		float healthConditionalThreshold = 0.0f;

	uint32 appliedStatusID;
};
