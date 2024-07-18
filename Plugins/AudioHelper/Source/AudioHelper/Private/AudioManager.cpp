// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioManager.h"

#include "AudioContactComponent.h"
#include "AudioTypeUserData.h"
#include "CollisionType.h"
#include "Kismet/GameplayStatics.h"

UCollisionType* UAudioManagerSettings::GetCollisionDataAsset() const
{
	if (CollisionDataAsset.IsNull()) return nullptr;

	return CollisionDataAsset.LoadSynchronous();
}

const UAudioManagerSettings* UAudioManager::GetSettings() const
{
	if(IsValid(SettingsCache) == false)
	{
		SettingsCache = GetDefault<UAudioManagerSettings>();
	}
	return SettingsCache;
}

UCollisionType* UAudioManager::GetCollisionData()
{
	if (IsValid(CollisionDataAssetCache) == false)
	{
		const UAudioManagerSettings* Settings = GetDefault<UAudioManagerSettings>();
		if(IsValid(Settings))
		{
			CollisionDataAssetCache = Settings->GetCollisionDataAsset();
		}
	}
	return CollisionDataAssetCache;
}

void UAudioManager::PlayCollisionSound(FGameplayTag TypeA, FGameplayTag TypeB, FVector Pos, FRotator Rot, float VolMul, float PitchMul)
{
	if(USoundBase* Sound = GetCollisionSound(TypeA,TypeB))
	{
		PlaySound(Sound, Pos, Rot, VolMul, PitchMul);
	}
}

const FCollisionStruct* UAudioManager::CollisionStruct(FGameplayTag TypeA, FGameplayTag TypeB)
{
	if (IsValid(GetCollisionData()))
	{
		return GetCollisionData()->GetCollisionStructs(TypeA, TypeB);
	}
	return nullptr;
}

//const FCollisionStruct* UAudioManager::CollisionStruct(UPhysicalMaterial* TypeA, UPhysicalMaterial* TypeB)
//{
//	if (IsValid(GetCollisionData()))
//	{
//		//return GetCollisionData()->GetCollisionStructs(TypeA, TypeB);
//	}
//	return nullptr;
//}

USoundBase* UAudioManager::GetCollisionSound(FGameplayTag TypeA, FGameplayTag TypeB)
{
	if(IsValid(GetCollisionData()))
	{
		//if(const FCollisionStruct* Struct = CollisionStruct(TypeA,TypeA))
		//{
		//	return Struct->ImpactSound.LoadSynchronous();
		//}
		if(const FCollisionStruct* Struct = GetCollisionData()->GetCollisionStructs(TypeA, TypeB))
		{
			return Struct->ImpactSound.LoadSynchronous();
		}
	}
	return nullptr;
}

void UAudioManager::PlaySound(TSoftObjectPtr<USoundBase> Sound, FVector Pos, FRotator Rot, float VolMul, float PitchMul)
{
	if(Sound.IsNull()) return;

	PlaySound(Sound.LoadSynchronous(), Pos, Rot, VolMul, PitchMul);
}

void UAudioManager::PlaySound(USoundBase* Sound, FVector Pos, FRotator Rot, float VolMul, float PitchMul)
{
	if (IsValid(Sound) == false) return;

	UGameplayStatics::PlaySoundAtLocation(this, Sound, Pos, Rot,VolMul,PitchMul);
}

void UAudioManager::AddContactPair(UPrimitiveComponent* A, UPrimitiveComponent* B, FVector Pos, FRotator Rot, float VolMul, float PitchMul)
{
	FGameplayTag TypeA;
	FGameplayTag TypeB;

	if(UMaterialInterface* MaterialInterface = A->GetMaterial(0))
	{
		const UAudioTypeUserData* AudioTypeUserData = MaterialInterface->GetAssetUserData<UAudioTypeUserData>();
		if (AudioTypeUserData)
			TypeA = AudioTypeUserData->GetAudioType();
	}
	if (UMaterialInterface* MaterialInterface = B->GetMaterial(0))
	{
		const UAudioTypeUserData* AudioTypeUserData = MaterialInterface->GetAssetUserData<UAudioTypeUserData>();
		if (AudioTypeUserData)
			TypeB = AudioTypeUserData->GetAudioType();
	}

	if(!TypeA.IsValid() || !TypeB.IsValid()) return;


	const FCollisionStruct* Struct = CollisionStruct(TypeA, TypeB);

	if(!Struct) return;

	FImpactStruct Temp = FImpactStruct();
	Temp.A = A;
	Temp.B = B;
	Temp.Pos = Pos;
	Temp.Struct = const_cast<FCollisionStruct*>( Struct);

	if(ContactPairCache.Contains(Temp)) return;

	ContactPairCache.AddUnique(Temp);

	CheckContactPairCache();
}

void UAudioManager::CheckContactPairCache()
{
	if(ContactPairCache.IsEmpty()) return;

	double Current = UGameplayStatics::GetTimeSeconds(GetWorld());

	for (FImpactStruct& ImpactStruct : ContactPairCache)
	{
		if(!ImpactStruct.A || !ImpactStruct.B || !ImpactStruct.Struct) continue;
		
		if(LastTriggered.Contains(ImpactStruct.Struct) == false)
		{
			LastTriggered.Add(ImpactStruct.Struct);
		}
		if (LastTriggered.Contains(ImpactStruct.Struct) )
		{
			
			if (LastTriggered[ImpactStruct.Struct] + ImpactStruct.Struct->MinSoundGap < Current)
			{
				LastTriggered[ImpactStruct.Struct] = Current;

				PlaySound(ImpactStruct.Struct->ImpactSound, ImpactStruct.Pos, ImpactStruct.Rot, ImpactStruct.VolMul, ImpactStruct.PitchMul);
			}
		}
		
	}
	ContactPairCache.Reset();

}
