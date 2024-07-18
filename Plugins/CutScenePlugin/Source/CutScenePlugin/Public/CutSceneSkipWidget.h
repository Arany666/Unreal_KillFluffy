// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Blueprint/UserWidget.h"
#include "CutSceneSkipWidget.generated.h"

/**
 * 
 */

class UButton;

UCLASS()
class CUTSCENEPLUGIN_API UCutSceneSkipWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Button_Skip;
public:
	UFUNCTION(BlueprintCallable)
	void SkipButtonPushed();
};
