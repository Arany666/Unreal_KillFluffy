// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AudioHelperStructs.h"
#include "Engine/DataAsset.h"
#include "CollisionType.generated.h"

/**
 * 
 */
UCLASS()
class AUDIOHELPER_API UCollisionType : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<FCollisionStruct> CollisionStructs;

public:

	const FCollisionStruct* GetCollisionStructs(FGameplayTag TypeA, FGameplayTag TypeB) const;
	//const FCollisionStruct* GetCollisionStructs(UPhysicalMaterial* TypeA, UPhysicalMaterial* TypeB) const;

};
