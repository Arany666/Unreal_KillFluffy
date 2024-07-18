// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "Subsystems/WorldSubsystem.h"
#include "WorldManager.generated.h"

class ABaseTarget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTargetProjectileSignature,ABaseTarget*, Target, AProjectileBase*, Projectile);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMechanicManagerSignature, AMechanicManager*, MechanicManager);

UCLASS()
class KILLFLUFFY_API UWorldManager : public UWorldSubsystem
{
	GENERATED_BODY()


	UPROPERTY()
	TArray<ABaseTarget*> TargetCache;

public:

	void RegisterAllTarget();
	void RegisterTarget(ABaseTarget* Target);
	void DeRegisterTarget(ABaseTarget* Target);




public:

	UPROPERTY(BlueprintAssignable)
	FTargetProjectileSignature TargetGotHit;

	UPROPERTY(BlueprintAssignable)
	FMechanicManagerSignature MechanicManagerAllTargetsDestroyed;
};
