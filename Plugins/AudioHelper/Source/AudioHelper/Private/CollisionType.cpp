// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionType.h"

const FCollisionStruct* UCollisionType::GetCollisionStructs(FGameplayTag TypeA, FGameplayTag TypeB) const
{
	for (const FCollisionStruct& CollisionStruct : CollisionStructs)
	{
		if((CollisionStruct.TypeA.MatchesTagExact(TypeA) || CollisionStruct.TypeA.MatchesTagExact(TypeB)) &&
			(CollisionStruct.TypeB.MatchesTagExact(TypeB) || CollisionStruct.TypeB.MatchesTagExact(TypeA)))
		{
			return &CollisionStruct;
		}
	}
	return nullptr;
}

//const FCollisionStruct* UCollisionType::GetCollisionStructs(UPhysicalMaterial* TypeA, UPhysicalMaterial* TypeB) const
//{
//	//for (const FCollisionStruct& CollisionStruct : CollisionStructs)
//	//{
//	//	if ((CollisionStruct.TypeA.LoadSynchronous() == TypeA || CollisionStruct.TypeA.LoadSynchronous() == TypeB) &&
//	//		(CollisionStruct.TypeB.LoadSynchronous() == TypeA || CollisionStruct.TypeB.LoadSynchronous() == TypeB))
//	//	{
//	//		return &CollisionStruct;
//	//	}
//	//}
//	return nullptr;
//}
