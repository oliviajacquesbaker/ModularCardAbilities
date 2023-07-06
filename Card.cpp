// Fill out your copyright notice in the Description page of Project Settings.

#include "Card.h" 	
#include "GameFramework/Actor.h"
#include "../Player/ProjectPheonixCharacter.h"
#include "../Plugins/Wwise/Source/AkAudio/Classes/AkGameplayStatics.h"

UCard::UCard()
{
	PrimaryComponentTick.bCanEverTick = false;

	cardImage = CreateDefaultSubobject<UTexture>(TEXT("Image"));
	fxSpawner = CreateDefaultSubobject<UFXSpawner>(TEXT("FXSpawner"));
}

void UCard::BeginPlay()
{
	Super::BeginPlay();
	EnsureComponentRegistration();
}

void UCard::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld() && useAfterWindupHandle.IsValid()) GetWorld()->GetTimerManager().ClearTimer(useAfterWindupHandle);

	Super::EndPlay(EndPlayReason);
}

bool UCard::Use(AActor* _owningActor, bool VFXEnabled)
{
	owningActor = _owningActor;
	if (VFXEnabled && windUpEffect != nullptr) fxSpawner->SpawnWindUp(owningActor, windUpTime, cardImage, windUpEffect);

	if (windUpTime == 0) BeginCardFunctionality(owningActor, VFXEnabled);
	else {
		useAfterWindupDelegate.BindUFunction(this, "BeginCardFunctionality", owningActor, VFXEnabled);
		GetWorld()->GetTimerManager().SetTimer(useAfterWindupHandle, useAfterWindupDelegate, windUpTime, false);
	}

	if (currentUseCount >= usesBeforeDiscard)
	{
		ResetCard();
		return true;
	}
	return false;
}

bool UCard::UseInstantly(AActor* _owningActor)
{
	float holdWindupTime = windUpTime;
	windUpTime = 0;
	bool result = UCard::Use(_owningActor);
	windUpTime = holdWindupTime;
	return result;
}

void UCard::BroadcastDamageableHitEvent(AActor* objectCausingHit, AActor* hitActor, FVector impactPoint)
{
	for (UCardBase* card : cardComponents)
	{
		if (card != nullptr) card->OnDamageableHit(owningActor, objectCausingHit, hitActor, impactPoint);
	}
}

void UCard::BroadcastStaticHitEvent(AActor* objectCausingHit, AActor* hitActor, FVector impactPoint)
{
	for (UCardBase* card : cardComponents)
	{
		if (card != nullptr) card->OnStaticHit(owningActor, objectCausingHit, hitActor, impactPoint);
	}
}

void UCard::BroadcastMovementEvent()
{
	for (UCardBase* card : cardComponents)
	{
		if (card != nullptr) card->OnMovementEvent(owningActor);
	}
}

void UCard::ChangeUseAmount(int usesToAdd, bool permanent)
{
	if (permanent) usesBeforeDiscard += usesToAdd;
	else currentUseCount -= usesToAdd;
}

void UCard::BeginCardFunctionality(AActor* _owningActor, bool VFXEnabled) {
	UAkGameplayStatics::PostEvent(cardSound, _owningActor, 0, FOnAkPostEventCallback());

	for (UCardBase* card : cardComponents)
	{
		if (card != nullptr) card->PreUse(_owningActor);
	}

	hasPendingActions = false;
	for (UCardBase* card : cardComponents)
	{
		if (card != nullptr)
		{
			bool componentHasPendingAction = card->Use(_owningActor);
			if (componentHasPendingAction) hasPendingActions = true;
		}
		if (VFXEnabled && useEffect != nullptr) fxSpawner->SpawnEffect(_owningActor, useEffect);
	}

	if(!hasPendingActions) ++currentUseCount;
}

void UCard::ResetCard()
{
	currentUseCount = 0;
}

const UTexture* UCard::GetImage() const
{
	return cardImage;
}

const UTexture* UCard::GetUiImage() const
{
	return uiImage;
}

UCardDeck* UCard::GetOwningDeck() const
{
	return owningDeck;
}

const float UCard::GetCoolDownTime() const
{
	return coolDownTime;
}

const float UCard::GetWindUpTime() const
{
	return windUpTime;
}

const UClass* UCard::GetEffectType() const
{
	return effectType;
}

const bool UCard::GetPendingStatus() const
{
	return hasPendingActions;
}

void UCard::RegisterWithDeck(UCardDeck* deck)
{
	owningDeck = deck;
}

void UCard::EnsureComponentRegistration()
{
	for (UCardBase* card : cardComponents)
	{
		if (card != nullptr)
		{
			card->RegisterWithCard(this);
			card->RegisterWithDeck(owningDeck);
		}
	}
}
