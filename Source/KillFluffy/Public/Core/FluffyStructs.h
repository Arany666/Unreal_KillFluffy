// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FluffyStructs.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
	GS_None,
	GS_Setup,
	GS_Start,
	GS_Ingame,
	GS_Rewrad,
	GS_End
};

USTRUCT(BlueprintType)
struct FMechanicStep
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	bool Delay;

	UPROPERTY(EditAnywhere)
	float Step;
	UPROPERTY(VisibleAnywhere)
	float CurrentStep;
};
