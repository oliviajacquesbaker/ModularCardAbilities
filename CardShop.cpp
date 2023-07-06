// Fill out your copyright notice in the Description page of Project Settings.


#include "CardShop.h"

UCardShop::UCardShop()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCardShop::BeginPlay()
{
	Super::BeginPlay();

	AActor* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (playerPawn)
	{
		playerMoneyPouch = Cast<UMoneyPouch>(playerPawn->GetComponentByClass(UMoneyPouch::StaticClass()));
		playerCardPool = Cast<UCardPool>(playerPawn->GetComponentByClass(UCardPool::StaticClass()));
	}
	gameInstance = Cast<UProjectPheonixGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

bool UCardShop::RequestRemoveCard(UCard* cardToRemove, int costToRemove)
{
	if (!playerMoneyPouch || !playerCardPool || !cardToRemove) return false;
	bool hasFundsAvailable = playerMoneyPouch->AttemptWithdrawCurrency(costToRemove);

	if (hasFundsAvailable) return playerCardPool->RemoveCard(cardToRemove);
	else return false;
}

bool UCardShop::RequestTransformCard(UCard* cardToTransform, int costToTransform, bool constrainRarity)
{
	if (!playerMoneyPouch || !playerCardPool || !cardToTransform) return false;
	bool hasFundsAvailable = playerMoneyPouch->AttemptWithdrawCurrency(costToTransform);

	if (hasFundsAvailable && gameInstance)
	{
		UCard* newCard;
		if (constrainRarity) newCard = gameInstance->GetRandomCardAsGood(cardToTransform);
		else newCard = gameInstance->GetRandomCard();

		if (!newCard) return false;
		return playerCardPool->SwapCards(cardToTransform, newCard);
	}
	return false;
}

bool UCardShop::RequestSwapCards(UCard* cardA, UCard* cardB, int costToSwap)
{
	if (!playerMoneyPouch || !playerCardPool || !cardA || !cardB) return false;
	bool hasFundsAvailable = playerMoneyPouch->AttemptWithdrawCurrency(costToSwap);

	if (hasFundsAvailable) return playerCardPool->SwapCards(cardA, cardB, true);
	else return false;
}


