// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XRMCComponent.h"
#include "XRMCHandComponent.generated.h"

class UAttacmentSlot;

/**
 * 
 */
UCLASS()
class XRMANAGER_API UXRMCHandComponent : public UXRMCComponent
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere)
	UAttacmentSlot* MainSlot;


	UPROPERTY(EditAnywhere)
	int VelocityCacheCount = 5;
	UPROPERTY(VisibleAnywhere)
	int VelocityCacheIt;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> VelocityCache;

	void UpdateVelocityCache();

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	

	FVector GetVelocityAvrg();

	UXRMCHandComponent();

	UAttacmentSlot* GetMainSlot();
	
};
