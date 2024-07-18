// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MechanicManager.generated.h"

class AProjectileBase;
class UWorldManager;
class ABaseTarget;
class UMechanicBase;

UCLASS()
class KILLFLUFFY_API AMechanicManager : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	UPROPERTY(EditAnywhere)
	float RoundTime = 5.f;
	UPROPERTY(EditAnywhere,Transient)
	TArray<UMechanicBase*> MechanicsCache;


	void CollectMechanics();
	void SetupMechanics();

	UPROPERTY(EditAnywhere, Transient)
	TArray<ABaseTarget*> TargetCache;

	void CollectTargets();
	bool AllTargetKilled();

	UPROPERTY(EditAnywhere, Transient)
	UWorldManager* WorldManagerCache;
	UWorldManager* GetWorldManager();

	UFUNCTION()
	void OnTargetGotHit(ABaseTarget* Target, AProjectileBase* Projectile);

	void TimerUpdate(float DeltaTime);
	
public:	
	// Sets default values for this actor's properties
	AMechanicManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetupMechanicManager();

};
