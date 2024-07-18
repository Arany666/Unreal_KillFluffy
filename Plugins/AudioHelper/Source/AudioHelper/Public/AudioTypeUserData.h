// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/AssetUserData.h"
#include "AudioTypeUserData.generated.h"

/**
 * 
 */
UCLASS()
class AUDIOHELPER_API UAudioTypeUserData : public UAssetUserData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FGameplayTag AudioType;

public:
	const FGameplayTag& GetAudioType() const { return AudioType; }
};
