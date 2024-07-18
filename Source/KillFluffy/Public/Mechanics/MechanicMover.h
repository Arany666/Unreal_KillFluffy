// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mechanics/Mechanic.h"
#include "MechanicMover.generated.h"

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable, ClassGroup=(Mechanics), meta=(BlueprintSpawnableComponent))
class KILLFLUFFY_API UMechanicMover : public UMechanic
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,Meta = (MakeEditWidget = true))
	FTransform EndPosition;

	

protected:
	virtual void UpdateMechanicPosition(UMechanicBase* Mechanic) override;
};
