// Fill out your copyright notice in the Description page of Project Settings.


#include "CardBase.h"
#include "CardDeck.h"

UCardBase::UCardBase()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCardBase::RegisterWithCard(UCard* card)
{
	owningCard = card;
}

void UCardBase::RegisterWithDeck(UCardDeck* deck)
{
	owningDeck = deck;
}

void UCardBase::BeginPlay()
{
	Super::BeginPlay();
}

void UCardBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld() && windupTimer.IsValid()) GetWorld()->GetTimerManager().ClearTimer(windupTimer);

	Super::EndPlay(EndPlayReason);
}

bool UCardBase::ActivateCardComponent(AActor* owningActor, AActor* objectCausingHit,  AActor* hitActor, FVector impactPoint)
{
	return false;
}

void UCardBase::DummyActivation(AActor* owningActor, AActor* objectCausingHit, AActor* hitActor, FVector impactPoint)
{
	ActivateCardComponent(owningActor, objectCausingHit, hitActor, impactPoint);
}

void UCardBase::TimerForComponentActivation(AActor* owningActor, AActor* objectCausingHit, AActor* hitActor, FVector impactPoint)
{
	FTimerDelegate del;
	del.BindUFunction(this, "DummyActivation", owningActor, objectCausingHit, hitActor, impactPoint);
	GetWorld()->GetTimerManager().SetTimer(windupTimer, del, windUpTime, false);
}

void UCardBase::PreUse(AActor* owningActor)
{
	if (activateDuringPreUseCycle)
	{
		if (windUpTime <= 0) ActivateCardComponent(owningActor);
		else TimerForComponentActivation(owningActor);
	}
}

bool UCardBase::Use(AActor* owningActor)
{	
	if (activateDuringMainUseCycle)
	{
		if (windUpTime <= 0) return ActivateCardComponent(owningActor);
		else TimerForComponentActivation(owningActor);
	}
	return false;
}

void UCardBase::OnDamageableHit(AActor* owningActor, AActor* objectCausingHit, AActor* hitActor, FVector impactPoint)
{
	if (activateOnDamageHitsFromOtherComponents)
	{
		if (windUpTime <= 0) ActivateCardComponent(owningActor, objectCausingHit, hitActor, impactPoint);
		else TimerForComponentActivation(owningActor, objectCausingHit, hitActor, impactPoint);
	}
}

void UCardBase::OnStaticHit(AActor* owningActor, AActor* objectCausingHit, AActor* hitActor, FVector impactPoint)
{
	if (activateOnStaticHitsFromOtherComponents) 
	{
		if (windUpTime <= 0) ActivateCardComponent(owningActor, objectCausingHit, hitActor, impactPoint);
		else TimerForComponentActivation(owningActor, objectCausingHit, hitActor, impactPoint);
	}
}

void UCardBase::OnMovementEvent(AActor* owningActor)
{
	if (activateOnMovementEventsFromOtherComponents)
	{
		if (windUpTime <= 0) ActivateCardComponent(owningActor);
		else TimerForComponentActivation(owningActor);
	}
}

void UCardBase::RaiseDamageableHitEvent(AActor* objectCausingHit, AActor* hitActor, FVector impactPoint)
{
	if (!canRaiseDamageableHitEvents || !IsValid(owningCard)) return;
	owningCard->BroadcastDamageableHitEvent(objectCausingHit, hitActor, impactPoint);
}

void UCardBase::RaiseStaticHitEvent(AActor* objectCausingHit, AActor* hitActor, FVector impactPoint)
{
	if (!canRaiseStaticHitEvents || !IsValid(owningCard)) return;
	owningCard->BroadcastStaticHitEvent(objectCausingHit, hitActor, impactPoint);
}

void UCardBase::RaiseMovementEvent()
{
	if (!canRaiseMovementEvents || !IsValid(owningCard)) return;
	owningCard->BroadcastMovementEvent();
}

void UCardBase::AdjustCooldown(float cooldownMultiplier)
{
	if (IsValid(owningDeck)) owningDeck->AdjustCurrentCoolDown(cooldownMultiplier);
}

void UCardBase::RespondToStatusSuccess(bool statusWasSuccessful)
{
}

void UCardBase::OverrideCardDamage(int damage)
{
	if (owningCard) owningCard->OverrideDamage(damage);
}

int UCardBase::GetOverridenCardDamage()
{
	if (owningCard) return owningCard->GetOverridenDamage();
	return 0;
}

