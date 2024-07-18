// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/KillFluffyGm.h"

#include "EngineUtils.h"
#include "Core/BasePawn.h"
#include "Core/BaseTarget.h"
#include "Core/ViewTarget.h"
#include "Core/WorldEntity.h"
#include "Core/WorldManager.h"
#include "GameFramework/PlayerStart.h"
#include "Mechanics/MechanicManager.h"

UWorldManager* AKillFluffyGm::GetWorldManager()
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

AWorldEntity* AKillFluffyGm::GetWorldEntity()
{
	if (IsValid(WorldEntityCache) == false)
	{
		for (TActorIterator<AWorldEntity> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			if (AWorldEntity* Interactable = *ActorItr)
			{
				WorldEntityCache = Interactable;
				break;
			}
		}
	}
	return WorldEntityCache;
}

void AKillFluffyGm::BeginPlay()
{
	Super::BeginPlay();

	SwitchGameState(EGameState::GS_Setup);

}

void AKillFluffyGm::OnTargetGotHit(ABaseTarget* Target, AProjectileBase* Projectile)
{
	//if(IsValid(Target))
	//{
	//	if(Target->IsPendingKillPending() == false)
	//		Target->Destroy();
	//}
	//if(IsValid(Projectile))
	//{
	//	if(Projectile->IsPendingKillPending() == false)
	//		Projectile->Destroy();
	//}
}

void AKillFluffyGm::OnMechanicManagerAllTargetsDestroyed(AMechanicManager* MechanicManager)
{
	SwitchGameState(EGameState::GS_Rewrad);
}


#pragma region Game States

void AKillFluffyGm::SwitchNone()
{
}

void AKillFluffyGm::SwitchSetup()
{
	GetPawnBase();
	MechanicManagerCache = GetWorld()->SpawnActor<AMechanicManager>(GetMechanicManagerClass());
	if(IsValid(GetWorldEntity()))
	{
		if(GetWorldEntity()->GetHolder())
		{
			FAttachmentTransformRules Rules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
			MechanicManagerCache->AttachToComponent(GetWorldEntity()->GetHolder(),Rules);

			FWorldEntitySignature Del;
			Del.BindLambda([this]
				{
					SwitchGameState(EGameState::GS_Start);
				});

			GetWorldEntity()->FadeHolderIn(Del);
		}
	}
	//if(IsValid(GetViewTarget()))
	//{
	//	if (IsValid(PawnCache) == false)
	//	{
	//		if(TSubclassOf<ABasePawn> BasePawnClass = BasePawnRef.LoadSynchronous())
	//		{
	//			PawnCache = GetWorld()->SpawnActor<ABasePawn>(BasePawnClass,GetViewTarget()->GetPlayerPosition(),FActorSpawnParameters());
	//			if(APlayerController* PC = GetWorld()->GetFirstPlayerController() )
	//			{
	//				PC->Possess(PawnCache);
	//			}
	//		}
	//	}
	//	else
	//	{
	//		PawnCache->TeleportTo(GetViewTarget()->GetPlayerPosition().GetLocation(), GetViewTarget()->GetPlayerPosition().Rotator());
	//		PawnCache->SetViewDirection();
	//	}
	//}
	//SwitchGameState(EGameState::GS_Start);
}

void AKillFluffyGm::SwitchStart()
{
	if(IsValid(MechanicManagerCache))
	{
		FDetachmentTransformRules Rules = FDetachmentTransformRules::KeepWorldTransform;
		MechanicManagerCache->DetachFromActor(Rules);
	}
	if(IsValid(GetViewTarget()))
	{
		if (IsValid(GetPawnBase()))
		{
			GetPawnBase()->TeleportTo(GetViewTarget()->GetPlayerPosition().GetLocation(), GetViewTarget()->GetPlayerPosition().Rotator());
			GetPawnBase()->SetViewTargetCenter(GetViewTarget());
			GetPawnBase()->SetViewDirection();
		}
		//if (IsValid(PawnCache) == false)
		//{
		//	if(TSubclassOf<ABasePawn> BasePawnClass = BasePawnRef.LoadSynchronous())
		//	{
		//		PawnCache = GetWorld()->SpawnActor<ABasePawn>(BasePawnClass,GetViewTarget()->GetPlayerPosition(),FActorSpawnParameters());
		//		if(APlayerController* PC = GetWorld()->GetFirstPlayerController() )
		//		{
		//			PC->Possess(PawnCache);
		//		}
		//	}
		//}
		//else
		//{
		//	PawnCache->TeleportTo(GetViewTarget()->GetPlayerPosition().GetLocation(), GetViewTarget()->GetPlayerPosition().Rotator());
		//	PawnCache->SetViewDirection();
		//}
	}
	SwitchGameState(EGameState::GS_Ingame);
}

void AKillFluffyGm::SwitchIngame()
{
	if(IsValid(GetWorldManager()))
	{
		GetWorldManager()->TargetGotHit.AddUniqueDynamic(this, &AKillFluffyGm::OnTargetGotHit);
		GetWorldManager()->MechanicManagerAllTargetsDestroyed.AddUniqueDynamic(this, &AKillFluffyGm::AKillFluffyGm::OnMechanicManagerAllTargetsDestroyed);
	}
}

void AKillFluffyGm::SwitchRewrad()
{
	SwitchGameState(EGameState::GS_End);
}

void AKillFluffyGm::SwitchEnd()
{
	if (IsValid(GetWorldEntity()) && IsValid(MechanicManagerCache))
	{
		if (GetWorldEntity()->GetHolder())
		{
			FAttachmentTransformRules Rules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
			MechanicManagerCache->AttachToComponent(GetWorldEntity()->GetHolder(), Rules);

			FWorldEntitySignature Del;
			Del.BindLambda([this]
				{
					if (IsValid(MechanicManagerCache))
						MechanicManagerCache->Destroy();
					MechanicManagerRefId++;
					SwitchGameState(EGameState::GS_Setup);
				});

			GetWorldEntity()->FadeHolderOut(Del);
		}
	}
	else
	{
		if (IsValid(MechanicManagerCache))
			MechanicManagerCache->Destroy();
		MechanicManagerRefId++;
		SwitchGameState(EGameState::GS_Setup);
	}
	//if (IsValid(MechanicManagerCache))
	//	MechanicManagerCache->Destroy();
	//MechanicManagerRefId++;
	//SwitchGameState(EGameState::GS_Setup);
}

void AKillFluffyGm::SwitchGameState(EGameState State)
{
	if(GameStateCache == State)
		return;

	GameStateCache = State;

	switch (GameStateCache)
	{
	case EGameState::GS_None:
		SwitchNone();
		break;
	case EGameState::GS_Setup:
		SwitchSetup();
		break;
	case EGameState::GS_Start:
		SwitchStart();
		break;
	case EGameState::GS_Ingame:
		SwitchIngame();
		break;
	case EGameState::GS_Rewrad:
		SwitchRewrad();
		break;
	case EGameState::GS_End:
		SwitchEnd();
		break;
	default: ;
	}
}

ABasePawn* AKillFluffyGm::GetPawnBase()
{
	if (IsValid(PawnCache) == false)
	{
		if (TSubclassOf<ABasePawn> BasePawnClass = BasePawnRef.LoadSynchronous())
		{
			PawnCache = GetWorld()->SpawnActor<ABasePawn>(BasePawnClass, GetViewTarget() ? GetViewTarget()->GetPlayerPosition() : FTransform(), FActorSpawnParameters());
			if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
			{
				PC->Possess(PawnCache);
			}
		}
	}
	return PawnCache;
}

TSubclassOf<AMechanicManager> AKillFluffyGm::GetMechanicManagerClass()
{
	if (AMechanicManagerRefs.IsValidIndex(MechanicManagerRefId) == false)
		MechanicManagerRefId = 0;
	return AMechanicManagerRefs[MechanicManagerRefId].LoadSynchronous();
}

AViewTarget* AKillFluffyGm::GetViewTarget()
{
	if(IsValid(ViewTargetCahe) == false)
	{
		for (TActorIterator<AViewTarget> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			if(AViewTarget* Interactable = *ActorItr)
			{
				ViewTargetCahe = Interactable;
				break;
			}
		}
	}
	return ViewTargetCahe;
}



#pragma endregion Game States

void AKillFluffyGm::AddTargetPiece(UPrimitiveComponent* Piece)
{
	if(IsValid(PieceHolder) == false)
	{
		PieceHolder = GetWorld()->SpawnActor(AActor::StaticClass());
		USceneComponent* Root = NewObject<USceneComponent>(PieceHolder);
		Root->RegisterComponent();
		PieceHolder->SetRootComponent(Root);
	}

	USceneComponent* PieceHolderRoot = PieceHolder->GetRootComponent();

	AActor* OldParent = Piece->GetAttachParentActor();

	FDetachmentTransformRules Rules = FDetachmentTransformRules::KeepWorldTransform;
	Piece->DetachFromComponent(Rules);

	FAttachmentTransformRules ARules = FAttachmentTransformRules::KeepWorldTransform;
	Piece->AttachToComponent(PieceHolderRoot, ARules);

	FString TEmp = FString("TrapPiece");
	TEmp += FString::FromInt(TargetPieces.Num());
	const TCHAR* wavLink = *TEmp;
	Piece->Rename(wavLink, PieceHolderRoot);

	TargetPieces.AddUnique(Piece);

	Piece->SetSimulatePhysics(true);
	if (Piece->BodyInstance.IsValidBodyInstance())
	{
		Piece->BodyInstance.AddImpulseAtPosition(OldParent->GetActorForwardVector() * 100.f, OldParent->GetActorLocation());
	}
}
