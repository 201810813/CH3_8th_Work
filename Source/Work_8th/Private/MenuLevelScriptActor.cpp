// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuLevelScriptActor.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraActor.h"

AMenuLevelScriptActor::AMenuLevelScriptActor()
{
	MenuWidget = nullptr;
	MenuWidgetInstance = nullptr;
}

void AMenuLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerController->SetInputMode(FInputModeUIOnly());
		TArray<AActor*> FoundCamera;
		(UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), FoundCamera));
		if (FoundCamera.Num() > 0)
		{
			PlayerController->SetViewTarget(FoundCamera[0]);
		}
		if (MenuWidget)
		{
			MenuWidgetInstance = Cast<UUserWidget>(CreateWidget(GetWorld(), MenuWidget));
			if (MenuWidgetInstance)
			{
				MenuWidgetInstance->AddToViewport();
				PlayerController->SetShowMouseCursor(true);
			}
		}
	}
}
