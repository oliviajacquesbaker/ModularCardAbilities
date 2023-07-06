// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_DeckManipulation.h"

bool UCC_DeckManipulation::ActivateCardComponent(AActor* owningActor, AActor* objectCausingHit,  AActor* hitActor, FVector impactPoint)
{
	if (owningCardPool == nullptr) FindOwningCardPool();

	DistinguishSelectedDecks();
	switch (deckManipulationType)
	{
	case M_Skip:
		SkipCards();
		break;
	case M_Shuffle:
		ShuffleCards();
		break;
	case M_ChangeUse:
		ChangeCardUses();
		break;
	case M_Cooldown:
		PutCardsOnCooldown();
		break;
	default:
		break;
	}

	return false;
}

void UCC_DeckManipulation::BeginPlay()
{
	Super::BeginPlay();
	FindOwningCardPool();
}

void UCC_DeckManipulation::SkipCards()
{
	if (!owningCardPool) return;

	for (int deckIndex : affectedDecks)
	{
		owningCardPool->SkipCardFromDeck(deckIndex, coolDownAfterSkip);
	}
}

void UCC_DeckManipulation::ShuffleCards()
{
	if (!owningCardPool) return;

	for (int deckIndex : affectedDecks)
	{
		owningCardPool->ShuffleDeck(deckIndex);
	}
}

void UCC_DeckManipulation::ChangeCardUses()
{
	if (!owningCardPool) return;

	for (int deckIndex : affectedDecks)
	{
		owningCardPool->ChangeUsesOfDeck(deckIndex, usesToAdd, changeIsPermanent);
	}
}

void UCC_DeckManipulation::PutCardsOnCooldown()
{
	if (!owningCardPool) return;

	for (int deckIndex : affectedDecks)
	{
		owningCardPool->PutDeckOnCooldown(deckIndex, forcedCoolDownAmount);
	}
}

void UCC_DeckManipulation::TakeCardsOffCooldown()
{
	if (!owningCardPool) return;

	for (int deckIndex : affectedDecks)
	{
		owningCardPool->TakeDeckOffCooldown(deckIndex);
	}
}

void UCC_DeckManipulation::DistinguishSelectedDecks()
{
	affectedDecks.Empty();

	if (affectNextUsedDeck) affectedDecks.Add(-1);
	else if (affectRandomDeck)
	{
		int randDeck = UKismetMathLibrary::RandomInteger(4);
		affectedDecks.Add(randDeck);
	}
	else
	{
		int currentDeck = -1;
		if (affectCurrentDeck && owningDeck != nullptr)
		{
			currentDeck = owningDeck->GetDeckIndex();
			affectedDecks.Add(currentDeck);
		}

		if (affectDeck0 && currentDeck != 0) affectedDecks.Add(0);
		if (affectDeck1 && currentDeck != 1) affectedDecks.Add(1);
		if (affectDeck2 && currentDeck != 2) affectedDecks.Add(2);
		if (affectDeck3 && currentDeck != 3) affectedDecks.Add(3);

		if (forceDontAffectCurrentDeck) affectedDecks.Remove(currentDeck);
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("affected decks: %d"), affectedDecks.Num()));
}

void UCC_DeckManipulation::FindOwningCardPool()
{
	if (owningDeck == nullptr) return;
	owningCardPool = owningDeck->GetOwningCardPool();
}
