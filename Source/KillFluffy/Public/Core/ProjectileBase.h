// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;

UCLASS()
class KILLFLUFFY_API AProjectileBase : public AActor
{
	GENERATED_BODY()

	//UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere)
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere,Transient)
	UPrimitiveComponent* ComponentCache;

	UPrimitiveComponent* GetComponentCache();

	UPROPERTY(VisibleAnywhere)
	bool HitDetected;
	UPROPERTY(VisibleAnywhere)
	FVector VelocityCache;

	UPROPERTY(VisibleAnywhere)
	bool bKilled;

	UPROPERTY(EditAnywhere)
	float MinImpactVelocity = 5000;
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();
	void SetupProjectile();

	void SetKilled() { bKilled = true; }
	bool IsKilled() { return bKilled; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetProjectileVelocity(FVector& Velocity, float MassRef = 100.f);
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	UFUNCTION()
	void OnComponentHit(/* UPrimitiveComponent OnComponentHit,*/ UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );
};
