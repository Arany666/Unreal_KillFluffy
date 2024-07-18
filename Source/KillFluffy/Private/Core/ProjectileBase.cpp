// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ProjectileBase.h"

#include "AudioManager.h"
#include "AudioTypeUserData.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UPrimitiveComponent* AProjectileBase::GetComponentCache()
{
	if(IsValid(ComponentCache) == false)
	{
		if( RootComponent)
		{
			TArray<USceneComponent*> Temp;
			RootComponent->GetChildrenComponents(false,Temp);
			
			if(Temp.IsEmpty() == false)
			{
				for (USceneComponent* Component : Temp)
				{
					if(UPrimitiveComponent* MechanicBase = Cast<UPrimitiveComponent>(Component))
					{
						ComponentCache = MechanicBase;
						break;
					}
				}
			}
		}
	}
	return ComponentCache;
}

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	RootComponent = SkeletalMeshComponent;
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
}

void AProjectileBase::SetupProjectile()
{
	
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	SkeletalMeshComponent->OnComponentHit.AddUniqueDynamic(this, &AProjectileBase::OnComponentHit);
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::SetProjectileVelocity(FVector& Velocity,float MassRef)
{

	VelocityCache = Velocity;
	if(SkeletalMeshComponent)
	{
		FVector Pos;
		FVector Extent;
		GetActorBounds(true, Pos, Extent);
		FVector Random = FVector(FMath::RandRange(-.2f, .2f), FMath::RandRange(-.2f, .2f), FMath::RandRange(-.2f, .2f));
		//Pos += FVector(20.f, 30.f, 20.f);
		Pos += Random;
		SkeletalMeshComponent->BodyInstance.AddVelocityChangeImpulseAtLocation(Velocity, Pos);
		for (FBodyInstance* BodyInstance : SkeletalMeshComponent->Bodies)
		{
			BodyInstance->AddVelocityChangeImpulseAtLocation(Velocity, Pos);
		}
	}

}

void AProjectileBase::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	SetKilled();

	//AudioComponent->Play();

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

	
	
	
	
	
	
	
	
	
	
	
	
	
	
}

void AProjectileBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	SetKilled();
}

void AProjectileBase::OnComponentHit(/*UPrimitiveComponent OnComponentHit,*/ UPrimitiveComponent* HitComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	SetKilled();
	//if (HitComponent->BodyInstance.IsValidBodyInstance())
	{
		if (/*HitComponent->BodyInstance.IsInstanceAwake() &&*/ NormalImpulse.Length() > MinImpactVelocity)
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
	}
}



