// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaGameState.h"
#include "SpartaGameInstance.h"
#include "CoinItem.h"
#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
ASpartaGameState::ASpartaGameState()
{
	Score = 0;
	CollectedCoin = 0;
	SpawnedCoin = 0;
	LevelDuration = 30.0f;
	CurrentLevelIndex = 0;
	MaxLevel = 3;
	IncreaseItemNum = 10;
	BaseItemNum = 50;
}

void ASpartaGameState::BeginPlay()
{
	Super::BeginPlay();
	StartLevel();
	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ASpartaGameState::UpdateHUD,
		0.1f,
		true
		);
}

int32 ASpartaGameState::GetScore() const
{
	return Score;
}

void ASpartaGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance= GetWorld()->GetGameInstance())
	{
		 if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		 {
			 SpartaGameInstance->AddToScore(Amount);
		 }
	}
	Score += Amount;
}

void ASpartaGameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaPlayerController->ShowGameHUD();
		}
	}
	if (UGameInstance* GameInstance= GetWorld()->GetGameInstance())
	{
		if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		{
			CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;
		}
	}
	SpawnedCoin = 0;
	CollectedCoin = 0;

	TArray<AActor*> FoundVolume;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(),FoundVolume);
	
	for (int i = 0; i < BaseItemNum; i++)
	{
		if (FoundVolume.Num()>0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolume[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoin++;
				}
			}
		}
	}
	UpdateHUD();
	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&ASpartaGameState::OnLevelTimeUp,
		LevelDuration,
		false
	);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Level %d Start!"), CurrentLevelIndex + 1));
}

void ASpartaGameState::OnLevelTimeUp()
{
	EndLevel();
}

void ASpartaGameState::OnCoinCollected()
{
	CollectedCoin++;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Collected Coin : %d / %d"),CollectedCoin, SpawnedCoin));
	if (CollectedCoin >= SpawnedCoin && SpawnedCoin > 0)
	{
		EndLevel();
	}
}

void ASpartaGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	
	if (UGameInstance* GameInstance= GetWorld()->GetGameInstance())
	{
		if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		{
			BaseItemNum += IncreaseItemNum;
			CurrentLevelIndex++;
			SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}
	
	if (CurrentLevelIndex >= MaxLevel)
	{
		OnGameOver();
		return;
	}
	if (LevelNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelNames[CurrentLevelIndex]);
	}
	else{OnGameOver();}
}

void ASpartaGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidgetInstance())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TimeValue"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("ScoreValue"))))
				{
					if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
					{
						if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %i"), SpartaGameInstance->TotalScore)));
						}
					}
				}

				if (UTextBlock* LevelText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
					{
						if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
						{
							LevelText->SetText(FText::FromString(FString::Printf(TEXT("Level: %i"), SpartaGameInstance->CurrentLevelIndex + 1)));
						}
					}
				}
				//체력, 스태미나 프로그레스 바 
				if (ACharacter* Character = GetWorld()->GetFirstPlayerController()->GetCharacter())
				{
					if (ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(Character))
					{
						if (UProgressBar* ProgressBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("HPBar"))))
						{
							ProgressBar->SetPercent(SpartaCharacter->GetHealth()/100.0f);
						}
						if (UProgressBar* ProgressBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("StaminaBar"))))
						{
							ProgressBar->SetPercent(SpartaCharacter->GetStamina()/100.0f);
						}
					}
				}
				
			}
		};
	}
}

void ASpartaGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaPlayerController->SetPause(true);
			SpartaPlayerController->ShowGameOver();
		}
	}
}
