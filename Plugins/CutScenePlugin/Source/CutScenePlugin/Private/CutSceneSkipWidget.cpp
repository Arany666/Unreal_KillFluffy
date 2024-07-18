// Fill out your copyright notice in the Description page of Project Settings.


#include "CutSceneSkipWidget.h"

#include "CutSceneManager.h"
#include "Components/Button.h"

void UCutSceneSkipWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(IsValid(Button_Skip) == false) return;

	if(Button_Skip->OnClicked.IsAlreadyBound(this, &UCutSceneSkipWidget::SkipButtonPushed) == false)
		Button_Skip->OnClicked.AddDynamic(this, &UCutSceneSkipWidget::SkipButtonPushed);
}

void UCutSceneSkipWidget::SkipButtonPushed()
{
	if(UWorld* World = GetWorld())
	{
		if(UCutSceneManager* CutSceneManagerCache = World->GetSubsystem<UCutSceneManager>())
		{
			CutSceneManagerCache->SkipCutScene();
		}
	}
}
