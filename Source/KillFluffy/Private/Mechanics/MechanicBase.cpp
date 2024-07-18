// Fill out your copyright notice in the Description page of Project Settings.


#include "Mechanics/MechanicBase.h"

#include "Mechanics/MechanicStepObject.h"

void UMechanicBase::FillTest()
{
	Test = NewObject<UMechanicStepObject>(this, UMechanicStepObject::StaticClass());
}

// Sets default values for this component's properties
UMechanicBase::UMechanicBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	//Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//Root->SetupAttachment(this);
	// ...
}

FMechanicStep* UMechanicBase::CurrentStep()
{
	if (Steps.IsValidIndex(StepId))
	{
		return &Steps[StepId];
	}
	return nullptr;
}

void UMechanicBase::NextStepId()
{
	StepId++;
	if (Steps.IsValidIndex(StepId) == false)
		StepId = 0;
	if(Steps.IsEmpty()) return;
	for (FMechanicStep& Step : Steps)
	{
		Step.CurrentStep = 0;
	}
}

void UMechanicBase::SetupMechanic()
{
	CurrentPosition = InitialPosition;
	DirectionReverse = InitialDirectionReverse;
}


