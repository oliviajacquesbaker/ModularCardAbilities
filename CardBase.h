// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CardBase.generated.h"


UCLASS(EditInlineNew, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTPHEONIX_API UCardBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCardBase();

	// Register card component with a specific card.
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		virtual void RegisterWithCard(class UCard* card);

	// Register card component with a specific deck.
	UFUNCTION(BlueprintCallable, Category = "Card Management")
		virtual void RegisterWithDeck(class UCardDeck* deck);

	// If this happens pre- "main use" cycle, activate it here. Used for components that modify other components, etc.
	UFUNCTION(BlueprintCallable, Category = "Card Behavior")
		virtual void PreUse(AActor* owningActor);

	// If this happens during the "main use" cycle, activate it here. Used for components considered the core of the card, such as spawning and launching a projectile. If there is a multi use component, it should happen here to properlly communicate with the owning deck.
	UFUNCTION(BlueprintCallable, Category = "Card Behavior")
		virtual bool Use(AActor* owningActor);
	
	// If this happens only when another component triggers a damageable hit event, activate it here. Used for components that apply an effect to hit characters, such as applying a status, etc.
	UFUNCTION(BlueprintCallable, Category = "Card Behavior")
		virtual void OnDamageableHit(AActor* owningActor, AActor* objectCausingHit, AActor* hitActor, FVector impactPoint);

	// If this happens only when another component triggers a NON damageable hit event, activate it here. Used for components that react to hit objects, such as bouncing off the ground, etc.
	UFUNCTION(BlueprintCallable, Category = "Card Behavior")
		virtual void OnStaticHit(AActor* owningActor, AActor* objectCausingHit, AActor* hitActor, FVector impactPoint);

	// If this happens when a movement event is raised, activate it here. Used for components that happen after special movement has ocurred, such as a ground slam.
	UFUNCTION(BlueprintCallable, Category = "Card Behavior")
		virtual void OnMovementEvent(AActor* owningActor);

	// Manage the cooldown of the containing deck upon successful use of this card.
	UFUNCTION(BlueprintCallable, Category = "Card Behavior")
		virtual void AdjustCooldown(float cooldownMultiplier);

	// Perform any card based actions meant as reactions to status successes (such as refunding card resources).
	UFUNCTION(BlueprintCallable, Category = "Card Behavior")
		virtual void RespondToStatusSuccess(bool statusWasSuccessful);

	void OverrideCardDamage(int damage);
	int GetOverridenCardDamage();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/// <summary>
	/// Activate this card component functionality.
	/// </summary>
	/// <param name="owningActor">The actor that owns this card.</param>
	/// <param name="hitActor">IF this activation happened as the result of a hit, this is the hit actor. This is defaulted to nullptr otherwise.</param>
	/// <param name="impactPoint">IF this activation happened as the result of a hit, this is the impact point. This is defaulted to a zero vector otherwise.</param>
	/// <returns>A boolean indicating if the use requires at least one more call to complete activation.</returns>
	UFUNCTION(BlueprintCallable, Category = "Card Behavior")
		virtual bool ActivateCardComponent(AActor* owningActor, AActor* objectCausingHit = nullptr,  AActor* hitActor = nullptr, FVector impactPoint = FVector::ZeroVector);

	//necessary for windups, since timer delegates cant use functions with return types.
	UFUNCTION(BlueprintCallable, Category = "Card Behavior")
		void DummyActivation(AActor* owningActor, AActor* objectCausingHit = nullptr, AActor* hitActor = nullptr, FVector impactPoint = FVector::ZeroVector);

	UFUNCTION(BlueprintCallable, Category = "Card Behavior")
		void TimerForComponentActivation(AActor* owningActor, AActor* objectCausingHit = nullptr, AActor* hitActor = nullptr, FVector impactPoint = FVector::ZeroVector);

	// Raise a hit event on a damageable actor in the owning card.
	UFUNCTION(BlueprintCallable, Category = "Card Behavior")
		virtual void RaiseDamageableHitEvent(AActor* objectCausingHit, AActor* hitActor, FVector impactPoint);

	// Raise a hit event on a NON damageable actor in the owning card.
	UFUNCTION(BlueprintCallable, Category = "Card Behavior")
		virtual void RaiseStaticHitEvent(AActor* objectCausingHit, AActor* hitActor, FVector impactPoint);

	// Raise a movement event in the owning card.
	UFUNCTION(BlueprintCallable, Category = "Card Behavior")
		virtual void RaiseMovementEvent();
	
	// The delay between activation and actual functionality for this component specifically. Note that setting a non zero windup time for a component that happens in the main use sequence disqualifies it for multi use activation.
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		float windUpTime = 0.0f;

	// Whether this component activates in the pre use cycle.
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		bool activateDuringPreUseCycle = false;

	// Whether this component activates in the main use cycle.
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		bool activateDuringMainUseCycle = true;

	// Whether this component activates when another component on this card raises a damage hit event.
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		bool activateOnDamageHitsFromOtherComponents = false;

	// Whether this component activates when another component on this card raises a static hit event.
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		bool activateOnStaticHitsFromOtherComponents = false;

	// Whether this component activates when another component on this card raises a movement event.
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		bool activateOnMovementEventsFromOtherComponents = false;

	// Whether this component can raise the damage hit event - for components that get activated on damage hits, turn this off to prevent recursive calls.
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		bool canRaiseDamageableHitEvents = true;

	// Whether this component can raise the static hit event - for components that get activated on static hits, turn this off to prevent recursive calls.
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		bool canRaiseStaticHitEvents = true;

	// Whether this component can raise the movement event - for components that get activated on movement events, turn this off to prevent recursive calls.
	UPROPERTY(EditAnywhere, Category = "Card Behavior")
		bool canRaiseMovementEvents = false;

	UCard* owningCard{ nullptr };
	UCardDeck* owningDeck{ nullptr };

	FTimerHandle windupTimer;
};
