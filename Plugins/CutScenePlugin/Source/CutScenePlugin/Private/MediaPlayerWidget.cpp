// Copyright 2023 Invictus Games. All Rights Reserved.


#include "MediaPlayerWidget.h"

#include "CutSceneManager.h"
#include "MediaPlayer.h"
#include "MediaSource.h"
#include "MediaTexture.h"
//#include "../../../../../Source/Mudfest/Public/UI/MudfestButtonBase.h"
#include "CommonButtonBase.h"
#include "Components/Image.h"
#include "Components/ScaleBox.h"
#include "Components/SizeBox.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Components/SizeBox.h"
#include "CommonButtonBase.h"
#include "CommonButtonBase.h"
#include "MediaSoundComponent.h"

UMediaSoundComponent* UMediaPlayerWidget::GetMediaSound()
{
	if(IsValid(MediaSoundCache) == false)
	{
		if(UWorld* World = GetWorld())
		{
			if(UCutSceneManager* CutSceneManager = World->GetSubsystem<UCutSceneManager>())
			{
				MediaSoundCache = CutSceneManager->GetMediaSound();
			}
		}
	}
	return MediaSoundCache;
}

void UMediaPlayerWidget::OnMediaPlayerMediaOpened(FString OpenedUrl)
{
	MediaPlayer->OnMediaOpenFailed.RemoveDynamic(this,&UMediaPlayerWidget::OnMediaPlayerMediaOpenedFailed);
	MediaPlayer->OnMediaOpened.RemoveDynamic(this,&UMediaPlayerWidget::OnMediaPlayerMediaOpened);

	if(MediaPlayer->Play())
	{
		MediaPlayer->OnEndReached.AddUniqueDynamic(this, &UMediaPlayerWidget::OnEndReached);

		MediaTexture->UpdateResource();
	}
	else
	{
		OnEndReached();
	}

}

void UMediaPlayerWidget::OnMediaPlayerMediaOpenedFailed(FString OpenedUrl)
{
	MediaPlayer->OnMediaOpenFailed.RemoveDynamic(this,&UMediaPlayerWidget::OnMediaPlayerMediaOpenedFailed);
	MediaPlayer->OnMediaOpened.RemoveDynamic(this,&UMediaPlayerWidget::OnMediaPlayerMediaOpened);
	OnEndReached();
}

void UMediaPlayerWidget::OnEndReached()
{
	MediaPlayer->OnMediaOpenFailed.RemoveDynamic(this,&UMediaPlayerWidget::OnMediaPlayerMediaOpenedFailed);
	MediaPlayer->OnMediaOpened.RemoveDynamic(this,&UMediaPlayerWidget::OnMediaPlayerMediaOpened);
	MediaPlayer->OnEndReached.RemoveDynamic(this, &UMediaPlayerWidget::OnEndReached);

	MediaPlayer->Close();

	OnMediaFinished.ExecuteIfBound();
}

void UMediaPlayerWidget::StopMedia()
{
	if(IsValid(MediaPlayer))
	{
		MediaPlayer->Close();	
	}
}

void UMediaPlayerWidget::Play(UMediaSource* MediaSource)
{
	const UCutSceneManagerSettings* Settings = GetDefault<UCutSceneManagerSettings>();

	bSizeSetted = false;
	Counter = 0;
	if(IsValid(MediaPlayer) == false)
		MediaPlayer = NewObject<UMediaPlayer>(this);
	if(IsValid(MediaTexture) == false)
		MediaTexture = NewObject<UMediaTexture>(this);
	if(IsValid(MediaPlayer) && IsValid(MediaTexture))
	{
		MediaPlayer->SetRate(1.f);
		MediaTexture->SetMediaPlayer(MediaPlayer);
		MediaTexture->UpdateResource();
		if(IsValid(GetMediaSound()))
		{
			GetMediaSound()->SetMediaPlayer(MediaPlayer);
		}
	}

	if(ScaleBox)
	{
		if (IsValid(Settings))
		{
			ScaleBox->SetStretch(Settings->Stretch);
		}
	}
	if(Image)
	{
		if(IsValid(MaterialInterface) == false)
		{
			if(UMaterialInterface* Temp = SoftMaterial.LoadSynchronous())
			{
				MaterialInterface = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(),Temp);
			}
		}
		if(IsValid(MaterialInterface))
		{
			FString aStr(TEXT("VideoTexture"));
			FName aName = *aStr;
			MaterialInterface->SetTextureParameterValue(aName,MediaTexture);
			Image->SetBrushFromMaterial(MaterialInterface);
		}

	}
	if(IsValid(MediaSource) == false)
	{
		return ;
	}

	if(MediaPlayer->OpenSource(MediaSource))
	{
		MediaPlayer->OnMediaOpenFailed.AddUniqueDynamic(this,&UMediaPlayerWidget::OnMediaPlayerMediaOpenedFailed);
		MediaPlayer->OnMediaOpened.AddUniqueDynamic(this,&UMediaPlayerWidget::OnMediaPlayerMediaOpened);
	}
	else
	{
		OnEndReached();
	}
}

void UMediaPlayerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if(IsValid(MediaPlayer) && IsValid(MediaTexture) &&MediaPlayer->IsPlaying())
	{
		int H =MediaTexture->GetHeight();
		int W =MediaTexture->GetWidth();
		

		if(bSizeSetted == false)
		{
			if( H > 100 && W > 100)
			{
				bSizeSetted = true;
				if(SizeBox)
				{
					SizeBox->SetHeightOverride(H);
					SizeBox->SetWidthOverride(W);
				}
				//if(GEngine)
				//	GEngine->AddOnScreenDebugMessage(-1, InDeltaTime, FColor::Yellow, FString::Printf(TEXT("Video Dimension %d * %d Counter : %d"), W,H, Counter));
			}
		}
		else
		{
			Counter++;
		}
		//if(GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, InDeltaTime, FColor::Yellow, FString::Printf(TEXT("Video Dimension %d * %d"), W,H));
	}
}

//void UMediaPlayerWidget::NativeConstruct()
//{
//	Super::NativeConstruct();
//	UCommonButtonBase* But;
//	But.basec
//	if(SkipButton)
//	{
//
//	}
//}
