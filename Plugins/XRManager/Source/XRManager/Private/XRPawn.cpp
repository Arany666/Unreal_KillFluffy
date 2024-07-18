// Fill out your copyright notice in the Description page of Project Settings.


#include "XRPawn.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "MotionControllerComponent.h"
#include "XRMCComponent.h"
#include "XRMCHandComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "XRGrabableComponent.h"

#include "IXRTrackingSystem.h"
#include "IHeadMountedDisplay.h"
#include "HeadMountedDisplayFunctionLibrary.h"

#include "AttachmentManager.h"

// Sets default values
AXRPawn::AXRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	MC_Grip_L = CreateDefaultSubobject<UXRMCHandComponent>(TEXT("MCGripL"));
	MC_Grip_L->SetupAttachment(Root);
	MC_Grip_R = CreateDefaultSubobject<UXRMCHandComponent>(TEXT("MCGripR"));
	MC_Grip_R->SetupAttachment(Root);

	AttacmentManager = CreateDefaultSubobject<UAttachmentManager>(TEXT("AttacmentManager"));
	
}

// Called when the game starts or when spawned
void AXRPawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Stage);
		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), "vr.PixelDensity 1.0");
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("VR OK")));
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("VR NOOOOOOOOOOOOOO")));
	}

	SetupAtachment();
}

// Called every frame
void AXRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AXRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("SetupPlayerInputComponent")));

	if (UEnhancedInputComponent* KFEnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller.Get()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				if (IMC_Default)
				{
					if (!Subsystem->HasMappingContext(IMC_Default))
						Subsystem->AddMappingContext(IMC_Default, 1, FModifyContextOptions());

					// Grab
					KFEnhancedInputComponent->BindAction(IA_Grab_L, ETriggerEvent::Started, this, &AXRPawn::Grab_L);
					KFEnhancedInputComponent->BindAction(IA_Grab_R, ETriggerEvent::Started, this, &AXRPawn::Grab_R);

					// Release
					KFEnhancedInputComponent->BindAction(IA_Grab_L, ETriggerEvent::Completed, this, &AXRPawn::Release_L);
					KFEnhancedInputComponent->BindAction(IA_Grab_R, ETriggerEvent::Completed, this, &AXRPawn::Release_R);


					KFEnhancedInputComponent->BindAction(IA_Move_L, ETriggerEvent::Triggered, this, &AXRPawn::Move);

					if (GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Bind Action")));

					//// Interact
					//KFEnhancedInputComponent->BindAction(IA_Shoot, ETriggerEvent::Triggered, this, &ABasePawn::Shoot);
				}
			}
		}
	}
}

void AXRPawn::Grab(UXRMCHandComponent* MCC)
{
	if (IsValid(MCC) == false)
		return;

	if (UXRGrabableComponent* Grab = GetGrabable(MCC)) 
	{
		AttacmentManager->AttachToHandSlot(Grab, MCC == MC_Grip_R);
	}
}

void AXRPawn::Release(UXRMCHandComponent* MCC)
{
	if (IsValid(MCC) == false)
		return;

	if (UXRGrabableComponent* Grab = GetGrabable(MCC))
	{
		if (UPrimitiveComponent* Primitive = AttacmentManager->DetachFromHandSlot(Grab))
		{
			FVector CenterMass = MCC->GetCenterOfMass();
			FVector AngularVelo = MCC->GetPhysicsAngularVelocityInDegrees();
			FVector Cross = FVector::CrossProduct(AngularVelo, Grab->GetComponentLocation() - CenterMass);
			//FVector MCC_Velocity = MCC->GetPhysicsLinearVelocity();
			FVector MCC_Velocity = MCC->GetPhysicsLinearVelocity() + Cross;
			Primitive->SetAllPhysicsLinearVelocity(MCC_Velocity, true);
			Primitive->SetAllPhysicsAngularVelocityInDegrees(AngularVelo, true);
		}
	}
}

UXRGrabableComponent* AXRPawn::GetGrabable(UXRMCHandComponent* MCC)
{
	if (IsValid(MCC) == false)
		return nullptr;

	FVector MccLocation = MCC->GetComponentLocation();

	if (UWorld* World = GetWorld())
	{
		TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes ;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

		TArray<AActor*> ActorsToIgnore = TArray<AActor*> {this};
		EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::Type::Persistent; 
		TArray<FHitResult> OutHit;

		if (UKismetSystemLibrary::SphereTraceMultiForObjects(World, MccLocation, MccLocation, GrabRadius, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit, true))
		{
			if (OutHit.Num() > 0)
			{
				for (int i = 0; i < OutHit.Num(); ++i)
				{
					if (AActor* Actor = OutHit[i].GetActor()) 
					{
						if (UXRGrabableComponent* Grab = Cast<UXRGrabableComponent>(Actor->GetComponentByClass(UXRGrabableComponent::StaticClass())))
						{
							return Grab;
						}
					}
				}
			}
		}
		else
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("No HIt")));
		}
	}
	
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("No Grab Componenet")));

	return nullptr;
}

bool AXRPawn::IsHeadMountedDisplayEnabled()
{
	return GEngine->XRSystem.IsValid() && GEngine->XRSystem->IsHeadTrackingAllowed();
}

bool AXRPawn::IsHeadMountedDisplayConnected()
{
	return GEngine->XRSystem.IsValid() && GEngine->XRSystem->GetHMDDevice() && GEngine->XRSystem->GetHMDDevice()->IsHMDConnected();
}



void AXRPawn::Move(const FInputActionValue& Value)
{
	FVector2D MoveAxisVector = Value.Get<FVector2D>();

	if (MoveAxisVector == FVector2D::ZeroVector) return;


	FVector Offset = FVector::ZeroVector;
	FVector RDir = GetActorRightVector();
	FVector FWDir = GetActorForwardVector();

	if (MoveAxisVector.X != 0)
		Offset += RDir * MoveAxisVector.X;
	if (MoveAxisVector.Y != 0)
		Offset += FWDir * MoveAxisVector.Y;

	AddActorWorldOffset(Offset);
}

void AXRPawn::Grab_L(const FInputActionValue& Value)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Grab Input LEft")));
	Grab(MC_Grip_L);

}

void AXRPawn::Grab_R(const FInputActionValue& Value)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Grab Input Right")));

	Grab(MC_Grip_R);
}

void AXRPawn::Release_L(const FInputActionValue& Value)
{
	Release(MC_Grip_L);
}

void AXRPawn::Release_R(const FInputActionValue& Value)
{
	Release(MC_Grip_R);
}

void AXRPawn::SetupAtachment()
{
	AttacmentManager->SetXRPawn(this);
}

