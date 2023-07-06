// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CardBase.h"
#include "../Misc/FXSpawner.h"
#include "../../../Plugins/Wwise/Source/AkAudio/Classes/AkAudioEvent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Card.generated.h"


UCLASS(Blueprintable, BlueprintType, EditInlineNew, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTPHEONIX_API UCard : public USceneComponent
{
	GENERATED_BODY()

public:	
	UCard();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Info")
		FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Info")
		FString description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Info")
		int rarity;

	/// <summary>
	/// Use this card. Calls the pre and main use cycle on all of the contained components with base class CardBase.
	/// </summary>
	/// <returns>A boolean indicating whether the card should be discarded after use. 
	/// i.e. for multi use cards, you may want to hold on to them in your hand longer, so return false. 
	/// But for single use cards, it would always return true.</returns>
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		bool Use(AActor* owningActor, bool VFXEnabled = true);

	UFUNCTION(BlueprintCallable, Category = "Card Management")
		bool UseInstantly(AActor* owningActor);

	UFUNCTION(BlueprintCallable, Category = "Card Management")
		void BroadcastDamageableHitEvent(AActor* objectCausingHit, AActor* hitActor, FVector impactPoint);

	UFUNCTION(BlueprintCallable, Category = "Card Management")
		void BroadcastStaticHitEvent(AActor* objectCausingHit, AActor* hitActor, FVector impactPoint);

	UFUNCTION(BlueprintCallable, Category = "Card Management")
		void BroadcastMovementEvent();

	UFUNCTION(BlueprintCallable, Category = "Card Management")
		void ChangeUseAmount(int usesToAdd, bool permanent = false);

	UFUNCTION(BlueprintCallable, Category = "Card Management")
		const UTexture* GetImage() const;
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		const UTexture* GetUiImage() const;
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		UCardDeck* GetOwningDeck() const;
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		const float GetCoolDownTime() const;
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		const float GetWindUpTime() const;
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		const UClass* GetEffectType() const;
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		const bool GetPendingStatus() const;

	void RegisterWithDeck(UCardDeck* deck);
	void EnsureComponentRegistration();

	void OverrideDamage(int damage) { overriddenDamage = damage; };
	int GetOverridenDamage() { return overriddenDamage; };

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:	
	UPROPERTY(EditAnywhere, Category = "Card Components", Instanced)
		TArray<UCardBase*> cardComponents;

	UPROPERTY(VisibleAnywhere, Category = "Card Effects")
		UFXSpawner* fxSpawner;

	UPROPERTY(EditAnywhere, Category = "Card Effects")
		UNiagaraSystem* windUpEffect;

	UPROPERTY(EditAnywhere, Category = "Card Effects")
		UNiagaraSystem* useEffect;

	UPROPERTY(EditAnywhere, Category = "Card Effects")
		UTexture* cardImage;

	UPROPERTY(EditAnywhere, Category = "Card Effects")
		UTexture* uiImage;

	UPROPERTY(EditAnywhere, Category = "Card Audio")
		UAkAudioEvent* cardSound;

	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		int usesBeforeDiscard = 1;

	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		float coolDownTime = 2.5f;

	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		float windUpTime = 0.0;

	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		UClass* effectType;

	UFUNCTION(BlueprintCallable, Category = "Card Management")
		void BeginCardFunctionality(AActor* owningActor, bool VFXEnabled = true);

	void ResetCard();

	UCardDeck* owningDeck;
	AActor* owningActor;
	int currentUseCount = 0;
	bool hasPendingActions = false;
	FTimerHandle useAfterWindupHandle;
	FTimerDelegate useAfterWindupDelegate;

	//may or may not be used depending on the cards and conditions.
	float overriddenDamage = 0;
};
