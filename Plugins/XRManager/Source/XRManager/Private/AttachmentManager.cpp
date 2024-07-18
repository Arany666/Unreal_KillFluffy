// Fill out your copyright notice in the Description page of Project Settings.


#include "AttachmentManager.h"

#include "XRPawn.h"
#include "XRMCHandComponent.h"
#include "AttacmentSlot.h"
#include "XRGrabableComponent.h"

// Sets default values for this component's properties
UAttachmentManager::UAttachmentManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttachmentManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAttachmentManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttachmentManager::GetHandSlots()
{
	if (IsValid(XRPawnCache) == false)
		return;

	GrabSlot_L = XRPawnCache->GetMC_Grip_L()->GetMainSlot();
	GrabSlot_R = XRPawnCache->GetMC_Grip_R()->GetMainSlot();
}

void UAttachmentManager::SetXRPawn(AXRPawn* XRPawn)
{
	XRPawnCache = XRPawn;
	GetHandSlots();
}

void UAttachmentManager::AttachToHandSlot(UXRGrabableComponent* Grabable, bool Right)
{
	if (IsValid(Grabable) == false)
		return;

	UAttacmentSlot* Slot = Right ? GrabSlot_R : GrabSlot_L;

	if (IsValid(Slot) == false) 
		return;

	if (AActor* GrabableActor = Grabable->GetAttachParentActor()) 
	{
		if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(GrabableActor->GetComponentByClass(UPrimitiveComponent::StaticClass())))
		{
			Primitive->SetSimulatePhysics(false);
			Primitive->SetEnableGravity(false);
		}
	}

	if (USceneComponent* SceneComp = Grabable->GetAttachParent())
	{
		FAttachmentTransformRules Rules = FAttachmentTransformRules::KeepWorldTransform;
		Rules.LocationRule = EAttachmentRule::KeepWorld;
		Rules.RotationRule = EAttachmentRule::KeepWorld;
		Rules.bWeldSimulatedBodies = true;
		SceneComp->AttachToComponent(Slot, Rules);
	}
}

UPrimitiveComponent* UAttachmentManager::DetachFromHandSlot(UXRGrabableComponent* Grabable)
{
	if (IsValid(Grabable) == false)
		return nullptr;

	if (USceneComponent* SceneComp = Grabable->GetAttachParent())
	{
		FDetachmentTransformRules Rules = FDetachmentTransformRules::KeepWorldTransform;
		SceneComp->DetachFromComponent(Rules);
	}

	if (AActor* GrabableActor = Grabable->GetAttachParentActor())
	{
		if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(GrabableActor->GetComponentByClass(UPrimitiveComponent::StaticClass())))
		{
			Primitive->SetSimulatePhysics(true);
			Primitive->SetEnableGravity(true);

			return Primitive;
		}
	}
	return nullptr;
}

