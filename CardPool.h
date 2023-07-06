// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Card.h"
#include "CardDeck.h"
#include "DeckManipulationEnum.h"
#include "CardPool.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTPHEONIX_API UCardPool : public USceneComponent
{
	GENERATED_BODY()

public:	
	UCardPool();
	
	// Add a card to one of the Card Pool's decks.
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		const UCardDeck* AddToDeck(TSubclassOf<UCard> cardType, int deckNumber);
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		const bool SwapCards(UCard* cardA, UCard* cardB, bool requireDifferentDecks = false);
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		const bool RemoveCard(UCard* card);

	UFUNCTION(BlueprintCallable, Category = "Card Management")
		bool UseCardFromDeck(int deckNumber);
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		bool SkipCardFromDeck(int deckNumber, float manualCooldownOverride = -1);
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		void ShuffleDeck(int deckNumber);
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		void ChangeUsesOfDeck(int deckNumber, int usesToAdd, bool permanent = false);
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		void PutDeckOnCooldown(int deckNumber, float cooldownTime);
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		void TakeDeckOffCooldown(int deckNumber);

	UFUNCTION(BlueprintCallable, Category = "Card Management")
		const UCardDeck* GetDeck(int deckNumber) const;
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		const UTexture* GetUIFromDeck(int deckNumber);
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		const bool GetPendingStatusFromDeck(int deckNumber) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Card Management")
		void FlipCardUI(int deckNum);

protected:
	virtual void BeginPlay() override;
	void HandlePendingDeckChanges(int currentDeck);

private:	
	UPROPERTY(EditAnywhere, Category = "Card Management")
		int numberOfDecks = 4;

	UPROPERTY(EditAnywhere, Category = "Card Management")
		TArray<UCardDeck*> decks;

	TEnumAsByte<EManipulationType> pendingManipulationType = EManipulationType::M_None;
	int pendingChangeUseAmount = 0;
	bool pendingChangeUsePermanent = false;
	float pendingManualCooldownOverride = -1;

	class UProjectPheonixGameInstance* gameInstance;
};
