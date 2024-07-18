// Fill out your copyright notice in the Description page of Project Settings.


#include "Mechanics/MechanicMover.h"

#include "Kismet/KismetMathLibrary.h"

void UMechanicMover::UpdateMechanicPosition(UMechanicBase* Mechanic)
{
	Super::UpdateMechanicPosition(Mechanic);

	FVector End = EndPosition.GetLocation();
	FVector LocalPos = UKismetMathLibrary::VLerp(FVector::Zero(),End,CurrentPositionFixed);

	Mechanic->SetRelativeLocation(LocalPos);
}
