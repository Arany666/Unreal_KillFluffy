// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Core/FluffyStructs.h"
#include "MechanicBase.generated.h"


class UMechanicStepObject;

UCLASS(BlueprintType,Blueprintable, ClassGroup=(Mechanics), meta=(BlueprintSpawnableComponent) )
class KILLFLUFFY_API UMechanicBase : public USceneComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,meta=(ClampMin = "0.0", ClampMax = "1.0",UIMin = "0.0",UIMax = "1.0"))
	float InitialPosition;

	UPROPERTY(EditAnywhere)
	TArray<FMechanicStep> Steps;
	UPROPERTY(VisibleAnywhere, Transient)
	int StepId;

	//UPROPERTY(EditAnywhere,Instanced,Export)
	//UPROPERTY(EditAnywhere,EditInline,Export)
	UPROPERTY(EditAnywhere, meta = (FullyExpand = true),Export)
	UMechanicStepObject* Test;

	
	

public:	
	// Sets default values for this component's properties
	UMechanicBase();

	UFUNCTION(CallInEditor)
	void FillTest();

protected:
	//UPROPERTY(EditAnywhere)
	//USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	bool InitialDirectionReverse = false;

	
	

public:
	UPROPERTY(VisibleAnywhere,Transient)
	float CurrentPosition;

	UPROPERTY(VisibleAnywhere,Transient)
	bool Reverse;
	UPROPERTY(VisibleAnywhere,Transient)
	float CurrentPositionFixed;
	
	UPROPERTY(VisibleAnywhere,Transient)
	bool DirectionReverse;

	FMechanicStep* CurrentStep();
	void NextStepId();

	float GetInitialPosition(){return InitialPosition;}

	virtual void SetupMechanic();
};
