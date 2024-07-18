// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

class UInputAction;
class UInputMappingContext;
class AProjectileBase;
class AViewTarget;
struct FInputActionValue;

UCLASS()
class KILLFLUFFY_API ABasePawn : public APawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Input|Mappings")
	UInputMappingContext* IMC_Default;


	UPROPERTY(EditAnywhere, Category = "Input|Actions")
	UInputAction* IA_Look;
	UPROPERTY(EditAnywhere, Category = "Input|Actions")
	UInputAction* IA_Shoot;

	UPROPERTY(EditAnywhere,Transient)
	AViewTarget* ViewTargetCahe;
	AViewTarget* GetViewTarget();

	UPROPERTY(VisibleAnywhere)
	bool ViewSet;
	

	UPROPERTY(VisibleAnywhere)
	float ViewTargetDistance;
	UPROPERTY(VisibleAnywhere)
	float ViewTargetDistanceMin;
	UPROPERTY(VisibleAnywhere)
	float ViewTargetDistanceMax;

	UPROPERTY(EditAnywhere)
	FVector TrajectoryStart;
	FVector GetTrajectoryStart();

	UPROPERTY(EditAnywhere)
	float LineSightRange = 1000.f;
	UPROPERTY(EditAnywhere,meta=(ClampMin = "0.0", ClampMax = "1.0",UIMin = "0.0",UIMax = "1.0"))
	float ProjectileAngleMin = 0.8f;
	UPROPERTY(EditAnywhere,meta=(ClampMin = "0.0", ClampMax = "1.0",UIMin = "0.0",UIMax = "1.0"))
	float ProjectileAngleMax = 0.4f;

	UPROPERTY(EditAnywhere)
	float ProjectileMul = 2;
	UPROPERTY(EditAnywhere)
	float MassRef = 50.f;

	float ProjectileAngle();
	UPROPERTY(VisibleAnywhere,Transient)
	FVector ProjectileVelocity;

	UPROPERTY(VisibleAnywhere,Transient)
	FVector HitLocationCache;
	bool CheckLineSight(FVector& HitLOcation);
	bool SuggestProjectileVelocity(FVector& OutVelocity);

	UPROPERTY(EditAnywhere,Transient)
	TArray<FVector> OutPathPositions;

	UPROPERTY(EditAnywhere)
	bool ShowTrajectory;

	UPROPERTY(EditAnywhere)
	float DebugLineRadius =10;
	UPROPERTY(EditAnywhere)
	float DebugLineEndMul =3;
	UPROPERTY(EditAnywhere)
	float DebugLineThick =2;
	bool PredictProjectilePathByTraceChannel();
	void RefreshPrediction();

	UPROPERTY(EditAnywhere)
	TArray<TSoftClassPtr<AProjectileBase>> ProjetileClass;
	UPROPERTY(VisibleAnywhere)
	int ProjectileId;

	TSubclassOf<AProjectileBase> GetProjetileClass();

	UPROPERTY(EditAnywhere,Transient)
	TArray<AProjectileBase*> ProjectileCache;

	AProjectileBase* GetProjectile();

public:
	// Sets default values for this pawn's properties
	ABasePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Look(const FInputActionValue& Value);
	void Shoot();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetViewTargetCenter(AViewTarget* Target);
	void SetViewDirection();

};
