// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/WorldEntity.h"
#include "LevelSequence.h"
#include "ActorSequencePlayer.h"
#include "ActorSequenceComponent.h"
//#include "CutSceneManager.h"

UCutSceneManager* AWorldEntity::GetCutSceneManager()
{
	if(IsValid(CutSceneManagerCache) == false)
	{
		if(GetWorld())
		{
			CutSceneManagerCache = GetWorld()->GetSubsystem<UCutSceneManager>();
		}
	}
	return CutSceneManagerCache;
}

void AWorldEntity::FadeFinished()
{
	OnFinished.ExecuteIfBound();
}

void AWorldEntity::FadeHolderInEd()
{
	FWorldEntitySignature Del;
	FadeHolderIn(Del);
}

void AWorldEntity::FadeHolderOutEd()
{
	FWorldEntitySignature Del;
	FadeHolderOut(Del);
}

// Sets default values
AWorldEntity::AWorldEntity()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	MechanicManagerHolder = CreateDefaultSubobject<USceneComponent>(TEXT("MechanicManagerHolder"));
	MechanicManagerHolder->SetupAttachment(Root);


	//ActorSequenceIn = CreateDefaultSubobject<UActorSequenceComponent>(TEXT("ActorSequenceIn"));
	//ActorSequenceOut = CreateDefaultSubobject<UActorSequenceComponent>(TEXT("ActorSequenceOut"));

	//ActorSequenceIn->SetFlags(RF_ClassDefaultObject);
	//ActorSequenceOut->SetFlags(RF_ClassDefaultObject);
}



void AWorldEntity::FadeHolderIn(FWorldEntitySignature Del)
{
	OnFinished = std::move(Del);
	FadeHolderInBp();
	//if(ActorSequenceIn)
	//{
	//	if(UActorSequencePlayer* SequencePlayer = ActorSequenceIn->GetSequencePlayer())
	//	{
	//		if(UMovieSceneSequencePlayer* Player = Cast<UMovieSceneSequencePlayer>(SequencePlayer))
	//		{
	//			Player->OnFinished.RemoveAll(this);
	//			Player->OnFinished.AddUniqueDynamic(this, &AWorldEntity::FadeFinished);
	//			ActorSequenceIn->PlaySequence();
	//			return;
	//		}
	//	}
	//}
	//FadeFinished();
	

	//if(IsValid(GetCutSceneManager()) && FadeIn.IsNull() == false)
	//{
	//	GetCutSceneManager()->PlayCutScene(FadeIn.LoadSynchronous(), Del, false);
	//}
	//else
	//{
	//	Del.ExecuteIfBound();
	//}
}

void AWorldEntity::FadeHolderOut(FWorldEntitySignature Del)
{
	OnFinished = std::move(Del);
	FadeHolderOutBp();


	//if (ActorSequenceOut)
	//{
	//	if (UActorSequencePlayer* SequencePlayer = ActorSequenceOut->GetSequencePlayer())
	//	{
	//		if (UMovieSceneSequencePlayer* Player = Cast<UMovieSceneSequencePlayer>(SequencePlayer))
	//		{
	//			Player->OnFinished.RemoveAll(this);
	//			Player->OnFinished.AddUniqueDynamic(this, &AWorldEntity::FadeFinished);
	//			ActorSequenceOut->PlaySequence();
	//			return;
	//		}
	//	}
	//}
	//FadeFinished();

	//if (IsValid(GetCutSceneManager()) && FadeOut.IsNull() == false)
	//{
	//	GetCutSceneManager()->PlayCutScene(FadeOut.LoadSynchronous(), Del,false);
	//}
	//else
	//{
	//	Del.ExecuteIfBound();
	//}
}

// Called when the game starts or when spawned
void AWorldEntity::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWorldEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

