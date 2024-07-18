// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioContactComponent.h"

#include "AudioManager.h"
#include "AudioTypeUserData.h"

// Sets default values for this component's properties
UAudioContactComponent::UAudioContactComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UAudioContactComponent::BeginPlay()
{
	Super::BeginPlay();

	if(USceneComponent* ParentComp = GetAttachParent())
	{
		if(UPrimitiveComponent * Comp = Cast<UPrimitiveComponent>(ParentComp))
		{
			Comp->OnComponentHit.AddUniqueDynamic(this, &UAudioContactComponent::OnComponentHit);
		}
	}
}

void UAudioContactComponent::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UAudioContactComponent* OtherAudioComp = nullptr;
	//TArray<USceneComponent*> Comps;
	//OtherComp->GetChildrenComponents(false, Comps);
	//for (USceneComponent* Comp : Comps)
	//{
	//	if(UAudioContactComponent * AudioComp = Cast<UAudioContactComponent>(Comp))
	//	{
	//		OtherAudioComp = AudioComp;
	//		break;
	//	}
	//}
	//if(IsValid(OtherAudioComp) == false)
	//{
	//	if(AActor* ParentActor =OtherComp->GetAttachParentActor())
	//	{
	//		if(USceneComponent* Root = ParentActor->GetRootComponent())
	//		{
	//			TArray<USceneComponent*> CompsParent;
	//			Root->GetChildrenComponents(false, CompsParent);
	//			for (USceneComponent* Comp : CompsParent)
	//			{
	//				if (UAudioContactComponent* AudioComp = Cast<UAudioContactComponent>(Comp))
	//				{
	//					OtherAudioComp = AudioComp;
	//					break;
	//				}
	//			}
	//		}
	//	}
	//}
	//if (IsValid(OtherAudioComp) == false) return;

	if (UMaterialInterface* MaterialInterface = HitComponent ->GetMaterial(0))
	{
		const UAudioTypeUserData* AudioTypeUserData = MaterialInterface->GetAssetUserData<UAudioTypeUserData>();
		if (AudioTypeUserData)
		{
			if (GetWorld())
			{
				if (UAudioManager* Manager = GetWorld()->GetSubsystem<UAudioManager>())
				{
					Manager->AddContactPair(HitComponent, OtherComp, Hit.Location);
				}
			}
		}
	}

}


