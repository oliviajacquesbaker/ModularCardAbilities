// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMathVectorCommon.h"
#include "MultiStepActivationObject.h"
#include "GravityOrb.generated.h"

UCLASS(Blueprintable)
class PROJECTPHEONIX_API AGravityOrb : public AMultiStepActivationObject
{
	GENERATED_BODY()
	
public:	
	AGravityOrb();
	virtual void Tick(float DeltaTime) override;

	virtual void AdvanceActivationProcess(AActor* owningActor) override;

	UFUNCTION(BlueprintCallable, Category = "Orb Effects")
		void Launch(FVector position, FVector direction);
	UFUNCTION(BlueprintCallable, Category = "Orb Effects")
		void BeginPull();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool BPFXSpawnEvent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void Cleanup();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Projectile Options")
		bool CleanupBP();
	void Move(float deltaTime);
	void Pull(float deltaTime);

	UPROPERTY(EditAnywhere, Category = "Orb Effects")
		float movementSpeed = 5.0f;
	UPROPERTY(EditAnywhere, Category = "Orb Effects")
		float maxMovementDistance = 1000.0f;
	UPROPERTY(EditAnywhere, Category = "Orb Effects")
		float gravitationalPullTime = 2.0f;
	UPROPERTY(EditAnywhere, Category = "Orb Effects")
		float scaleSpeed = 0.002f;
	UPROPERTY(EditAnywhere, Category = "Orb Effects")
		float gravitationalForce = 10.0f;
	UPROPERTY(EditAnywhere, Category = "Orb Effects")
		float captureRadius = 500.0f;

	float elapsedMoveTime = 0.0f;
	float allowedMoveTime = 0.0f;
	float acceptanceRadius = 10.0f;
	float errorAcceptance = 1.0f;
	FVector destinationLocation;
	FTimerHandle destructionTimerHandle;
	TArray<AActor*> caughtObjects;
};
