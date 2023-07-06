// Fill out your copyright notice in the Description page of Project Settings.


#include "CardDeck.h"

UCardDeck::UCardDeck()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCardDeck::BeginPlay()
{
	Super::BeginPlay();
}

void UCardDeck::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld() && coolDownTimer.IsValid()) GetWorld()->GetTimerManager().ClearTimer(coolDownTimer);
	Super::EndPlay(EndPlayReason);
}

void UCardDeck::AddCard(TSubclassOf<UCard> cardType)
{
	if (!IsValid(cardType)) return;
	UCard* card = NewObject<UCard>(this, cardType);
	if (card == nullptr) return;
	card->SetupAttachment(this);
	card->RegisterWithDeck(this);
	card->RegisterComponent();

	deck.Add(card);
}

bool UCardDeck::RemoveCard(UCard* card)
{
	if (card || (deck.Num() > topCard && deck[topCard]->GetPendingStatus()))
	{
		int numRemoved = deck.Remove(card);
		bool success = (numRemoved == 1);
		if (success && topCard >= deck.Num()) Shuffle();
		return success;
	}
	return false;
}

bool UCardDeck::SwapCard(UCard* oldCard, UCard* newCard)
{
	if (!oldCard || !newCard || deck.Num() <= topCard || !deck[topCard] || deck[topCard]->GetPendingStatus()) return false;
	int index = deck.Find(oldCard);
	if (index != INDEX_NONE)
	{
		UCard* card = NewObject<UCard>(this, newCard->GetClass());
		if (card == nullptr) return false;
		card->SetupAttachment(this);
		card->RegisterWithDeck(this);
		card->RegisterComponent();

		deck[index] = card;
		return true;
	}
	return false;
}

bool UCardDeck::UseTopCard()
{
	if (!usable || deck.Num() <= topCard) return false;

	float coolDown = deck[topCard]->GetCoolDownTime();
	bool needsDiscard = deck[topCard]->Use(GetOwner());

	if (needsDiscard)
	{
		DiscardTopCard();
		PutDeckOnCoolDown(coolDown);
		OnTopCardUsed.Broadcast(coolDown);
	}

	return true;
}

bool UCardDeck::SkipTopCard(float manualCooldownOverride)
{
	if (!usable || deck.Num() <= topCard) return false;
	bool isAlreadyInAction = deck[topCard]->GetPendingStatus();
	if (isAlreadyInAction) return false;

	float coolDown = (manualCooldownOverride != -1) ? manualCooldownOverride : 
		deck[topCard]->GetCoolDownTime() * (1 - skipResourceRefundPercent);

	DiscardTopCard();
	if(coolDown > 0) PutDeckOnCoolDown(coolDown);
	return true;
}

bool UCardDeck::IsUsable() const
{
	return usable;
}

const UCard* UCardDeck::GetTopCard()
{
	return (topCard < deck.Num()) ? deck[topCard] : nullptr;
}

const TArray<UCard*> UCardDeck::GetDeckContents() const
{
	return deck;
}

void UCardDeck::DiscardTopCard()
{
	++topCard;
	if (topCard >= deck.Num())
	{
		topCard = 0;
		Shuffle();
	}
}

void UCardDeck::Shuffle()
{
	int newInd;
	UCard* temp;
	for (int i = 0; i < deck.Num(); ++i)
	{
		newInd = FMath::RandRange(0, deck.Num() - 1);
		temp = deck[newInd];
		deck[newInd] = deck[i];
		deck[i] = temp;
	}
}

void UCardDeck::ChangeUseAmountOfTopCard(int usesToAdd, bool permanent)
{
	if (deck.Num() <= topCard) return;
	deck[topCard]->ChangeUseAmount(usesToAdd, permanent);
}

void UCardDeck::PutDeckOnCoolDown(float coolDownTime)
{
	usable = false;
	UWorld* world = GetWorld();
	if (world) 
	{
		GetWorld()->GetTimerManager().ClearTimer(coolDownTimer);
		GetWorld()->GetTimerManager().SetTimer(coolDownTimer, this, &UCardDeck::TakeDeckOffCoolDown, coolDownTime, false);
	}	
}

void UCardDeck::AdjustCurrentCoolDown(float coolDownMultiplier)
{
	if (usable) return;
	UWorld* world = GetWorld();
	if (world) 
	{
		float newCoolDown = GetWorld()->GetTimerManager().GetTimerRemaining(coolDownTimer) * coolDownMultiplier;
		PutDeckOnCoolDown(newCoolDown);
	}
}

void UCardDeck::AssignDeckInfo(UCardPool* owningPool, int index)
{
	owningCardPool = owningPool;
	deckIndex = index;
}

void UCardDeck::TakeDeckOffCoolDown()
{
	GetWorld()->GetTimerManager().ClearTimer(coolDownTimer);
	usable = true;
}

float UCardDeck::GetCoolDownTimePercentage()
{
	if (usable)
	{
		return 1.0f;
	}
	
	UWorld* world = GetWorld();
	if (world)
	{
		return 1.0f - (GetWorld()->GetTimerManager().GetTimerRemaining(coolDownTimer) / (GetWorld()->GetTimerManager().GetTimerElapsed(coolDownTimer) + GetWorld()->GetTimerManager().GetTimerRemaining(coolDownTimer)));
	}

	return 0.0f;
}

int UCardDeck::GetDeckIndex() const
{
	return deckIndex;
}

UCardPool* UCardDeck::GetOwningCardPool()
{
	return owningCardPool;
}
