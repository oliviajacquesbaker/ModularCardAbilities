// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileObject.h"
#include "Projectile_Pulsing.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPHEONIX_API AProjectile_Pulsing : public AProjectileObject
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Cleanup() override;

	void Pulse();
	UFUNCTION(BlueprintCallable)
		void PulseOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
	UFUNCTION(BlueprintCallable)
		void ForceDestroy(AActor* toDestroy);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool BPFXSpawnEvent();

	UPROPERTY(EditAnywhere, Category = "Projectile Options")
		float pulseInterval = 1.5f;
	UPROPERTY(EditAnywhere, Category = "Projectile Options")
		float pulseFinalScale = 5.0f;
	UPROPERTY(EditAnywhere, Category = "Projectile Options")
		float pulseScaleTime = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Projectile Options")
		float pulseDamageAmount = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Projectile Options")
		TArray<TEnumAsByte<EStatus>> pulseDamageType;
	UPROPERTY(EditAnywhere, Category = "Projectile Options")
		TSubclassOf<class AActor> pulseType;

	TArray<FTimerHandle> spawnedDestructionTimers;
	TArray<AActor*> activePulseObjects;
	FTimerHandle pulseTimerHandle;
	float scaleRate;
};
