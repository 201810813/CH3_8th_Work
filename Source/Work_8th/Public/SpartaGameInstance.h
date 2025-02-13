// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SpartaGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class WORK_8TH_API USpartaGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	USpartaGameInstance();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex;

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount);
};
