// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AudioHelperStructs.generated.h"

class UAudioContactComponent;

USTRUCT(BlueprintType)
struct FCollisionStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FGameplayTag TypeA;
	UPROPERTY(EditAnywhere)
	FGameplayTag TypeB;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere)
	float MinSoundGap = 0.05f;

	//UPROPERTY(VisibleAnywhere,Transient,NonTransactional,SkipSerialization)
	//float LastTime;
};

USTRUCT(BlueprintType)
struct FImpactStruct
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UPrimitiveComponent* A;
	UPROPERTY(VisibleAnywhere)
	UPrimitiveComponent* B;

	FCollisionStruct* Struct;

	UPROPERTY(VisibleAnywhere)
	FVector Pos = FVector::ZeroVector;
	UPROPERTY(VisibleAnywhere)
	FRotator Rot = FRotator::ZeroRotator;
	UPROPERTY(VisibleAnywhere)
	float VolMul = 1.f;
	UPROPERTY(VisibleAnywhere)
	float PitchMul = 1.f;

	bool operator == (const FImpactStruct Rhs) const
	{
		return Rhs.A == A && Rhs.B == B && Rhs.Pos == Pos;
	}

	//FImpactStruct(UAudioContactComponent* OtherComp, const FVector& Position) : Other(OtherComp),Pos(Position)
	//{
	//	//Other = OtherComp;
	//	//Pos = Position;
	//	Rot = FRotator::ZeroRotator;
	//	VolMul = 1.f;
	//	PitchMul = 1.f;
	//}
};
