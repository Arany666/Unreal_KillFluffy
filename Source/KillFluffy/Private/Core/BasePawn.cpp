// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BasePawn.h"

#include "EngineUtils.h"
#include "EnhancedInputSubsystems.h"
#include "Core/KFEnhancedInputComponent.h"

#include "Core/KFEnhancedInputComponent.h"
#include "Core/ProjectileBase.h"
#include "Core/ViewTarget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AViewTarget* ABasePawn::GetViewTarget()
{
	if(IsValid(ViewTargetCahe) == false)
	{
		for (TActorIterator<AViewTarget> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			if(AViewTarget* Interactable = *ActorItr)
			{
				ViewTargetCahe = Interactable;
				//SetViewTargetCenter(ViewTargetCahe);
				break;
			}
		}
	}
	return ViewTargetCahe;
}

void ABasePawn::SetViewTargetCenter(AViewTarget* Target)
{
	//if(ViewSet) return;
	if(IsValid(Target) == false)
		return;

	if(APlayerController* PlayerController = Cast<APlayerController>(Controller.Get()))
	{
		FVector EyePos;
		FRotator Rot;
		GetActorEyesViewPoint(EyePos, Rot);

		FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(EyePos,Target->GetActorLocation());
		PlayerController->SetControlRotation(PlayerRot);
		if(APlayerCameraManager* Camera = PlayerController->PlayerCameraManager.Get())
		{


			FVector Loc = Target->GetActorLocation();
			FVector2D Range = Target->GetRange();

			FVector LocPitchMax = Loc + Target->GetActorUpVector() * Range.Y;
			FRotator PitchMax = UKismetMathLibrary::FindLookAtRotation(EyePos,LocPitchMax);
			Camera->ViewPitchMax = PitchMax.Pitch;

			FVector LocPitchMin= Loc - Target->GetActorUpVector() * Range.Y;
			FRotator PitchMin = UKismetMathLibrary::FindLookAtRotation(EyePos,LocPitchMin);
			Camera->ViewPitchMin = PitchMin.Pitch;


			FVector LocViewYawMax = Loc - Target->GetActorRightVector() * Range.X;
			FRotator ViewYawMax = UKismetMathLibrary::FindLookAtRotation(EyePos,LocViewYawMax);
			Camera->ViewYawMax = ViewYawMax.Yaw;
			
			
			FVector LocViewYawMin= Loc + Target->GetActorRightVector() * Range.X;
			FRotator ViewYawMin = UKismetMathLibrary::FindLookAtRotation(EyePos,LocViewYawMin);
			Camera->ViewYawMin = ViewYawMin.Yaw;

			FVector Top = Loc + Target->GetActorUpVector() * Range.Y - Target->GetActorRightVector() * Range.X;
			float TopDist = (EyePos - Top).Length();
			FVector Bottom = Loc - Target->GetActorUpVector() * Range.Y - Target->GetActorRightVector() * Range.X;
			float BottomDist = (EyePos - Bottom).Length();

			ViewTargetDistance = (EyePos - Loc).Length();;
			
			ViewTargetDistanceMin = TopDist < BottomDist ? TopDist : BottomDist;
			ViewTargetDistanceMin = ViewTargetDistance < ViewTargetDistanceMin ? ViewTargetDistance : ViewTargetDistanceMin;
			
			ViewTargetDistanceMax = TopDist > BottomDist ? TopDist : BottomDist;;
		}
		ViewSet = true;
	}
}


FVector ABasePawn::GetTrajectoryStart()
{
	FVector EyePos;
	FRotator Rot;
	GetActorEyesViewPoint(EyePos,Rot);
	
	EyePos += GetActorForwardVector() * TrajectoryStart.Y;
	EyePos += GetActorRightVector() * TrajectoryStart.X;
	EyePos += GetActorUpVector() * TrajectoryStart.Z;
	return EyePos;
}

float ABasePawn::ProjectileAngle()
{
	float DistRange = ViewTargetDistanceMax - ViewTargetDistanceMin;
	float AngleRange = ProjectileAngleMax - ProjectileAngleMin;

	float Distance =FMath::Clamp( (GetActorLocation() - HitLocationCache).Length(),ViewTargetDistanceMin,ViewTargetDistanceMax);
	float Ratio = Distance - ViewTargetDistanceMin;
	float Angle = ProjectileAngleMin + Ratio * ( AngleRange / DistRange );
	return 1- Angle;
}

bool ABasePawn::CheckLineSight(FVector& HitLOcation)
{
	FVector Pos;
	FRotator Rot;
	GetActorEyesViewPoint(Pos,Rot);

	FVector EndPoint = Pos + Rot.Vector().GetSafeNormal() * LineSightRange;
	if(IsValid(GetViewTarget()) == false) return false;

	FPlane Plane = FPlane(GetViewTarget()->GetActorLocation(), GetViewTarget()->GetActorForwardVector());

	float T;
	if(UKismetMathLibrary::LinePlaneIntersection(Pos, EndPoint, Plane, T, HitLOcation))
	{
		HitLocationCache = HitLOcation;
		return true;
	}

	return false;
}

bool ABasePawn::SuggestProjectileVelocity(FVector& OutVelocity)
{
	FVector EndPos;
	if(CheckLineSight(EndPos))
	{
		if(UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(),OutVelocity,GetTrajectoryStart(),EndPos,0,ProjectileAngle()))
		{
			return true;
		}	
	}
	OutVelocity = FVector::Zero();
	return false;
}

bool ABasePawn::PredictProjectilePathByTraceChannel()
{

	FPredictProjectilePathParams Params = FPredictProjectilePathParams(0, GetTrajectoryStart(), ProjectileVelocity, 2.f);
	Params.bTraceWithCollision = true;
	Params.SimFrequency = 15.f;
	Params.OverrideGravityZ = 0;
	Params.TraceChannel = ECC_WorldDynamic; // Trace by channel
	
	// Do the trace
	FPredictProjectilePathResult PredictResult;
	bool bHit = UGameplayStatics::PredictProjectilePath(GetWorld(), Params, PredictResult);
	
	// Fill in results.
	if(bHit)
	{
		OutPathPositions.Empty(PredictResult.PathData.Num());
		for (const FPredictProjectilePathPointData& PathPoint : PredictResult.PathData)
		{
			OutPathPositions.Add(PathPoint.Location);
		}
	}
	return bHit;
}

void ABasePawn::RefreshPrediction()
{
	if(OutPathPositions.IsEmpty())
		return;

	for (int a = 0; a < OutPathPositions.Num(); ++a)
	{
		if(a == OutPathPositions.Num() - 1)
			DrawDebugSphere(GetWorld(),OutPathPositions[a],DebugLineRadius * DebugLineEndMul,4,FColor::Magenta,false,-1,0,DebugLineThick);
		else
			DrawDebugSphere(GetWorld(),OutPathPositions[a],DebugLineRadius ,4,FColor::Green,false,-1,0,DebugLineThick);
	}
	
}

TSubclassOf<AProjectileBase> ABasePawn::GetProjetileClass()
{
	TSoftClassPtr<AProjectileBase> Class;
	ProjectileId++;
	if (ProjetileClass.IsValidIndex(ProjectileId) == false)
		ProjectileId = 0;
	if(ProjetileClass[ProjectileId].IsNull())
		return nullptr;
	return ProjetileClass[ProjectileId].LoadSynchronous();
}

AProjectileBase* ABasePawn::GetProjectile()
{
	if(GetWorld())
	{
		FActorSpawnParameters Param;

		AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(GetProjetileClass(), GetTrajectoryStart(),FRotator::ZeroRotator);
		if(IsValid(Projectile))
		{
			ProjectileCache.AddUnique(Projectile);
			return  Projectile;
		}
	}
	return nullptr;
}

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABasePawn::Look(const FInputActionValue& Value)
{
	 FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABasePawn::Shoot()
{
	if(AProjectileBase* Projectile = GetProjectile())
	{
		Projectile->SetActorLocation(GetTrajectoryStart());
		FVector Vel = ProjectileVelocity * ProjectileMul;
		Projectile->SetProjectileVelocity(ProjectileVelocity,MassRef);
	}
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(SuggestProjectileVelocity(ProjectileVelocity))
	{

	}
	if(ShowTrajectory)
	{
		PredictProjectilePathByTraceChannel();
		RefreshPrediction();
	}
	
}

// Called to bind functionality to input
void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if(UEnhancedInputComponent* KFEnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if(APlayerController* PlayerController = Cast<APlayerController>(Controller.Get()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				if(IMC_Default)
				{
					if (!Subsystem->HasMappingContext(IMC_Default))
						Subsystem->AddMappingContext(IMC_Default, 1, FModifyContextOptions());

					// Look
					KFEnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ABasePawn::Look);
					// Interact
					KFEnhancedInputComponent->BindAction(IA_Shoot, ETriggerEvent::Triggered, this, &ABasePawn::Shoot);
				}
			}
		}
	}
	SetViewTargetCenter(GetViewTarget());
}

void ABasePawn::SetViewDirection()
{
	SetViewTargetCenter(GetViewTarget());
}

