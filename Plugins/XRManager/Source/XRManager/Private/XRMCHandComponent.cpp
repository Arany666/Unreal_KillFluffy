// Fill out your copyright notice in the Description page of Project Settings.


#include "XRMCHandComponent.h"
#include "AttacmentSlot.h"


void UXRMCHandComponent::UpdateVelocityCache()
{
	if (VelocityCache.Num() <= VelocityCacheIt)
		VelocityCache.Add(FVector::ZeroVector);

	VelocityCache[VelocityCacheIt] = GetPhysicsLinearVelocity();

	VelocityCacheIt++;
	if (VelocityCacheIt >= VelocityCacheCount)
		VelocityCacheIt = 0;
}

void UXRMCHandComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateVelocityCache();
}

FVector UXRMCHandComponent::GetVelocityAvrg()
{
	FVector Avrg = FVector::ZeroVector;

	for (size_t i = 0; i < VelocityCache.Num(); i++)
	{
		Avrg += VelocityCache[i];
	}

	Avrg /= VelocityCacheCount;
	return Avrg;
}

UXRMCHandComponent::UXRMCHandComponent()
{
	//MainSlot = CreateDefaultSubobject<UAttacmentSlot>(TEXT("MainSlot"));
	//MainSlot->SetupAttachment(this);
}

UAttacmentSlot* UXRMCHandComponent::GetMainSlot()
{
	if (IsValid(MainSlot) == false) 
	{
		MainSlot = NewObject<UAttacmentSlot>(this);
		MainSlot->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		MainSlot->RegisterComponent();
	}
	return MainSlot;
}
