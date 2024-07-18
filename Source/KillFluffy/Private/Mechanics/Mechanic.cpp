// Fill out your copyright notice in the Description page of Project Settings.


#include "Mechanics/Mechanic.h"

//FMechanicStep* UMechanic::CurrentStep()
//{
//	if(Steps.IsValidIndex(StepId))
//	{
//		return &Steps[StepId];
//	}
//	return nullptr;
//}

void UMechanic::CollectMechanics()
{
	TArray<USceneComponent*> Temp;
	GetChildrenComponents(false,Temp);
	
	if(Temp.IsEmpty()) return;
	
	for (USceneComponent* Component : Temp)
	{
		if(UMechanicBase* MechanicBase = Cast<UMechanicBase>(Component))
		{
			MechanicsCache.AddUnique(MechanicBase);
		}
	}
}

void UMechanic::SetupMechanicsInner()
{
	if(MechanicsCache.IsEmpty()) return;
	for (UMechanicBase* Mechanic : MechanicsCache)
	{
		if(IsValid(Mechanic))
			Mechanic->SetupMechanic();
	}
}

void UMechanic::UpdateAllPosition()
{
	if(MechanicsCache.IsEmpty()) return;
	for (UMechanicBase* Mechanic : MechanicsCache)
	{
		UpdateMechanicPosition(Mechanic);
	}
}

void UMechanic::UpdateMechanicPosition(UMechanicBase* Mechanic)
{
	if(IsValid(Mechanic) == false) return;

	Reverse = false;
	if(Mechanic->DirectionReverse != InitialDirectionReverse)
		Reverse = true;

	CurrentPositionFixed = Reverse == false ? Mechanic->CurrentPosition : 1 - Mechanic->CurrentPosition;
}

void UMechanic::SetupMechanic()
{
	Super::SetupMechanic();

	CollectMechanics();
	SetupMechanicsInner();

	UpdateAllPosition();
}

void UMechanic::TimerUpdate(float DeltaTime, float FullRoundTime)
{
	if(MechanicsCache.IsEmpty()) return;
	float BaseRoundTime = (1.f / FullRoundTime) * ( 1.f / RoundTimeMultiplier);

	for (int a = 0; a < MechanicsCache.Num();++a)
	{
		UMechanicBase* MechanicBase = MechanicsCache[a];
		if(UMechanic* Mechanic = Cast<UMechanic>(MechanicBase))
		{
			Mechanic->TimerUpdate(DeltaTime, FullRoundTime);
		}

		if(FMechanicStep* Step = MechanicBase->CurrentStep())
		{
			if(Step->CurrentStep < Step->Step)
			{
				if(Step->Delay)
				{
					Step->CurrentStep += DeltaTime;
				}
				else
				{
					Step->CurrentStep += DeltaTime * BaseRoundTime;

					if (MechanicsCache[a]->CurrentPosition < 1)
					{
						MechanicsCache[a]->CurrentPosition += DeltaTime * BaseRoundTime;
					}
					else
					{
						MechanicsCache[a]->CurrentPosition = 0.f;
						if (Loop == false)
							MechanicsCache[a]->DirectionReverse = !MechanicsCache[a]->DirectionReverse;
					}
					MechanicsCache[a]->CurrentPosition = FMath::Clamp(MechanicsCache[a]->CurrentPosition, 0.f, 1.f);
					UpdateMechanicPosition(MechanicsCache[a]);
				}
			}
			else
			{
				MechanicBase->NextStepId();
			}
		}
		else
		{
			if (MechanicsCache[a]->CurrentPosition < 1)
			{
				MechanicsCache[a]->CurrentPosition += DeltaTime * BaseRoundTime;
			}
			else
			{
				MechanicsCache[a]->CurrentPosition = 0.f;
				if (Loop == false)
					MechanicsCache[a]->DirectionReverse = !MechanicsCache[a]->DirectionReverse;
			}
			MechanicsCache[a]->CurrentPosition = FMath::Clamp(MechanicsCache[a]->CurrentPosition, 0.f, 1.f);
			UpdateMechanicPosition(MechanicsCache[a]);
		}

		//if(MechanicsCache[a]->CurrentPosition < 1)
		//{
		//	MechanicsCache[a]->CurrentPosition += DeltaTime * BaseRoundTime;
		//}
		//else
		//{
		//	MechanicsCache[a]->CurrentPosition = 0.f;
		//	if(Loop == false)
		//		MechanicsCache[a]->DirectionReverse = !MechanicsCache[a]->DirectionReverse;
		//}
		//MechanicsCache[a]->CurrentPosition = FMath::Clamp(MechanicsCache[a]->CurrentPosition,0.f,1.f);
		//UpdateMechanicPosition(MechanicsCache[a]);
	}
}
