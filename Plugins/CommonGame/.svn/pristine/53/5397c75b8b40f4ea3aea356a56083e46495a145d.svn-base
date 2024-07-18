// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PrimaryGameLayout.h"
#include "Components/BorderSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Overlay.h"
#include "MudPrimaryGameLayout.generated.h"



/**
 * 
 */
UCLASS()
class COMMONGAME_API UMudPrimaryGameLayout : public UPrimaryGameLayout
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* PermanentUnder;
		UPROPERTY(meta = (BindWidget))
	UCanvasPanel* PermanentOver;

public:

	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	ActivatableWidgetT* AddWidget(TSubclassOf<UCommonActivatableWidget> ActivatableWidgetClass,bool Over = false)
	{
		if (ActivatableWidgetClass && ActivatableWidgetClass->IsChildOf<ActivatableWidgetT>())
		{
			for (uint8 a = 0; a < PermanentUnder->GetChildrenCount();++a)
			{
				if(PermanentUnder->GetChildAt(a)->GetClass()->IsChildOf<ActivatableWidgetT>())
					return Cast<ActivatableWidgetT>(PermanentUnder->GetChildAt(a));
			}
			for (uint8 a = 0; a < PermanentOver->GetChildrenCount();++a)
			{
				if(PermanentOver->GetChildAt(a)->GetClass()->IsChildOf<ActivatableWidgetT>())
					return Cast<ActivatableWidgetT>(PermanentOver->GetChildAt(a));
			}
			ActivatableWidgetT* WidgetInstance = CreateWidget<ActivatableWidgetT>(this, ActivatableWidgetClass);
			if(WidgetInstance)
			{
				if(Over)
					PermanentOver->AddChild(WidgetInstance);
				else
					PermanentUnder->AddChild(WidgetInstance);
				if(UBorderSlot* BorderSlot= Cast<UBorderSlot>(WidgetInstance->Slot))
				{
					BorderSlot->SetHorizontalAlignment(HAlign_Fill);
					BorderSlot->SetVerticalAlignment(VAlign_Fill);
				}
				if(UCanvasPanelSlot* CanvasPanelSlot= Cast<UCanvasPanelSlot>(WidgetInstance->Slot))
				{
					CanvasPanelSlot->SetAnchors(FAnchors(0,0,1,1));
					CanvasPanelSlot->SetAlignment(FVector2D::Zero());
					CanvasPanelSlot->SetOffsets(FMargin(0,0,0,0));
				}
				
				return Cast<ActivatableWidgetT>(WidgetInstance);
			}
		}
		return nullptr;
	}

protected:
	virtual void NativeDestruct() override;
};


