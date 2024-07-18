// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ViewTarget.h"

#include "Components/BoxComponent.h"
#include "GameFramework/PlayerStart.h"

#if WITH_EDITOR

void AViewTarget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if(BoxComp)
	{
		BoxComp->SetBoxExtent(FVector::OneVector);
		BoxComp->SetRelativeScale3D(FVector(5,Range.X,Range.Y));
		BoxComp->SetRelativeLocation(FVector(0,0,0));
	}
}

#endif


// Sets default values
AViewTarget::AViewTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	PlayerPosition = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerPosition"));
	PlayerPosition->SetupAttachment(Root);

	#if WITH_EDITOR


	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(Root);

	#endif


}

FTransform AViewTarget::GetPlayerPosition()
{
	if(PlayerPosition)
		return PlayerPosition->GetComponentTransform();
	return FTransform();
}


//APlayerStart* AViewTarget::GetPlayerPosition()
//{
//	if(IsValid(PlayerPositionCache) == false)
//	{
//		TArray<AActor*> Temp;
//		GetAllChildActors(Temp,true);
//		
//		for (AActor* Actor : Temp)
//		{
//			if(APlayerStart* Start = Cast<APlayerStart>(Actor))
//			{
//				PlayerPositionCache = Start;
//				break;
//			}
//		}
//	}
//	return PlayerPositionCache;
//}


