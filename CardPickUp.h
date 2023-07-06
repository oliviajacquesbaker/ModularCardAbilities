// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "CardPickUp.generated.h"

class ACardChest;
class UCard;

UCLASS()
class PROJECTPHEONIX_API ACardPickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACardPickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UWidgetComponent* frontImage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UWidgetComponent* backImage;

	UFUNCTION(BlueprintCallable)
		void InitializeCardInfo(UCard* cardToInit, ACardChest* owningChest);

	UFUNCTION(BlueprintCallable)
		UCard* GetCard() const;

	UFUNCTION(BlueprintNativeEvent)
		void ChangeImage(const UTexture* cardImg);

	UFUNCTION(BlueprintCallable)
		void DestroyChest();

private:
	UPROPERTY(VisibleAnywhere)
		UCard* card;
	UPROPERTY(VisibleAnywhere)
		ACardChest* chest;
};
