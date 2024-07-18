// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BaseTarget.h"

#include "ActorSequenceComponent.h"
#include "ActorSequencePlayer.h"
#include "AudioManager.h"
#include "AudioTypeUserData.h"
#include "Core/KillFluffyGm.h"
#include "Core/ProjectileBase.h"
#include "Core/WorldManager.h"
#include "Kismet/GameplayStatics.h"

UActorSequenceComponent* ABaseTarget::GetActorSequence()
{
	if(IsValid(ActorSequenceCache) == false)
	{
		if (UActorComponent* ActorComponent = ActorRef.GetComponent(this))
		{
			if (UActorSequenceComponent* ActorSequence = Cast<UActorSequenceComponent>(ActorComponent))
			{
				ActorSequenceCache = ActorSequence;
			}
		}
	}
	return ActorSequenceCache;
}

UWorldManager* ABaseTarget::GetWorldManager()
{
	if(IsValid(WorldManagerCache) == false)
	{
		if(GetWorld())
		{
			return GetWorld()->GetSubsystem<UWorldManager>();
		}
	}
	return WorldManagerCache;
}

void ABaseTarget::CollectDetachAbles()
{
	if (RootDetachAble)
	{
		TArray<USceneComponent*> Temp;
		RootDetachAble->GetChildrenComponents(true, Temp);

		if (Temp.IsEmpty()) return;

		for (USceneComponent* Component : Temp)
		{
			if (UPrimitiveComponent* MechanicBase = Cast<UPrimitiveComponent>(Component))
			{
				DetachAbles.AddUnique(MechanicBase);
			}
		}
	}
}

void ABaseTarget::BlowDetachAbles()
{
	if(DetachAbles.IsEmpty())
		return;

	if(AKillFluffyGm* Gm = Cast<AKillFluffyGm>(GetWorld()->GetAuthGameMode()))
	{
		for (UPrimitiveComponent* DetachAble : DetachAbles)
		{
			if (IsValid(DetachAble))
			{
				DetachAble->OnComponentHit.AddUniqueDynamic(this, &ABaseTarget::OnPieceComponentHit);
				Gm->AddTargetPiece(DetachAble);
			}
		}
	}
	DetachAbles.Reset();
}

void ABaseTarget::PlaySound(TSoftObjectPtr<USoundBase> Sound, FVector Pos)
{
	UGameplayStatics::PlaySoundAtLocation(this, Sound.LoadSynchronous(), Pos, FRotator::ZeroRotator);
}

// Sets default values
ABaseTarget::ABaseTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	RootNormal = CreateDefaultSubobject<USceneComponent>(TEXT("RootNormal"));
	RootNormal->SetupAttachment(Root);
	RootDetachAble = CreateDefaultSubobject<USceneComponent>(TEXT("RootDetachAble"));
	RootDetachAble->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void ABaseTarget::BeginPlay()
{
	Super::BeginPlay();

	CollectDetachAbles();

	if(IsValid(GetWorldManager()))
		GetWorldManager()->RegisterTarget(this);

	StartCycleInner();
}

void ABaseTarget::OnPieceComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HitComponent->BodyInstance.IsValidBodyInstance())
	{
		if (bKilled && HitComponent->BodyInstance.IsInstanceAwake() && NormalImpulse.Length() > MinImpactVelocity)
			//PlaySound(ImpactSound, Hit.Location);
		{

			if (UMaterialInterface* MaterialInterface = HitComponent->GetMaterial(0))
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



		//Hit.PhysMaterial
		/*UMaterialInterface* MaterialInterface = OtherComp->GetMaterial(0);
		UPhysicalMaterial* PhysicalMaterial = MaterialInterface->GetPhysicalMaterial();*/
	}
	
	
}

// Called every frame
void ABaseTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseTarget::TargetUpdate(float DeltaTime)
{
	if(bKilled) return;

	if(bVulnerable)
	{
		if(CurrentCycleTimeOut < CycleTimeOut)
		{
			CurrentCycleTimeOut += DeltaTime;
		}
		else
		{
			StartCycleInner();
		}
	}
}

void ABaseTarget::CycleFinished()
{
	CurrentActiveCycle++;
	if(CurrentActiveCycle >= ActiveCycle)
	{
		StopCycle();
		CurrentCycleTimeOut =0;
		bVulnerable = true;
		CurrentActiveCycle = 0;
	}
	else
	{
		bVulnerable = false;
		StartCycle();
	}
}

void ABaseTarget::StartCycleInner()
{
	bVulnerable = false;
	if(IsValid(GetActorSequence()) == false) return;
	if(UActorSequencePlayer* Player = GetActorSequence()->GetSequencePlayer())
	{
		Player->Play();
		Player->OnFinished.AddUniqueDynamic(this, &ABaseTarget::CycleFinishedInner);
	}
	
}

void ABaseTarget::StopCycleInner()
{
	CurrentCycleTimeOut = 0;
	bVulnerable = true;
	CurrentActiveCycle = 0;
}

void ABaseTarget::CycleFinishedInner()
{
	CurrentActiveCycle++;
	if (CurrentActiveCycle >= ActiveCycle)
	{
		StopCycleInner();
	}
	else
	{
		StartCycleInner();
	}
}

void ABaseTarget::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if(IsValid(GetWorldManager()))
		GetWorldManager()->DeRegisterTarget(this);
}

void ABaseTarget::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if(AProjectileBase* Projectile = Cast<AProjectileBase>(Other))
	{
		if(Projectile->IsKilled() == false)
		{
			if(bVulnerable)
			{
				Projectile->SetKilled();

				BlowDetachAbles();
				bKilled = true;

				PlaySound(BlowSound, HitLocation);

				if (IsValid(GetWorldManager()))
				{
					if (GetWorldManager()->TargetGotHit.IsBound())
						GetWorldManager()->TargetGotHit.Broadcast(this, Projectile);

				}
			}
		}
	}
	//if(bKilled)
	//	PlaySound(ImpactSound, HitLocation);
	if (UMaterialInterface* MaterialInterface = MyComp->GetMaterial(0))
	{
		const UAudioTypeUserData* AudioTypeUserData = MaterialInterface->GetAssetUserData<UAudioTypeUserData>();
		if (AudioTypeUserData)
		{
			if (GetWorld())
			{
				if (UAudioManager* Manager = GetWorld()->GetSubsystem<UAudioManager>())
				{
					Manager->AddContactPair(MyComp, OtherComp, Hit.Location);
				}
			}
		}
	}
}

