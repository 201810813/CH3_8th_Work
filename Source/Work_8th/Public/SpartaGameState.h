// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SpartaGameState.generated.h"


UCLASS()
class WORK_8TH_API ASpartaGameState : public AGameState
{
	GENERATED_BODY()
	ASpartaGameState();

	
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 SpawnedCoin;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoin;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	float LevelDuration;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	int32 IncreaseItemNum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	int32 BaseItemNum;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelNames;
	
	FTimerHandle LevelTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);

	void OnGameOver();
	void OnLevelTimeUp();
	void OnCoinCollected();
	void StartLevel();
	void EndLevel();
	void UpdateHUD();
};

