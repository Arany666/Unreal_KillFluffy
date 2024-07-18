// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MechanicBase.h"
#include "Core/FluffyStructs.h"
//#include "Mechanics/MachanicBase.h"
#include "Mechanic.generated.h"

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable, ClassGroup=(Mechanics), meta=(BlueprintSpawnableComponent))
class KILLFLUFFY_API UMechanic : public UMechanicBase
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere)
	float RoundTimeMultiplier = 1.f;
	
	UPROPERTY(EditAnywhere)
	bool Loop;


	UPROPERTY(EditAnywhere,Transient)
	TArray<UMechanicBase*> MechanicsCache;

	void CollectMechanics();
	void SetupMechanicsInner();

	void UpdateAllPosition();

protected:
	
	
	virtual void UpdateMechanicPosition(UMechanicBase* Mechanic);

public:
	virtual void SetupMechanic() override;

	void TimerUpdate(float DeltaTime, float FullRoundTime );
};
