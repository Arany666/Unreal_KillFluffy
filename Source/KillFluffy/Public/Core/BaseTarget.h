// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTarget.generated.h"

class UActorSequenceComponent;
class UWorldManager;

UCLASS()
class KILLFLUFFY_API ABaseTarget : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	bool bKilled;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootNormal;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootDetachAble;


	UPROPERTY(EditAnywhere)
	FComponentReference ActorRef;
	UPROPERTY(VisibleAnywhere)
	UActorSequenceComponent* ActorSequenceCache;
	UActorSequenceComponent* GetActorSequence();

	UPROPERTY(EditAnywhere,Transient)
	UWorldManager* WorldManagerCache;
	UWorldManager* GetWorldManager();

	UPROPERTY(EditAnywhere,Transient)
	TArray<UPrimitiveComponent*> DetachAbles;
	void CollectDetachAbles();

	void BlowDetachAbles();

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USoundBase> BlowSound;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USoundBase> ImpactSound;

	void PlaySound(TSoftObjectPtr<USoundBase> Sound ,FVector Pos);


	UPROPERTY(VisibleAnywhere)
	bool bVulnerable;

	UPROPERTY(EditAnywhere)
	int ActiveCycle = 1;
	UPROPERTY(VisibleAnywhere)
	int CurrentActiveCycle;

	UPROPERTY(EditAnywhere)
	float CycleTimeOut = 2;
	UPROPERTY(VisibleAnywhere)
	float CurrentCycleTimeOut;
	
	
public:	
	// Sets default values for this actor's properties
	ABaseTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float MinImpactVelocity = 100;
	UFUNCTION()
	void OnPieceComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:

	bool IsKilled() { return bKilled; }
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	 void TargetUpdate(float DeltaTime) ;

	UFUNCTION(BlueprintPure)
	 bool IsVulnerable() { return bVulnerable; }

	UFUNCTION(BlueprintImplementableEvent)
	void StartCycle();
	UFUNCTION(BlueprintImplementableEvent)
	void StopCycle();
	UFUNCTION(BlueprintCallable)
	void CycleFinished();

	void StartCycleInner();
	void StopCycleInner();
	UFUNCTION()
	void CycleFinishedInner();

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
};
