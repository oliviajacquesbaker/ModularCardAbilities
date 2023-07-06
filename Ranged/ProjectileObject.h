// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CC_RangeBase.h"
#include "../../Status/StatusEnum.h"
#include "../../Status/DamageManagers/DamageManager.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "ProjectileObject.generated.h"


UCLASS()
class PROJECTPHEONIX_API AProjectileObject : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectileObject();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile Options")
		virtual void Launch(FVector position, FVector direction, bool isFunctional = true);
	UFUNCTION(BlueprintCallable, Category = "Projectile Options")
		virtual void RegisterCardOwner(UCC_RangeBase* owningCard);
	UFUNCTION(BlueprintCallable, Category = "Projectile Options")
		virtual void RegisterActorOwner(AActor* owningActor);
	UFUNCTION(BlueprintCallable, Category = "Projectile Options")
		void ResetCollisionOptions();
	UFUNCTION(BlueprintCallable, Category = "Projectile Options")
		virtual void OverrideMaxDistance(float max) { if (max < maxMovementDistance) maxMovementDistance = max; }
	UFUNCTION(BlueprintCallable, Category = "Projectile Options")
		void ForceProjectileMovement(float projectileInitialVelocity = 1000.0f);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Projectile Options")
		bool CleanupBP();

	void SetHitDamageableResponse(TFunction<void(AActor*, FVector)> func) { hitDamageResponse = func; };
	void SetHitStaticResponse(TFunction<void(AActor*, FVector)> func) { hitStaticResponse = func; };

protected:
	virtual void BeginPlay() override;
	virtual void Move(float deltaTime);
	virtual void HitResponse(AActor* otherActor, FVector impactPoint);
	virtual void Cleanup();

	UFUNCTION(Category = "Projectile Options")
		virtual void OnHit(UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, FVector normalImpulse, const FHitResult& hit);

	UPROPERTY(EditAnywhere, Category = "Projectile Options")
		float spawnOffsetGeneral = 40.0f;
	UPROPERTY(EditAnywhere, Category = "Projectile Options")
		float movementSpeed = 5.0f;
	UPROPERTY(EditAnywhere, Category = "Projectile Options")
		float maxMovementDistance = 10000.0f;
	UPROPERTY(EditAnywhere, Category = "Projectile Options")
		USphereComponent* collider;
	UPROPERTY(EditAnywhere, Category = "Projectile Options")
		FName collisionType = "Projectile";
	UPROPERTY(EditAnywhere, Category = "Projectile Options")
		bool canHurtOwningActor = false;
	//if this projectile has no card owner, how much damage should it do?
	UPROPERTY(EditAnywhere, Category = "Projectile Options")
		float fallbackDamage = 0.0f;

	float elapsedMoveTime = 0.0f;
	float allowedMoveTime = 0.0f;
	float acceptanceRadius = 10.0f;
	FVector movementDirection = FVector(0);
	FVector originPoint;
	FVector maxDestination;
	FVector startingLocation;
	UCC_RangeBase* owningCard;
	AActor* owningActor;
	bool cleanupFired = false;

	bool usingProjectileMovement = false;
	UProjectileMovementComponent* projectileMovement;
		

	TFunction<void(AActor*, FVector)> hitDamageResponse;
	TFunction<void(AActor*, FVector)> hitStaticResponse;
};
