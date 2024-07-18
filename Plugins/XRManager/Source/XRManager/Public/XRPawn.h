// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "XRPawn.generated.h"

class UInputMappingContext;
class UInputAction;
class UMotionControllerComponent;
class UXRMCComponent;
class UXRMCHandComponent;
class UXRGrabableComponent;
class USceneComponent;
class UAttachmentManager;
struct FInputActionValue;

UCLASS()
class XRMANAGER_API AXRPawn : public APawn
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

public:
	// Sets default values for this pawn's properties
	AXRPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


#pragma region MotionController

private:

	UPROPERTY(EditAnywhere, Category = "Input|Mappings")
	float GrabRadius = 1.f;

	UPROPERTY(VisibleAnywhere, Category = "Input|Mappings")
	UXRMCHandComponent* MC_Grip_L;

	UPROPERTY(VisibleAnywhere, Category = "Input|Mappings")
	UXRMCHandComponent* MC_Grip_R;


	void Grab(UXRMCHandComponent* MCC);
	void Release(UXRMCHandComponent* MCC);

	UXRGrabableComponent* GetGrabable(UXRMCHandComponent* MCC);


	bool IsHeadMountedDisplayEnabled();

	bool IsHeadMountedDisplayConnected();


public:

	UXRMCHandComponent* GetMC_Grip_L() { return MC_Grip_L; }
	UXRMCHandComponent* GetMC_Grip_R() { return MC_Grip_R; }

#pragma endregion MotionController


#pragma region Input

private:

	UPROPERTY(EditAnywhere, Category = "Input|Mappings")
	UInputMappingContext* IMC_Default;

	UPROPERTY(EditAnywhere, Category = "Input|Actions")
	UInputAction* IA_Grab_L;
	UPROPERTY(EditAnywhere, Category = "Input|Actions")
	UInputAction* IA_Grab_R;


	UPROPERTY(EditAnywhere, Category = "Input|Actions")
	UInputAction* IA_Move_L;

protected:

	void Move(const FInputActionValue& Value);

	void Grab_L(const FInputActionValue& Value);
	void Grab_R(const FInputActionValue& Value);

	void Release_L(const FInputActionValue& Value);
	void Release_R(const FInputActionValue& Value);



#pragma endregion Input


#pragma region Attachment

private:

	UPROPERTY(VisibleAnywhere)
	UAttachmentManager* AttacmentManager;

	void SetupAtachment();

#pragma endregion Attachment

};
