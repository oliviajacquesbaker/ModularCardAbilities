// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileObject.h"

AProjectileObject::AProjectileObject()
{
	PrimaryActorTick.bCanEverTick = true;
	collider = CreateDefaultSubobject<USphereComponent>(FName("CollisionSphere"));
	if (collider != nullptr)
	{
		RootComponent = collider;
		collider->SetCollisionProfileName(collisionType);
		collider->OnComponentHit.AddDynamic(this, &AProjectileObject::OnHit);
	}	
}

void AProjectileObject::BeginPlay()
{
	Super::BeginPlay();	
}

void AProjectileObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move(DeltaTime);
}

void AProjectileObject::Launch(FVector position, FVector direction, bool isFunctional)
{
	if (!isFunctional) collider->SetCollisionProfileName("NoCollision");

	SetActorLocation(position + (direction * spawnOffsetGeneral));
	SetActorRotation(direction.ToOrientationRotator());
	movementDirection = direction;
	startingLocation = position;
	maxDestination = position + (direction * maxMovementDistance);
	allowedMoveTime = FVector::Dist(GetActorLocation(), maxDestination) / movementSpeed;

	if(usingProjectileMovement) projectileMovement->Velocity = movementDirection * projectileMovement->InitialSpeed;
}

void AProjectileObject::Move(float deltaTime)
{
	elapsedMoveTime += deltaTime;

	if (!usingProjectileMovement)
	{
		FVector newPosition = FMath::Lerp(startingLocation, maxDestination, elapsedMoveTime / allowedMoveTime);
		SetActorLocation(newPosition, true);
	}	

	if (FVector::Distance(maxDestination, GetActorLocation()) < acceptanceRadius) Cleanup();
}

void AProjectileObject::OnHit(UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, FVector normalImpulse, const FHitResult& hitResult)
{
	if (otherActor == owningActor || cleanupFired) return;
	HitResponse(otherActor, hitResult.ImpactPoint);
	Cleanup();
}

void AProjectileObject::HitResponse(AActor* otherActor, FVector impactPoint)
{
	if (!IsValid(otherActor)) return;
	if (!canHurtOwningActor && otherActor == owningActor) return;

	UDamageManager* healthOfHitActor = Cast<UDamageManager>(otherActor->GetComponentByClass(UDamageManager::StaticClass()));

	if (IsValid(owningCard)) //depend on card to handle damage first
	{
		if (healthOfHitActor && hitDamageResponse.operator bool()) hitDamageResponse(otherActor, impactPoint);
		else if (!healthOfHitActor && hitStaticResponse.operator bool()) hitStaticResponse(otherActor, impactPoint);
	}
	else if (fallbackDamage > 0 && healthOfHitActor) healthOfHitActor->Damage(owningActor, this, fallbackDamage, {EStatus::SE_Ranged});
}

void AProjectileObject::RegisterCardOwner(UCC_RangeBase* _owningCard)
{
	owningCard = _owningCard;
	if (owningCard != nullptr) SetActorLocation(owningCard->GetOwner()->GetActorLocation());
}

void AProjectileObject::RegisterActorOwner(AActor* _owningActor)
{
	owningActor = _owningActor;
}

void AProjectileObject::ResetCollisionOptions()
{
	collider->SetCollisionProfileName("Projectile");
}

void AProjectileObject::ForceProjectileMovement(float projectileInitialVelocity)
{
	projectileMovement = NewObject<UProjectileMovementComponent>(this);
	if (projectileMovement != nullptr)
	{
		usingProjectileMovement = true;
		projectileMovement->RegisterComponent();
		projectileMovement->UpdatedComponent = collider;
		projectileMovement->InitialSpeed = projectileInitialVelocity;
		projectileMovement->MaxSpeed = projectileInitialVelocity;
		projectileMovement->bRotationFollowsVelocity = true;
		projectileMovement->bShouldBounce = false;
	}
}

void AProjectileObject::Cleanup()
{
	cleanupFired = CleanupBP();
}

/// <summary>
/// Destroys projectile.
/// </summary>
/// <returns></returns>
bool AProjectileObject::CleanupBP_Implementation()
{
	//MarkAsGarbage(); //ML: trying to get my hit effects to not insta despawn (update: has issues doing it this way)
	Destroy();         //ML: instead, opting to just override in BP as needed, and let this script hard despawn on FX that aren't implemented yet.
	return true;
}

