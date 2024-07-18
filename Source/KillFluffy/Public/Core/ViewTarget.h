// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ViewTarget.generated.h"

class APlayerStart;
class USceneComponent;
class UBoxComponent;

UCLASS()
class KILLFLUFFY_API AViewTarget : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere,Meta = (MakeEditWidget = true))
	USceneComponent* PlayerPosition;

	UPROPERTY(EditAnywhere)
	FVector2D Range;

#if WITH_EDITORONLY_DATA

	
	

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComp;



#endif

#if WITH_EDITOR

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;


#endif

public:	
	// Sets default values for this actor's properties
	AViewTarget();


public:	

	FVector2D& GetRange(){return Range;};
	FTransform GetPlayerPosition();


};
