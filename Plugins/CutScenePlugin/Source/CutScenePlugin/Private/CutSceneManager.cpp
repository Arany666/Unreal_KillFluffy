// Fill out your copyright notice in the Description page of Project Settings.


#include "CutSceneManager.h"

#include "CommonLocalPlayer.h"
#include "CutSceneSkipWidget.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MediaPlayerWidget.h"
#include "MediaSoundComponent.h"
#include "MediaSource.h"
#include "MGameTypes.h"
#include "PrimaryGameLayout.h"

UCutSceneManager::UCutSceneManager()
{
	bIsCreateOnRunning = GIsRunning;
}

void UCutSceneManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool UCutSceneManager::IsTickable() const
{
	//return Super::IsTickable();
	return  bIsCreateOnRunning;
}

TStatId UCutSceneManager::GetStatId() const
{
	//return Super::GetStatId();
	RETURN_QUICK_DECLARE_CYCLE_STAT(UCutSceneManager, STATGROUP_Tickables);
}

ALevelSequenceActor* UCutSceneManager::GetCutSceneSequenceActor()
{
	if (IsValid(CutSceneSequenceActor) == false)
	{
		if(UWorld* World = GetWorld())
		{
			FMovieSceneSequencePlaybackSettings settings;

			UClass* Ref = nullptr;
			const UCutSceneManagerSettings* Settings = GetDefault<UCutSceneManagerSettings>();
			if (IsValid(Settings))
			{
				Ref = Settings->LevelSequenceActorClasss.LoadSynchronous();
			}
			CutSceneSequenceActor = World->SpawnActor<ALevelSequenceActor>(Ref != nullptr ? Ref : ALevelSequenceActor::StaticClass());
			CutSceneSequenceActor->bReplicatePlayback = true;
			CutSceneSequenceActor->bRelevantForNetworkReplays = true;
			CutSceneSequenceActor->PlaybackSettings = settings;
		}
	}
	return CutSceneSequenceActor;
}


void UCutSceneManager::OnCutScenesSequenceFinished()
{
	RemoveCutSceneSkipWidget();
	if(IsValid(MediaPlayerWidgetCache))
	{
		MediaPlayerWidgetCache->OnMediaFinished.Unbind();
		//MediaPlayerWidgetCache->RemoveFromParent();
		MediaPlayerWidgetCache->DeactivateWidget();
		MediaPlayerWidgetCache = nullptr;
	}

	

	const UCutSceneManagerSettings* Settings = GetDefault<UCutSceneManagerSettings>();
	if (IsValid(Settings) && Settings->DeastroyActorOnFinish)
	{
		DestroyCutSceneActor();
	}

	if (IsValid(CutSceneSequenceActor))
	{
		if (ULevelSequencePlayer* CutSceneSequencePlayer = CutSceneSequenceActor->SequencePlayer)
		{
			CutSceneSequencePlayer->OnFinished.RemoveDynamic(this, &UCutSceneManager::OnCutScenesSequenceFinished);
		}
	}

	//CutSceneSequenceActor = nullptr;

	if(LevelSequencesCache.Num() > 0)
		LevelSequencesCache.RemoveAt(0);
	if(LevelSequencesCache.Num() > 0)
	{
		PlayCutSceneInner(LevelSequencesCache[0]);
	}
	else
	{
		if(OnFCutSceneEnded.IsBound())
			OnFCutSceneEnded.Broadcast();
		OnFinished.ExecuteIfBound();
	}
	FadeCameraOut();
}

void UCutSceneManager::DestroyCutSceneActor()
{
	if (IsValid(CutSceneSequenceActor) == false) return;
	StopCutScene();
	CutSceneSequenceActor->Destroy();
	
}

void UCutSceneManager::PlayCutSceneInner(ULevelSequence* LevelSequence, bool Loop)
{
	
	//if (IsValid(CutSceneSequenceActor))
	//{
	//	if (ULevelSequencePlayer* CutSceneSequencePlayer = CutSceneSequenceActor->SequencePlayer)
	//	{
	//		CutSceneSequencePlayer->OnFinished.RemoveDynamic(this, &UCutSceneManager::OnCutScenesSequenceFinished);
	//		CutSceneSequencePlayer->Stop();
	//		CutSceneSequenceActor = nullptr;
	//	}
	//}
	if( IsValid(LevelSequence) && IsValid( GetCutSceneSequenceActor()))
	{
		ULevelSequencePlayer* CutSceneSequencePlayer = GetCutSceneSequenceActor()->SequencePlayer;

		if (CutSceneSequencePlayer != nullptr)
		{
			CutSceneSequencePlayer->OnFinished.RemoveDynamic(this, &UCutSceneManager::OnCutScenesSequenceFinished);
			CutSceneSequencePlayer->Stop();
			CutSceneSequencePlayer->OnFinished.AddDynamic(this, &UCutSceneManager::OnCutScenesSequenceFinished);

			CutSceneSequenceActor->SetSequence(LevelSequence);
			if (Loop == true)
				CutSceneSequencePlayer->PlayLooping();
			else
				CutSceneSequencePlayer->Play();
		}
		else
		{
			OnCutScenesSequenceFinished();
		}
	}
	else
	{
		OnCutScenesSequenceFinished();
	}
}

void UCutSceneManager::PlayCutSceneFade()
{
	SetFade(EMCameraFadeState::CFS_FadingIn);
}

void UCutSceneManager::PlayCutScene(TArray<ULevelSequence*> LevelSequences, FFinished Del, bool SkipButton)
{

	OnFinished = std::move(Del);
	LevelSequencesCache = std::move(LevelSequences);
	LoopCache = false;
	SkipButtonCache = SkipButton;

	//PlayCutSceneFade();
	
	if (LevelSequencesCache.IsEmpty())
	{
		OnCutScenesSequenceFinished();
		return;
	}

	if (SkipButton)
		GetCutSceneSkipWidget();
	
	OnFCutSceneStarted.Broadcast();

	PlayCutSceneInner(LevelSequencesCache[0],false);
}

void UCutSceneManager::PlayCutScene(ULevelSequence* LevelSequence, FFinished Del, bool SkipButton, bool Loop)
{

	OnFinished = std::move(Del);
	LevelSequencesCache.Reset();
	LevelSequencesCache.AddUnique(LevelSequence);
	LoopCache = Loop;
	SkipButtonCache = SkipButton;

	//PlayCutSceneFade();

	if ( !IsValid(LevelSequence) )
	{
		OnCutScenesSequenceFinished();
		return;
	}
	
	if (SkipButton)
		GetCutSceneSkipWidget();
	
	OnFCutSceneStarted.Broadcast();

	PlayCutSceneInner(LevelSequence, Loop);
}

void UCutSceneManager::StopCutScene()
{
	LevelSequencesCache.Empty();
	SkipCutScene();
}

void UCutSceneManager::SkipCutScene()
{
	if (IsValid(CutSceneSequenceActor))
	{
		if (ULevelSequencePlayer* CutSceneSequencePlayer = CutSceneSequenceActor->SequencePlayer)
		{
			CutSceneSequencePlayer->Stop();
		}
	}
	if(IsValid(MediaPlayerWidgetCache))
	{
		MediaPlayerWidgetCache->StopMedia();
	}
	OnCutScenesSequenceFinished();
}

bool UCutSceneManager::GetIsPlaying() const
{
	if(CutSceneSequenceActor && CutSceneSequenceActor->GetSequencePlayer())
	{
		return CutSceneSequenceActor->GetSequencePlayer()->IsPlaying();
	}

	return false;
}


#pragma endregion Meadi Player

UMediaPlayerWidget* UCutSceneManager::GetMediaPlayerWidget()
{
	if(IsValid(MediaPlayerWidgetCache) == false)
	{
		if(UWorld* World = GetWorld())
		{
			const UCutSceneManagerSettings* Settings = GetDefault<UCutSceneManagerSettings>();
			if (IsValid(Settings))
			{
				if(TSubclassOf<UMediaPlayerWidget> Class = Settings->MediaPlayerWidgetClasss.LoadSynchronous())
				{

					if (APlayerController* Pc = GetWorld()->GetFirstPlayerController())
					{
						if (UCommonLocalPlayer* LocalPlayer = Cast<UCommonLocalPlayer>(Pc->GetLocalPlayer()))
						{
							if (UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(LocalPlayer))
							{
								MediaPlayerWidgetCache = Cast<UMediaPlayerWidget>(RootLayout->PushWidgetToLayerStack(TAG_UI_LAYER_MODAL, Class));
								//MediaPlayerWidgetCache = Cast<UMediaPlayerWidget>(RootLayout->PushWidgetToLayerStack(TAG_UI_LAYER_GAME_MENU, Class));
							}
						}
					}
				}
			}
		}
	}
	return MediaPlayerWidgetCache;
}

void UCutSceneManager::RemoveMediaPlayerWidget()
{
}

void UCutSceneManager::PlayMediaInner(UMediaSource* MediaSource)
{
	if(IsValid(GetMediaPlayerWidget()))
	{
		GetMediaPlayerWidget()->OnMediaFinished.Unbind();
		GetMediaPlayerWidget()->OnMediaFinished.BindUObject(this, &UCutSceneManager::OnCutScenesSequenceFinished);
		GetMediaPlayerWidget()->Play(MediaSource);
	}
	else
	{
		OnCutScenesSequenceFinished();
	}
}


void UCutSceneManager::PlayMedia(UMediaSource* MediaSource, FFinished Del, bool SkipButton)
{
	OnFinished = std::move(Del);

	if ( !IsValid(MediaSource) )
	{
		OnCutScenesSequenceFinished();
		return;
	}


	OnFCutSceneStarted.Broadcast();
	
	PlayMediaInner(MediaSource);
}

void UCutSceneManager::PlayMedia(UMediaSource* MediaSource)
{
	PlayMedia(MediaSource,nullptr,true);
}

#pragma endregion Meadi Player

#pragma region CutSceneSkipWidget



UCutSceneSkipWidget* UCutSceneManager::GetCutSceneSkipWidget()
{
	{
		if(UWorld* World = GetWorld())
		{
			const UCutSceneManagerSettings* Settings = GetDefault<UCutSceneManagerSettings>();
			if (IsValid(Settings))
			{
				if (Settings->UseCutSceneSkipWidget == true)
				{
					UClass* Ref = Settings->CutSceneSkipWidgetClass.LoadSynchronous();
					if(IsValid(Ref))
					{

						if (APlayerController* Pc = GetWorld()->GetFirstPlayerController())
						{
							if (UCommonLocalPlayer* LocalPlayer = Cast<UCommonLocalPlayer>(Pc->GetLocalPlayer()))
							{
								if (UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(LocalPlayer))
								{
									CutSceneSkipWidgetCache = Cast<UCutSceneSkipWidget>(RootLayout->PushWidgetToLayerStack(TAG_UI_LAYER_MODAL, Ref));
									//CutSceneSkipWidgetCache = Cast<UCutSceneSkipWidget>(RootLayout->PushWidgetToLayerStack(TAG_UI_LAYER_GAME_MENU, Ref));
								}
							}
						}
					}
					
				}
			}}
	}

	return CutSceneSkipWidgetCache;
}

void UCutSceneManager::RemoveCutSceneSkipWidget()
{
	if(IsValid(CutSceneSkipWidgetCache) )
	{
		CutSceneSkipWidgetCache->DeactivateWidget();
	}
}

APlayerCameraManager* UCutSceneManager::GetCameraManager()
{
	if(IsValid(CameraManagerCache) == false)
	{
		if(UWorld* World = GetWorld())
		{
			if(APlayerController* PC = World->GetFirstPlayerController())
			{
				CameraManagerCache = PC->PlayerCameraManager;
			}
		}
	}
	return CameraManagerCache;
}

void UCutSceneManager::FadeCameraIn()
{
	if(UWorld* World = GetWorld())
	{
		if(APlayerController* PC = World->GetFirstPlayerController())
		{
			if(APlayerCameraManager* CameraManager = PC->PlayerCameraManager)
			{
				if(CameraManager->FadeAmount < 0.1f)
					CameraManager->StartCameraFade(0.f,1.f,2.f,FLinearColor::Black,false,true);
			}
		}
	}
}

void UCutSceneManager::FadeCameraOut()
{
	if(UWorld* World = GetWorld())
	{
		if(APlayerController* PC = World->GetFirstPlayerController())
		{
			if(APlayerCameraManager* CameraManager = PC->PlayerCameraManager)
			{
				if(CameraManager->FadeAmount > 0.9f)
					CameraManager->StartCameraFade(1.f,0.f,1.f,FLinearColor::Black,false,true);
			}
		}
	}
}

void UCutSceneManager::FadeTick(float DeltaTime)
{
	if(FadeStateCache == EMCameraFadeState::CFS_FadingIn)
	{
		if(IsValid(GetCameraManager()))
		{
			if(GetCameraManager()->FadeAmount >= 1.f)
			{
				SetFade(EMCameraFadeState::CFS_Faded);
			}
		}
	}
	else if(FadeStateCache == EMCameraFadeState::CFS_Fadingout)
	{
		if(IsValid(GetCameraManager()))
		{
			if(GetCameraManager()->FadeAmount <= 0.f)
			{
				SetFade(EMCameraFadeState::CFS_None);
			}
		}
	}
}

void UCutSceneManager::SetFade(EMCameraFadeState FadeState)
{
	FadeStateCache = FadeState;
	if(FadeStateCache == EMCameraFadeState::CFS_FadingIn)
	{
		if(IsValid(GetCameraManager()))
		{
			GetCameraManager()->StartCameraFade(0.f,1.f,2.f,FLinearColor::Black,false,true);
		}
	}
	else if(FadeStateCache == EMCameraFadeState::CFS_Fadingout)
	{
		if(IsValid(GetCameraManager()))
		{
			GetCameraManager()->StartCameraFade(1.f,0.f,2.f,FLinearColor::Black,false,true);
		}
	}
	else if(FadeStateCache == EMCameraFadeState::CFS_Faded)
	{
		SetFade(EMCameraFadeState::CFS_Fadingout);
	}
}




#pragma endregion CutSceneSkipWidget

#pragma region Audio

UMediaSoundComponent* UCutSceneManager::GetMediaSound()
{
	if(IsValid(MediaSoundCache) == false)
	{
		if(UWorld* World = GetWorld())
		{
			if(APlayerController* PC = World->GetFirstPlayerController())
			{
				MediaSoundCache = NewObject<UMediaSoundComponent>(PC);
				MediaSoundCache->RegisterComponent();
				FAttachmentTransformRules Rules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
				MediaSoundCache->AttachToComponent(PC->GetRootComponent(),Rules);
			}
		}
	}
	return MediaSoundCache;
}

#pragma endregion Audio


