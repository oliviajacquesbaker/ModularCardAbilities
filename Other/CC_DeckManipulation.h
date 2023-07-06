// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CardBase.h"
#include "../CardDeck.h"
#include "../CardPool.h"
#include "../DeckManipulationEnum.h"
#include "Kismet/KismetMathLibrary.h"
#include "CC_DeckManipulation.generated.h"

/**
 *
 */
UCLASS()
class PROJECTPHEONIX_API UCC_DeckManipulation : public UCardBase
{
	GENERATED_BODY()

public:
	virtual bool ActivateCardComponent(AActor* owningActor, AActor* objectCausingHit = nullptr,  AActor* hitActor = nullptr, FVector impactPoint = FVector::Zero()) override;

protected:
	virtual void BeginPlay() override;

	void SkipCards();
	void ShuffleCards();
	void ChangeCardUses();
	void PutCardsOnCooldown();
	void TakeCardsOffCooldown();
	void DistinguishSelectedDecks();
	void FindOwningCardPool();

	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		TEnumAsByte<EManipulationType> deckManipulationType;

	UPROPERTY(EditAnywhere, Category = "Card Behavior - Change Use Options")
		bool changeIsPermanent = false;
	UPROPERTY(EditAnywhere, Category = "Card Behavior - Change Use Options")
		int usesToAdd = 0;

	UPROPERTY(EditAnywhere, Category = "Card Behavior - Skip Card Options")
		float coolDownAfterSkip = 0;

	UPROPERTY(EditAnywhere, Category = "Card Behavior - Cool Down Options")
		float forcedCoolDownAmount = 0;

	UPROPERTY(EditAnywhere, Category = "Card Behavior - Affected Decks")
		bool affectDeck0 = false;
	UPROPERTY(EditAnywhere, Category = "Card Behavior - Affected Decks")
		bool affectDeck1 = false;
	UPROPERTY(EditAnywhere, Category = "Card Behavior - Affected Decks")
		bool affectDeck2 = false;
	UPROPERTY(EditAnywhere, Category = "Card Behavior - Affected Decks")
		bool affectDeck3 = false;
	UPROPERTY(EditAnywhere, Category = "Card Behavior - Affected Decks")
		bool affectCurrentDeck = false;
	//for when you want to flag 'every deck but the current one' - click decks 1-3 AND this one
	UPROPERTY(EditAnywhere, Category = "Card Behavior - Affected Decks")
		bool forceDontAffectCurrentDeck = false;

	//By making the 'next deck' option true, you invalidate all other selections!
	UPROPERTY(EditAnywhere, Category = "Card Behavior - Affected Decks")
		bool affectNextUsedDeck = false;

	//By making the 'random' option true, you invalidate all other selections!
	UPROPERTY(EditAnywhere, Category = "Card Behavior - Affected Decks")
		bool affectRandomDeck = false;

	TArray<int> affectedDecks = {};
	UCardPool* owningCardPool;
};
