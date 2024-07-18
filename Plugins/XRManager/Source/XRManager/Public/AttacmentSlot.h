// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "XTSructs.h"

#include "AttacmentSlot.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class XRMANAGER_API UAttacmentSlot : public USceneComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EAttachmentSlotType SlotType;

public:	
	// Sets default values for this component's properties
	UAttacmentSlot();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
	EAttachmentSlotType& GetSlotType() { return SlotType; }
};
