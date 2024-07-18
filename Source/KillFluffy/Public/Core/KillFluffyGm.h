// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FluffyStructs.h"
#include "GameFramework/GameModeBase.h"
#include "KillFluffyGm.generated.h"

class AWorldEntity;
class AViewTarget;
class ABasePawn;
class AMechanicManager;
class ABaseTarget;
class AProjectileBase;
class UWorldManager;
/**
 * 
 */
UCLASS()
class KILLFLUFFY_API AKillFluffyGm : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Transient)
	UWorldManager* WorldManagerCache;
	UWorldManager* GetWorldManager();

	UPROPERTY(EditAnywhere,Transient)
	AWorldEntity* WorldEntityCache;
	AWorldEntity* GetWorldEntity();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTargetGotHit(ABaseTarget* Target, AProjectileBase* Projectile);
	UFUNCTION()
	void OnMechanicManagerAllTargetsDestroyed(AMechanicManager* MechanicManager);


#pragma region Game States

private:

	UPROPERTY(VisibleAnywhere)
	EGameState GameStateCache;

	void SwitchNone();
	void SwitchSetup();
	void SwitchStart();
	void SwitchIngame();
	void SwitchRewrad();
	void SwitchEnd();

public:

	EGameState& GetGameState() { return GameStateCache; }
	void SwitchGameState(EGameState State);

#pragma endregion Game States

private:

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<ABasePawn> BasePawnRef;
	UPROPERTY(EditAnywhere, Transient)
	ABasePawn* PawnCache;
	ABasePawn* GetPawnBase();

	UPROPERTY(VisibleAnywhere, Transient)
	int MechanicManagerRefId;
	UPROPERTY(EditAnywhere)
	TArray<TSoftClassPtr<AMechanicManager>> AMechanicManagerRefs;
	TSubclassOf<AMechanicManager> GetMechanicManagerClass();

	UPROPERTY(EditAnywhere, Transient)
	AMechanicManager* MechanicManagerCache;

	UPROPERTY(EditAnywhere, Transient)
	AViewTarget* ViewTargetCahe;
	AViewTarget* GetViewTarget();




private:

	UPROPERTY(EditAnywhere, Transient)
	TArray<UPrimitiveComponent*> TargetPieces;

	UPROPERTY(EditAnywhere)
	AActor* PieceHolder;

public:

	void AddTargetPiece(UPrimitiveComponent* Piece);
};
