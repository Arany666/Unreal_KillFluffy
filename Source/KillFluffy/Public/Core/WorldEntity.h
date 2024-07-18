// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CutSceneManager.h"
#include "GameFramework/Actor.h"
#include "WorldEntity.generated.h"

class UActorSequenceComponent;
class UCutSceneManager;
class ULevelSequence;

DECLARE_DELEGATE(FWorldEntitySignature)

UCLASS()
class KILLFLUFFY_API AWorldEntity : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere,Transient)
	UCutSceneManager* CutSceneManagerCache;
	UCutSceneManager* GetCutSceneManager();

	UPROPERTY(EditAnywhere)
	USceneComponent* MechanicManagerHolder;

	//UPROPERTY(EditAnywhere)
	//TSoftObjectPtr<ULevelSequence> FadeIn;
	//UPROPERTY(EditAnywhere)
	//TSoftObjectPtr<ULevelSequence> FadeOut;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	//UPROPERTY(EditAnywhere)
	//UActorSequenceComponent* ActorSequenceIn;
	//UPROPERTY(EditAnywhere)
	//UActorSequenceComponent* ActorSequenceOut;

	
	

	FWorldEntitySignature OnFinished;

	UFUNCTION(CallInEditor)
	void FadeHolderInEd();


	UFUNCTION(CallInEditor)
	void FadeHolderOutEd();

	
public:	
	// Sets default values for this actor's properties
	AWorldEntity();

	UFUNCTION(BlueprintImplementableEvent)
	void FadeHolderOutBp();
	UFUNCTION(BlueprintImplementableEvent)
	void FadeHolderInBp();

	UFUNCTION(BlueprintCallable)
	void FadeFinished();

	USceneComponent* GetHolder() { return MechanicManagerHolder; }
	//UFUNCTION(CallInEditor)
	void FadeHolderIn(FWorldEntitySignature Del);
	//UFUNCTION(CallInEditor)
	void FadeHolderOut(FWorldEntitySignature Del);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
