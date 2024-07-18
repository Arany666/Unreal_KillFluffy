// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mechanics/Mechanic.h"
#include "MechanicSpline.generated.h"

class UAdvancedSplineComponent;
/**
 * 
 */
UCLASS(BlueprintType,Blueprintable, ClassGroup=(Mechanics), meta=(BlueprintSpawnableComponent))
class KILLFLUFFY_API UMechanicSpline : public UMechanic
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	bool SubRotation;

public:
	UMechanicSpline();
protected:
	UPROPERTY(EditAnywhere)
	UAdvancedSplineComponent* AdvancedSpline;

	virtual void UpdateMechanicPosition(UMechanicBase* Mechanic) override;
};
