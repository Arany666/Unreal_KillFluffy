// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "KFEnhancedInputComponent.generated.h"

class UInputMappingContext;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class KILLFLUFFY_API UKFEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

	/*UPROPERTY(EditAnywhere, Category = "Input|Mappings")
	UInputMappingContext* IMC_Default;

	UPROPERTY(EditAnywhere, Category = "Input|Actions")
	UInputAction* Horizontal;
	UPROPERTY(EditAnywhere, Category = "Input|Actions")
	UInputAction* Vertical;


	UPROPERTY(EditAnywhere, Category = "Input|Actions")
	UInputAction* Look;
	UPROPERTY(EditAnywhere, Category = "Input|Actions")
	UInputAction* Shoot;*/

	virtual void BeginPlay() override;
	void ApplyInputMappingContexts(const TArray<UInputMappingContext*>& InIMCs, bool bClearAll = true);

public:

	
	//UInputMappingContext*	GetIMC_Default	(){return IMC_Default;}
	//UInputAction*			GetHorizontal	(){return Horizontal;}
	//UInputAction*			GetVertical		(){return Vertical;}

	//UInputAction*			GetLook		(){return Look;}
	//UInputAction*			GetShoot		(){return Shoot;}
};
