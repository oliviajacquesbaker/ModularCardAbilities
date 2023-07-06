// Fill out your copyright notice in the Description page of Project Settings.

#include "CC_Damage.h"
#include "../../Status/DamageManagers/DamageManager.h"
#include "../../Status/StatusManager.h"

bool UCC_Damage::ActivateCardComponent(AActor* owningActor, AActor* objectCausingHit, AActor* hitActor, FVector impactPoint)
{
	if (!IsValid(hitActor)) return false; //this component only worth something if activated on a damageable hit event

	UDamageManager* damageManager = Cast<UDamageManager>(hitActor->GetComponentByClass(UDamageManager::StaticClass()));
	UStatusManager* statusManager = Cast<UStatusManager>((hitActor->GetComponentByClass(UStatusManager::StaticClass())));
	if (!IsValid(damageManager) || !IsValid(statusManager)) return false;

	if (!(statusManager->CheckActivationAllowedOnSelf(applicationConditions, healthConditionalThreshold))) return false;

	float damage = GetDamageAmount(owningActor);
	if (damage > 0) damageManager->Damage(owningActor, objectCausingHit, damage, damageType);

	return false;
}

float UCC_Damage::GetDamageAmount(AActor* owningActor)
{
	if (damageDoneOnHitCondition == EConditionalDamageAmount::CD_None) return damageAmount;
	else if (damageDoneOnHitCondition == EConditionalDamageAmount::CD_Override) return GetOverridenCardDamage();
	else if (damageDoneOnHitCondition == EConditionalDamageAmount::CD_Block)
	{
		if (!owningActor) return damageAmount;

		UStatusManager* statusManager = Cast<UStatusManager>(owningActor->GetComponentByClass(UStatusManager::StaticClass()));
		if (statusManager) return statusManager->GetCurrentBlock();
	}

	return damageAmount;
}