// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SceneComponent.h"
#include "AudioContactComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AUDIOHELPER_API UAudioContactComponent : public USceneComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FGameplayTag MaterialType;

public:	
	// Sets default values for this component's properties
	UAudioContactComponent();

	const FGameplayTag GetMaterialType() { return MaterialType; }

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnComponentHit(/* UPrimitiveComponent OnComponentHit,*/ UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
