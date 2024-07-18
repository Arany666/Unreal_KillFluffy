// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/KFEnhancedInputComponent.h"

void UKFEnhancedInputComponent::BeginPlay()
{
	Super::BeginPlay();

	//ApplyInputMappingContexts({ IMC_Default });
}

void UKFEnhancedInputComponent::ApplyInputMappingContexts(const TArray<UInputMappingContext*>& InIMCs, bool bClearAll)
{

}
