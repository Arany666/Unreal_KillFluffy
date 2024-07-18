// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mechanics/Mechanic.h"
#include "MechanicRotator.generated.h"

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable, ClassGroup=(Mechanics), meta=(BlueprintSpawnableComponent))
class KILLFLUFFY_API UMechanicRotator : public UMechanic
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float OrigoDistance = 50;

	UPROPERTY(EditAnywhere,Meta = (MakeEditWidget = true))
	FRotator RotAtionA;
	UPROPERTY(EditAnywhere,Meta = (MakeEditWidget = true))
	FRotator RotAtionB;

	UPROPERTY(EditAnywhere)
	bool SubRotation;

	FVector RotatePointAroundPivot(const FVector Point, const FVector Pivot, const FRotator Rotation);

protected:
	virtual void UpdateMechanicPosition(UMechanicBase* Mechanic) override;
};
