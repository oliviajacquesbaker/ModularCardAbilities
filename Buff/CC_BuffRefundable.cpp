// Fill out your copyright notice in the Description page of Project Settings.

#include "CC_BuffRefundable.h"
#include "../../Status/StatusManager.h"

bool UCC_BuffRefundable::ActivateCardComponent(AActor* owningActor, AActor*,  AActor* hitActor, FVector)
{
	if (applyStatusToSelf) AddStatusToActor(owningActor, hitActor);
	else if (hitActor != nullptr) AddStatusToActor(hitActor, owningActor);

	UStatusManager* statusManager = Cast<UStatusManager>((owningActor->GetComponentByClass(UStatusManager::StaticClass())));
	if (appliedStatusID <= 0 || !IsValid(statusManager)) return false;
	for (EStatusSuccessEvents event : eventsToSubscribeTo)
	{
		statusManager->SubscribeCardToStatusEvent(this, event, appliedStatusID);
	}

	return false;
}

void UCC_BuffRefundable::RespondToStatusSuccess(bool statusWasSuccessful)
{
	if (statusWasSuccessful) AdjustCooldown(coolDownMultiplier);
}
