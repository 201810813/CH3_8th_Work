// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpartaPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;

UCLASS()
class WORK_8TH_API ASpartaPlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:
	ASpartaPlayerController();

	FTimerHandle DebuffTimerHandle;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Input")
	UInputMappingContext* MappingContext;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Input")
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Input")
	UInputAction* LookAction;


	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="HUD")
	UUserWidget* HUDWidgetInstance;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="GameOver")
	TSubclassOf<UUserWidget> GameOverWidgetClass;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="GameOver")
	UUserWidget* GameOverWidgetInstance;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameHUD();
	UFUNCTION(BlueprintCallable, Category = "GameOver")
	void ShowGameOver();
	UFUNCTION(BlueprintCallable, Category = "GameOver")
	void ReStartGame();
	UFUNCTION(BlueprintCallable, Category = "GameOver")
	void ToMainMenu();
	UFUNCTION(BlueprintCallable, Category = "GameOver")
	void ExitGame();
	UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetHUDWidgetInstance() const;
	
protected:
	virtual void BeginPlay() override;
};
