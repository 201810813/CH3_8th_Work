// Fill out your copyright notice in the Description page of Project Settings.
#include "SpartaPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "SpartaGameInstance.h"
#include "SpartaGameState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"



ASpartaPlayerController::ASpartaPlayerController() : MappingContext(nullptr),
														MoveAction(nullptr),
														JumpAction(nullptr),
														SprintAction(nullptr),
														LookAction(nullptr),
														HUDWidgetInstance(nullptr),
														HUDWidgetClass(nullptr),
														GameOverWidgetClass(	nullptr),
														GameOverWidgetInstance(nullptr)
{}


void ASpartaPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (MappingContext)
			{
				SubSystem->AddMappingContext(MappingContext,0);
			}
		}
	}
	
	ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
	if (SpartaGameState)
	{
		SpartaGameState->UpdateHUD();
	}

	FString CurrentLevelName = GetWorld()->GetMapName();
	if (CurrentLevelName.Contains(TEXT("GameOverLevel")))
	{
		ShowGameOver();
	}
}

void ASpartaPlayerController::ShowGameHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}
	if (GameOverWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
	}
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->AddToViewport();
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
	if (ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr)
	{
		SpartaGameState->UpdateHUD();
	}
}

void ASpartaPlayerController::ShowGameOver()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}
	if (GameOverWidgetInstance)
	{
		GameOverWidgetInstance->RemoveFromParent();
		GameOverWidgetInstance = nullptr;
	}
	if (GameOverWidgetClass)
	{
		GameOverWidgetInstance = CreateWidget<UUserWidget>(this, GameOverWidgetClass);
		if (GameOverWidgetInstance)
		{
			GameOverWidgetInstance->AddToViewport();
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}

		if (UTextBlock* TextBlock = Cast<UTextBlock>(GameOverWidgetInstance->GetWidgetFromName(TEXT("GameOverText"))))
			if (UFunction* PlayAnimFunc = GameOverWidgetInstance->FindFunction(TEXT("PlayAinmGameOver")))
			{
				GameOverWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
			}
		if (UTextBlock* TextBlock = Cast<UTextBlock>(GameOverWidgetInstance->GetWidgetFromName(TEXT("ScoreText"))))
		{
			if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
			{
				TextBlock->SetText(FText::FromString(FString::Printf(TEXT("Score: %i"),SpartaGameInstance->TotalScore)));
			}
		}
		
		if (UTextBlock* TextBlock = Cast<UTextBlock>(GameOverWidgetInstance->GetWidgetFromName(TEXT("ReStartButtonText"))))
		{
			TextBlock->SetText(FText::FromString(TEXT("재시작")));
		}
	
		if (UTextBlock* TextBlock = Cast<UTextBlock>(GameOverWidgetInstance->GetWidgetFromName(TEXT("MenuTExt"))))
		{
			TextBlock->SetText(FText::FromString(TEXT("메인 메뉴")));
		}
		if (UTextBlock* TextBlock = Cast<UTextBlock>(GameOverWidgetInstance->GetWidgetFromName(TEXT("EixtText"))))
		{
			TextBlock->SetText(FText::FromString(TEXT("게임 종료")));
		}
	}
}

void ASpartaPlayerController::ReStartGame()
{
	if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		SpartaGameInstance->CurrentLevelIndex=0;
		SpartaGameInstance->TotalScore=0;
	}
	if (ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr)
	{
		SpartaGameState->BaseItemNum = 40;
	}
		
	UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
}



void ASpartaPlayerController::ToMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenuLevel"));
}

void ASpartaPlayerController::ExitGame()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		
			UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
	}
}

UUserWidget* ASpartaPlayerController::GetHUDWidgetInstance() const
{
	return HUDWidgetInstance;
}
