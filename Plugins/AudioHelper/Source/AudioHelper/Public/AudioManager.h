// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AudioHelperStructs.h"
#include "GameplayTagContainer.h"
#include "Subsystems/WorldSubsystem.h"
#include "AudioManager.generated.h"

class UAudioContactComponent;
class UCollisionType;

UCLASS(config = Game, defaultconfig, meta = (DisplayName = "Audio Manager Settings"))
class AUDIOHELPER_API UAudioManagerSettings : public UDeveloperSettings
{
	GENERATED_BODY()

	/// <summary>
	/// Collision Data Asset
	/// </summary>
	UPROPERTY(Config, EditDefaultsOnly)
	TSoftObjectPtr<UCollisionType> CollisionDataAsset;

	public:

	 UCollisionType* GetCollisionDataAsset() const;


};


UCLASS()
class AUDIOHELPER_API UAudioManager : public UWorldSubsystem
{
	GENERATED_BODY()

	UPROPERTY()
	mutable const UAudioManagerSettings* SettingsCache;
	const UAudioManagerSettings* GetSettings() const;

	UPROPERTY()
	UCollisionType* CollisionDataAssetCache;
	UCollisionType* GetCollisionData();

	UPROPERTY()
	//TMap<UAudioContactComponent*, FImpactStruct*> ContactPairCache;
	TArray<FImpactStruct> ContactPairCache;

	//UPROPERTY()
	TMap<FCollisionStruct*, float> LastTriggered;

public:

	void PlayCollisionSound(FGameplayTag TypeA, FGameplayTag TypeB, FVector Pos,FRotator Rot = FRotator::ZeroRotator, float VolMul = 1.f, float PitchMul = 1.f);

	const FCollisionStruct* CollisionStruct(FGameplayTag TypeA, FGameplayTag TypeB);

	//const FCollisionStruct* CollisionStruct(UPhysicalMaterial* TypeA, UPhysicalMaterial* TypeB);

	USoundBase* GetCollisionSound(FGameplayTag TypeA, FGameplayTag TypeB);

	void PlaySound(TSoftObjectPtr<USoundBase> Sound, FVector Pos, FRotator Rot = FRotator::ZeroRotator, float VolMul = 1.f, float PitchMul = 1.f);
	void PlaySound(USoundBase* Sound, FVector Pos, FRotator Rot = FRotator::ZeroRotator, float VolMul = 1.f, float PitchMul = 1.f);

	void AddContactPair(UPrimitiveComponent* A, UPrimitiveComponent* B, FVector Pos, FRotator Rot = FRotator::ZeroRotator, float VolMul = 1.f, float PitchMul = 1.f);

	void CheckContactPairCache();
};
