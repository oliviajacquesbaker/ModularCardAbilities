// Fill out your copyright notice in the Description page of Project Settings.


#include "CardPool.h"
#include "../ProjectPheonixGameInstance.h"

UCardPool::UCardPool()
{
	PrimaryComponentTick.bCanEverTick = false;

	for (int i = 0; i < numberOfDecks; ++i) 
	{
		decks.Emplace(CreateDefaultSubobject<UCardDeck>(TEXT("Deck") + i));
		decks[i]->SetupAttachment(this);
		decks[i]->AssignDeckInfo(this, i);
	}
}

void UCardPool::BeginPlay()
{
	Super::BeginPlay();	
	gameInstance = Cast<UProjectPheonixGameInstance>(UGameplayStatics::GetGameInstance(this));
}

const UCardDeck* UCardPool::AddToDeck(TSubclassOf<UCard> cardType, int deckNumber)
{
	deckNumber = deckNumber % numberOfDecks;
	if (decks.Num() <= deckNumber) return nullptr;
	decks[deckNumber]->AddCard(cardType);
	return decks[deckNumber];
}

const bool UCardPool::SwapCards(UCard* cardA, UCard* cardB, bool requireDifferentDecks)
{
	if (!cardA || !cardB) return false;
	UCardDeck* deckA = cardA->GetOwningDeck();
	UCardDeck* deckB = cardB->GetOwningDeck();

	if (requireDifferentDecks && deckA == deckB) return false;

	bool successA = (deckA) ? deckA->SwapCard(cardA, cardB) : true;
	bool successB = (deckB) ? deckB->SwapCard(cardB, cardA) : true;
	return (successA && successB);
}

const bool UCardPool::RemoveCard(UCard* card)
{
	if (!card) return false;
	UCardDeck* deck = card->GetOwningDeck();
	if (!deck) return false;

	return deck->RemoveCard(card);
}

const UCardDeck* UCardPool::GetDeck(int deckNumber) const
{
	deckNumber = deckNumber % numberOfDecks;
	if (decks.Num() <= deckNumber) return nullptr;
	return decks[deckNumber];
}

bool UCardPool::UseCardFromDeck(int deckNumber)
{
	if (pendingManipulationType != EManipulationType::M_None) HandlePendingDeckChanges(deckNumber);

	deckNumber = deckNumber % numberOfDecks;
	if (decks.Num() <= deckNumber) return false;

	bool useResult = decks[deckNumber]->UseTopCard();
	if (pendingManipulationType == EManipulationType::M_CooldownSkip) HandlePendingDeckChanges(deckNumber);

	return useResult;
}

bool UCardPool::SkipCardFromDeck(int deckNumber, float manualCooldownOverride)
{
	if (deckNumber == -1) //a request to affect the next used deck, whichever one that is
	{
		pendingManipulationType = EManipulationType::M_Skip;
		pendingManualCooldownOverride = manualCooldownOverride;
		return true;
	}

	deckNumber = deckNumber % numberOfDecks;
	if (decks.Num() <= deckNumber) return false;
	bool skipped = decks[deckNumber]->SkipTopCard(manualCooldownOverride);

	if (skipped) FlipCardUI(deckNumber);
	return skipped;
}

void UCardPool::ShuffleDeck(int deckNumber)
{
	if (deckNumber == -1) //a request to affect the next used deck, whichever one that is
	{
		pendingManipulationType = EManipulationType::M_Shuffle;
		return;
	}

	deckNumber = deckNumber % numberOfDecks;
	if (decks.Num() <= deckNumber) return;
	decks[deckNumber]->Shuffle();
}

void UCardPool::ChangeUsesOfDeck(int deckNumber, int usesToAdd, bool permanent)
{
	if (deckNumber == -1) //a request to affect the next used deck, whichever one that is
	{
		pendingManipulationType = EManipulationType::M_ChangeUse;
		pendingChangeUseAmount = usesToAdd;
		pendingChangeUsePermanent = permanent;
		return;
	}

	deckNumber = deckNumber % numberOfDecks;
	if (decks.Num() <= deckNumber) return;
	decks[deckNumber]->ChangeUseAmountOfTopCard(usesToAdd, permanent);
}

void UCardPool::PutDeckOnCooldown(int deckNumber, float cooldownTime)
{
	if (deckNumber == -1) //a request to affect the next used deck, whichever one that is
	{
		pendingManipulationType = EManipulationType::M_Cooldown;
		pendingManualCooldownOverride = cooldownTime;
		return;
	}

	deckNumber = deckNumber % numberOfDecks;
	if (decks.Num() <= deckNumber) return;
	decks[deckNumber]->PutDeckOnCoolDown(cooldownTime);
}

void UCardPool::TakeDeckOffCooldown(int deckNumber)
{
	if (deckNumber == -1) //a request to affect the next used deck, whichever one that is
	{
		pendingManipulationType = EManipulationType::M_CooldownSkip;
		return;
	}

	deckNumber = deckNumber % numberOfDecks;
	if (decks.Num() <= deckNumber) return;
	decks[deckNumber]->TakeDeckOffCoolDown();
}

const UTexture* UCardPool::GetUIFromDeck(int deckNumber)
{
	deckNumber = deckNumber % numberOfDecks;
	if (decks.Num() <= deckNumber) return nullptr;

	const UCard* card = decks[deckNumber]->GetTopCard();
	return (card != nullptr) ? card->GetUiImage() : nullptr;
}

const bool UCardPool::GetPendingStatusFromDeck(int deckNumber) const
{
	deckNumber = deckNumber % numberOfDecks;
	if (decks.Num() <= deckNumber) return false;

	const UCard* card = decks[deckNumber]->GetTopCard();
	return (card != nullptr) ? card->GetPendingStatus() : false;
}

void UCardPool::FlipCardUI_Implementation(int deckNum)
{
}

void UCardPool::HandlePendingDeckChanges(int currentDeck)
{
	switch (pendingManipulationType)
	{
	case M_Skip:
		SkipCardFromDeck(currentDeck, pendingManualCooldownOverride);
		pendingManualCooldownOverride = 0;
		break;
	case M_Shuffle:
		ShuffleDeck(currentDeck);
		break;
	case M_ChangeUse:
		ChangeUsesOfDeck(currentDeck, pendingChangeUseAmount, pendingChangeUsePermanent);
		pendingChangeUseAmount = 0;
		pendingChangeUsePermanent = false;
		break;
	case M_Cooldown:
		PutDeckOnCooldown(currentDeck, pendingManualCooldownOverride);
		pendingManualCooldownOverride = 0;
		break;
	case M_CooldownSkip:
		TakeDeckOffCooldown(currentDeck);
		break;
	default:
		break;
	}

	pendingManipulationType = EManipulationType::M_None;
}
