// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TimerManager.h"
#include "Card.h"
#include "CardDeck.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTopCardHasBeenUsed, float, Time);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTPHEONIX_API UCardDeck : public USceneComponent
{
	GENERATED_BODY()

public:
	UCardDeck();

	UPROPERTY(BlueprintAssignable, Category = "Card Management")
		FTopCardHasBeenUsed OnTopCardUsed;

	UFUNCTION(BlueprintCallable, Category = "Card Management")
		bool UseTopCard();
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		bool SkipTopCard(float manualCooldownOverride = -1);
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		void Shuffle();
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		void ChangeUseAmountOfTopCard(int usesToAdd, bool permanent = false);
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		void PutDeckOnCoolDown(float coolDownTime);
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		void TakeDeckOffCoolDown();

	UFUNCTION(BlueprintCallable, Category = "Card Management")
		bool IsUsable() const;
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		const UCard* GetTopCard();
	UFUNCTION(BlueprintCallable, Category = "Deck Management")
		const TArray<UCard*> GetDeckContents() const;
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		float GetCoolDownTimePercentage();
	UFUNCTION(BlueprintCallable, Category = "Deck Management")
		int GetDeckIndex() const;
	UFUNCTION(BlueprintCallable, Category = "Deck Management")
		class UCardPool* GetOwningCardPool();

	void AddCard(TSubclassOf<UCard> cardType);
	bool RemoveCard(UCard* card);
	bool SwapCard(UCard* oldCard, UCard* newCard);
	void AdjustCurrentCoolDown(float coolDownMultiplier);
	void AssignDeckInfo(UCardPool* owningPool, int index);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(EditAnywhere, Category = "Card Management")
		float skipResourceRefundPercent = 0.5f;

	UPROPERTY(VisibleAnywhere, Category = "Card Management")
		int topCard = 0;

	UPROPERTY(VisibleAnywhere, Category = "Card Management")
		bool usable = true;

	UPROPERTY(VisibleAnywhere, Category = "Card Management")
		TArray<UCard*> deck;

	FTimerHandle coolDownTimer;
	UCardPool* owningCardPool;
	int deckIndex = -1;

	void DiscardTopCard();
};
