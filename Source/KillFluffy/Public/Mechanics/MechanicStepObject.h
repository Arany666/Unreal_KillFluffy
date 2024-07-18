// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MechanicStepObject.generated.h"

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable,  meta = (BlueprintSpawnableComponent))
class KILLFLUFFY_API UMechanicStepObject : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString NAme;
	
};
