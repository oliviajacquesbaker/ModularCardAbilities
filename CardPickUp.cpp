// Fill out your copyright notice in the Description page of Project Settings.


#include "CardPickUp.h"
#include "Card.h"
#include "../Interaction/CardChest.h"

// Sets default values
ACardPickUp::ACardPickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	frontImage = CreateDefaultSubobject<UWidgetComponent>(TEXT("FrontImage"));
	backImage = CreateDefaultSubobject<UWidgetComponent>(TEXT("BackImage"));
	
	frontImage->SetupAttachment(root);
	backImage->SetupAttachment(root);
}

// Called when the game starts or when spawned
void ACardPickUp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACardPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACardPickUp::InitializeCardInfo(UCard* cardToInit, ACardChest* owningChest)
{
	if (!cardToInit || !owningChest) return;
	card = cardToInit;
	chest = owningChest;
	ChangeImage(card->GetImage());
}

UCard* ACardPickUp::GetCard() const 
{
	return card;
}


void ACardPickUp::DestroyChest() 
{
	chest->DeleteSelf();
}

void ACardPickUp::ChangeImage_Implementation(const UTexture* cardImg) 
{
}