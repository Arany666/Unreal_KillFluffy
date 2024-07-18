// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttachmentManager.generated.h"

class UAttacmentSlot;
class AXRPawn;
class UXRGrabableComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class XRMANAGER_API UAttachmentManager : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	AXRPawn* XRPawnCache;

	UPROPERTY(EditAnywhere)
	UAttacmentSlot* GrabSlot_L;
	UPROPERTY(EditAnywhere)
	UAttacmentSlot* GrabSlot_R;



public:	
	// Sets default values for this component's properties
	UAttachmentManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:

	void GetHandSlots();

public:

	void SetXRPawn(AXRPawn* XRPawn);

	void AttachToHandSlot(UXRGrabableComponent* Grabable, bool Right);
	UPrimitiveComponent* DetachFromHandSlot(UXRGrabableComponent* Grabable);
};
