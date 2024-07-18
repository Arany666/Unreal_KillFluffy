// Fill out your copyright notice in the Description page of Project Settings.


#include "Mechanics/MechanicManager.h"

#include "Core/BaseTarget.h"
#include "Core/WorldManager.h"
#include "Mechanics/Mechanic.h"
#include "Mechanics/MechanicBase.h"

void AMechanicManager::CollectMechanics()
{
	if( Root)
	{
		TArray<USceneComponent*> Temp;
		Root->GetChildrenComponents(false,Temp);
		
		if(Temp.IsEmpty()) return;
		
		for (USceneComponent* Component : Temp)
		{
			if(UMechanicBase* MechanicBase = Cast<UMechanicBase>(Component))
			{
				MechanicsCache.AddUnique(MechanicBase);
			}
		}
	}
}

void AMechanicManager::SetupMechanics()
{
	if(MechanicsCache.IsEmpty()) return;
	for (UMechanicBase* Mechanic : MechanicsCache)
	{
		if(IsValid(Mechanic))
			Mechanic->SetupMechanic();
	}
}

void AMechanicManager::CollectTargets()
{
	TArray<AActor*> Temp;
	GetAllChildActors(Temp, true);
	if (Temp.IsEmpty()) return;

	for (AActor* Actor : Temp)
	{
		if (ABaseTarget* Target = Cast<ABaseTarget>(Actor))
		{
			TargetCache.AddUnique(Target);

		}
	}
	if (GetWorldManager())
		GetWorldManager()->TargetGotHit.AddUniqueDynamic(this, &AMechanicManager::OnTargetGotHit);
}

bool AMechanicManager::AllTargetKilled()
{
	if (TargetCache.IsEmpty()) return false;

	for (ABaseTarget* Target : TargetCache)
	{
		if(Target->IsKilled() == false)
			return false;
	}
	return true;
}

UWorldManager* AMechanicManager::GetWorldManager()
{
	if (IsValid(WorldManagerCache) == false)
	{
		if (GetWorld())
		{
			return GetWorld()->GetSubsystem<UWorldManager>();
		}
	}
	return WorldManagerCache;
}

void AMechanicManager::OnTargetGotHit(ABaseTarget* Target, AProjectileBase* Projectile)
{
	if(AllTargetKilled())
	{
		if (GetWorldManager() && GetWorldManager()->MechanicManagerAllTargetsDestroyed.IsBound())
			GetWorldManager()->MechanicManagerAllTargetsDestroyed.Broadcast(this);

	}
}

void AMechanicManager::TimerUpdate(float DeltaTime)
{
	if(MechanicsCache.IsEmpty() == false)
	{
		for (int a = 0; a < MechanicsCache.Num(); ++a)
		{
			if (UMechanic* Mechanic = Cast<UMechanic>(MechanicsCache[a]))
			{
				Mechanic->TimerUpdate(DeltaTime, RoundTime);
			}
		}
	}
	if (TargetCache.IsEmpty() == false)
	{
		for (int a = 0; a < TargetCache.Num(); ++a)
		{
			if (ABaseTarget* Target = TargetCache[a])
			{
				Target->TargetUpdate(DeltaTime);
			}
		}
	}

}

// Sets default values
AMechanicManager::AMechanicManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
}

// Called when the game starts or when spawned
void AMechanicManager::BeginPlay()
{
	Super::BeginPlay();
	SetupMechanicManager();	
}

// Called every frame
void AMechanicManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimerUpdate(DeltaTime);
}

void AMechanicManager::SetupMechanicManager()
{
	CollectMechanics();
	SetupMechanics();
	CollectTargets();
}

