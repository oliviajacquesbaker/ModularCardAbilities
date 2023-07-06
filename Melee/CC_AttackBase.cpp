// Fill out your copyright notice in the Description page of Project Settings.

#include "CC_AttackBase.h"
#include "Camera/CameraComponent.h"
#include "../../Status/DamageManagers/DamageManager.h"
#include "Kismet/KismetSystemLibrary.h"

bool UCC_AttackBase::ActivateCardComponent(AActor* owningActor, AActor* objectCausingHit,  AActor* hitActor, FVector impactPoint)
{
	Attack(owningActor);
	return false;
}

void UCC_AttackBase::Attack(AActor* owningActor)
{
	if (owningActor == nullptr) return;

	TArray<AActor*> objectsToIgnore;
	objectsToIgnore.Add(owningActor);

	TArray<FHitResult> hitActors;
	UCameraComponent* characterCamera = Cast<UCameraComponent>(owningActor->GetComponentByClass(UCameraComponent::StaticClass()));
	FVector attackOrigin = owningActor->GetActorLocation();
	attackOrigin += (characterCamera ? characterCamera->GetForwardVector() * hitRangeForwardReach : owningActor->GetActorForwardVector() * hitRangeForwardReach);
	FRotator meleeOrientation = (characterCamera ? characterCamera->GetComponentRotation() : owningActor->GetActorRotation());
	UKismetSystemLibrary::BoxTraceMulti(GetWorld(), attackOrigin, attackOrigin, FVector(hitRangeForwardReach, hitRangeSideReach, hitRangeVerticalReach), meleeOrientation, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2), false, objectsToIgnore, EDrawDebugTrace::ForDuration, hitActors, true, FLinearColor::Red, FLinearColor::Green, 2.0f);

	for (FHitResult hitResult : hitActors)
	{
		if (!hitResult.GetActor()) continue;

		UDamageManager* damageManager = Cast<UDamageManager>(hitResult.GetActor()->GetComponentByClass(UDamageManager::StaticClass()));
		if (damageManager) RaiseDamageableHitEvent(owningActor, hitResult.GetActor(), hitResult.ImpactPoint);
		else RaiseStaticHitEvent(owningActor, hitResult.GetActor(), hitResult.ImpactPoint);
	}
}