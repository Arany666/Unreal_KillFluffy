// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/WorldManager.h"

#include "EngineUtils.h"
#include "Core/BaseTarget.h"

void UWorldManager::RegisterAllTarget()
{
	for (TActorIterator<ABaseTarget> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if(ABaseTarget* Target = *ActorItr)
		{
			RegisterTarget(Target);
		}
	}
}

void UWorldManager::RegisterTarget(ABaseTarget* Target)
{
	if(IsValid(Target) == false)
		return;

	TargetCache.AddUnique(Target);
}

void UWorldManager::DeRegisterTarget(ABaseTarget* Target)
{
	if(IsValid(Target) == false)
		return;
	
	if(TargetCache.Contains(Target))
		TargetCache.Remove(Target);
}
