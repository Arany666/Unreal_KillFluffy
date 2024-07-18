// Fill out your copyright notice in the Description page of Project Settings.


#include "XRGrabableComponent.h"

#include "MotionControllerComponent.h"

// Sets default values for this component's properties
UXRGrabableComponent::UXRGrabableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UXRGrabableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UXRGrabableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UXRGrabableComponent::CheckGrab(UMotionControllerComponent* MCC, bool Started)
{
	if (IsValid(MCC) == false)
		return;


}

