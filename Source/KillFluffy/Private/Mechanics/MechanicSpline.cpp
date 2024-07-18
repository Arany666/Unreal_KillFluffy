// Fill out your copyright notice in the Description page of Project Settings.


#include "Mechanics/MechanicSpline.h"

#include "Mechanics/AdvancedSplineComponent.h"

UMechanicSpline::UMechanicSpline()
{
	AdvancedSpline = CreateDefaultSubobject<UAdvancedSplineComponent>(TEXT("AdvancedSpline"));
	AdvancedSpline->SetupAttachment(this);
}

void UMechanicSpline::UpdateMechanicPosition(UMechanicBase* Mechanic)
{
	Super::UpdateMechanicPosition(Mechanic);

	if(!AdvancedSpline) return;
	float SplinePos = AdvancedSpline->GetSplineLength() * CurrentPositionFixed;

	//FVector Pos = AdvancedSpline->GetWorldLocationAtDistanceAlongSpline(CurrentPositionFixed);
	FVector Pos = AdvancedSpline->GetLocationAtDistanceAlongSpline(SplinePos,ESplineCoordinateSpace::Local);
	//FVector Dir = AdvancedSpline->GetWorldDirectionAtDistanceAlongSpline(CurrentPositionFixed);
	FVector Dir = AdvancedSpline->GetDirectionAtDistanceAlongSpline(SplinePos,ESplineCoordinateSpace::Local);

	//BallPos.Ball->SetActorLocationAndRotation(Pos,Rot);
	Mechanic->SetRelativeLocation(Pos);
	if(SubRotation)
	{
		if(Reverse)
			Dir *= -1.f;
		FRotator Rot = Dir.Rotation();
		Mechanic->SetRelativeRotation(Rot);
	}
}
