// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CardPool.h"
#include "../Misc/MoneyPouch.h"
#include "Kismet/GameplayStatics.h"
#include "../ProjectPheonixGameInstance.h"
#include "CardShop.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom) )
class PROJECTPHEONIX_API UCardShop : public USceneComponent
{
	GENERATED_BODY()

public:	
	UCardShop();

	UFUNCTION(BlueprintCallable, Category = "Shop Options")
		bool RequestRemoveCard(UCard* cardToRemove, int costToRemove = 0);
	UFUNCTION(BlueprintCallable, Category = "Shop Options")
		bool RequestTransformCard(UCard* cardToTransform, int costToTransform = 0, bool constrainRarity = false);
	UFUNCTION(BlueprintCallable, Category = "Shop Options")
		bool RequestSwapCards(UCard* cardA, UCard* cardB, int costToSwap = 0);

protected:
	virtual void BeginPlay() override;

	UCardPool* playerCardPool{ nullptr };
	UMoneyPouch* playerMoneyPouch{ nullptr };	
	UProjectPheonixGameInstance* gameInstance{ nullptr };
};
