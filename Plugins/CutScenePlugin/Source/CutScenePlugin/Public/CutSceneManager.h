// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Widgets/Layout/SScaleBox.h"
#include "CutSceneManager.generated.h"

class UMediaSoundComponent;

UENUM(BlueprintType)
enum class EMCameraFadeState : uint8
{
	CFS_None,
	CFS_FadingIn,
	CFS_Faded,
	CFS_Fadingout
};

class UMediaPlayerWidget;
class UMediaSource;
class ALevelSequenceActor;
class ULevelSequence;
class UCutSceneSkipWidget;

UCLASS(config = Game, defaultconfig, meta = (DisplayName = "CutSceneManager Settings"))
class CUTSCENEPLUGIN_API UCutSceneManagerSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:

	UPROPERTY(Config, EditDefaultsOnly,BlueprintReadOnly)
	TSoftClassPtr<ALevelSequenceActor> LevelSequenceActorClasss;
	UPROPERTY(Config, EditDefaultsOnly,BlueprintReadOnly)
	bool DeastroyActorOnFinish = false;

	UPROPERTY(Config, EditDefaultsOnly,BlueprintReadOnly)
	TSoftClassPtr<UCutSceneSkipWidget> CutSceneSkipWidgetClass;
	UPROPERTY(Config, EditDefaultsOnly,BlueprintReadOnly)
	bool UseCutSceneSkipWidget = false;

	UPROPERTY(Config, EditDefaultsOnly,BlueprintReadOnly)
	//EStretch::Type Stretch = EStretch::Type::ScaleToFitX;
	TEnumAsByte<EStretch::Type> Stretch = EStretch::Type::ScaleToFitX;
	UPROPERTY(Config, EditDefaultsOnly,BlueprintReadOnly)
	TSoftClassPtr<UMediaPlayerWidget> MediaPlayerWidgetClasss;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCutSceneEventSig);
DECLARE_DELEGATE(FFinished)

UCLASS()
class CUTSCENEPLUGIN_API UCutSceneManager : public UTickableWorldSubsystem
{
	GENERATED_BODY()


public:

	UCutSceneManager();

	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;
	
protected:
	bool bIsCreateOnRunning = false;

private:
	UPROPERTY(Transient, VisibleAnywhere)
	ALevelSequenceActor* CutSceneSequenceActor;

	ALevelSequenceActor* GetCutSceneSequenceActor();
	UPROPERTY()
	TArray<ULevelSequence*> LevelSequencesCache;
	UPROPERTY()
	ULevelSequence* LevelSequencCache;

	FFinished OnFinished;

	UPROPERTY()
	bool SkipButtonCache = false;
	UPROPERTY()
	bool LoopCache = false;

	UFUNCTION()
		void OnCutScenesSequenceFinished();

	void DestroyCutSceneActor();
	void PlayCutSceneInner(ULevelSequence* LevelSequence, bool Loop = false);
	void PlayCutSceneFade();

public:

	void PlayCutScene(ULevelSequence* LevelSequence, FFinished Del, bool SkipButton = true, bool Loop = false);
	void PlayCutScene(TArray<ULevelSequence*> LevelSequences, FFinished Del, bool SkipButton = true);

	void StopCutScene();
	UFUNCTION(BlueprintCallable)
	void SkipCutScene();

	bool GetIsPlaying() const;
	
	UPROPERTY(BlueprintAssignable)
		FCutSceneEventSig OnFCutSceneStarted;
	UPROPERTY(BlueprintAssignable)
		FCutSceneEventSig OnFCutSceneEnded;

#pragma endregion Meadi Player

private:

	UPROPERTY()
	UMediaPlayerWidget* MediaPlayerWidgetCache;

	UMediaPlayerWidget* GetMediaPlayerWidget();
	void RemoveMediaPlayerWidget();
	
	void PlayMediaInner(UMediaSource* MediaSource);
	//void OnMediaFinished();

public:

	void PlayMedia(UMediaSource* MediaSource, FFinished Del, bool SkipButton = true);
	UFUNCTION(BlueprintCallable)
	void PlayMedia(UMediaSource* MediaSource);

#pragma endregion Meadi Player


#pragma region CutSceneSkipWidget

private:

	UPROPERTY()
	UCutSceneSkipWidget* CutSceneSkipWidgetCache;

	UCutSceneSkipWidget* GetCutSceneSkipWidget();
	void RemoveCutSceneSkipWidget();


#pragma endregion CutSceneSkipWidget


#pragma region Camera Fade

private:

	UPROPERTY()
	EMCameraFadeState FadeStateCache;
	UPROPERTY()
	APlayerCameraManager* CameraManagerCache;
	APlayerCameraManager* GetCameraManager();

	void FadeCameraIn();
	void FadeCameraOut();

	void FadeTick(float DeltaTime);

public:

	void SetFade(EMCameraFadeState FadeState);

#pragma endregion Camera Fade


#pragma endregion Audio

private:

	UPROPERTY()
	AActor* MediaSoundHolder;

	UPROPERTY()
	UMediaSoundComponent* MediaSoundCache;

public:
	UMediaSoundComponent* GetMediaSound();



#pragma endregion Audio

};
