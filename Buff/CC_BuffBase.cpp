// Fill out your copyright notice in the Description page of Project Settings.

#include "CC_BuffBase.h"
#include "../../Status/StatusManager.h"

bool UCC_BuffBase::ActivateCardComponent(AActor* owningActor, AActor*,  AActor* hitActor, FVector)
{
	if (applyStatusToSelf) AddStatusToActor(owningActor, hitActor);
	else if (hitActor != nullptr) AddStatusToActor(hitActor, owningActor);

	return false;
}

void UCC_BuffBase::AddStatusToActor(AActor* affectedActor, AActor* other)
{
	if (!IsValid(affectedActor)) return;

	UStatusManager* statusManager = Cast<UStatusManager>((affectedActor->GetComponentByClass(UStatusManager::StaticClass())));
	if (!IsValid(statusManager)) return;

	if (checkConditionAgainstThis && !(statusManager->CheckActivationAllowedOnSelf(applicationConditions, healthConditionalThreshold))) return;
	else if (!checkConditionAgainstThis)
	{
		if (!IsValid(other)) return;
		UStatusManager* otherStatusManager = Cast<UStatusManager>((other->GetComponentByClass(UStatusManager::StaticClass())));
		if (!IsValid(otherStatusManager) || !(otherStatusManager->CheckActivationAllowedOnSelf(applicationConditions, healthConditionalThreshold))) return;
	}

	if (presetStatus != EStatus::SE_None) appliedStatusID = statusManager->ApplyPresetStatus(presetStatus, presetStatusApplicationTime);
	else appliedStatusID = statusManager->ApplyStatus(customStatus, customStatus.statusLingerTime);
}
