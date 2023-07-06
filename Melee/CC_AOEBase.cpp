// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_AOEBase.h"

bool UCC_AOEBase::ActivateCardComponent(AActor* owningActor, AActor* objectCausingHit,  AActor* hitActor, FVector impactPoint)
{
	ActivateAOE(owningActor);
	return false;
}

void UCC_AOEBase::ActivateAOE(AActor* owningActor)
{
	if (!owningActor) return;

	TArray<AActor*> objectsToIgnore;
	objectsToIgnore.Add(owningActor);

	TArray<FHitResult> hitActors;
	FVector origin = owningActor->GetActorLocation();
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), origin, origin, AOERadius, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2), false, objectsToIgnore, EDrawDebugTrace::ForDuration, hitActors, true, FLinearColor::Red, FLinearColor::Green, 2.0f);

	for (FHitResult hitResult : hitActors)
	{
		if (hitResult.GetActor() == nullptr) continue;
		if (owningActor && hitResult.GetActor()->GetClass() == owningActor->GetClass()) continue;

		UDamageManager* damageManager = Cast<UDamageManager>(hitResult.GetActor()->GetComponentByClass(UDamageManager::StaticClass()));
		if (damageManager) RaiseDamageableHitEvent(owningActor, hitResult.GetActor(), hitResult.ImpactPoint);
		else RaiseStaticHitEvent(owningActor, hitResult.GetActor(), hitResult.ImpactPoint);
	}
}
