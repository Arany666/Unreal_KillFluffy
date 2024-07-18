// Fill out your copyright notice in the Description page of Project Settings.


#include "Mechanics/MechanicRotator.h"

#include "Kismet/KismetMathLibrary.h"

FVector UMechanicRotator::RotatePointAroundPivot(const FVector Point, const FVector Pivot, const FRotator Rotation)
{
	FTransform Center = FTransform(Pivot);
	FVector inversePos =Center.InverseTransformPosition(Point);
	FVector RotVec = Rotation.UnrotateVector(inversePos);
	return  Center.TransformPosition(RotVec);
}

void UMechanicRotator::UpdateMechanicPosition(UMechanicBase* Mechanic)
{
	Super::UpdateMechanicPosition(Mechanic);
	
	FRotator Rot = UKismetMathLibrary::RLerp(RotAtionA,RotAtionB,CurrentPositionFixed,false);
	FVector Pos = RotatePointAroundPivot(FVector::UpVector * OrigoDistance, FVector::Zero(),Rot);

	Mechanic->SetRelativeLocation(Pos);
	if(SubRotation)
		Mechanic->SetRelativeRotation(Rot * -1);
}
