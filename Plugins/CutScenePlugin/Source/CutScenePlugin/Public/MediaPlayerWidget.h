// Copyright 2023 Invictus Games. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
//#include "MediaPlayer.h"
#include "CommonActivatableWidget.h"
//#include "CommonButtonBase.h"
#include "Blueprint/UserWidget.h"
#include "MediaPlayerWidget.generated.h"

class UMediaSoundComponent;
////class UCommonButtonBase;
//class UMudfestButtonBase;
class UImage;
class UMediaTexture;
class UScaleBox;
class USizeBox;
DECLARE_DELEGATE(FMediaFinished)

class UMediaSource;
class UMediaPlayer;
/**
 * 
 */
UCLASS()
class CUTSCENEPLUGIN_API UMediaPlayerWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

	//UPROPERTY(EditAnywhere, meta = (BindWidget))
	//UCommonButtonBase* SkipButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScaleBox* ScaleBox;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USizeBox* SizeBox;

	UPROPERTY(VisibleAnywhere)
	bool bSizeSetted;
	UPROPERTY(VisibleAnywhere)
	int Counter;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Image;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UMaterialInterface> SoftMaterial;
	UPROPERTY(EditAnywhere)
	UMaterialInstanceDynamic* Material;
	UPROPERTY(EditAnywhere)
	UMaterialInstanceDynamic* MaterialInterface;
	UPROPERTY(EditAnywhere,Instanced)
	UMediaPlayer* MediaPlayer;
	UPROPERTY(EditAnywhere,Instanced)
	UMediaTexture* MediaTexture;

	UPROPERTY()
	UMediaSoundComponent* MediaSoundCache;

public:
	UMediaSoundComponent* GetMediaSound();

	UFUNCTION()
	void OnMediaPlayerMediaOpened( FString OpenedUrl);
	UFUNCTION()
	void OnMediaPlayerMediaOpenedFailed( FString OpenedUrl);
	UFUNCTION()
	void OnEndReached();

	void StopMedia();

public:

	void Play(UMediaSource* MediaSource);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//virtual void NativeConstruct() override;

public:
	FMediaFinished OnMediaFinished;
};
