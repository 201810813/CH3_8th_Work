// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MenuLevelScriptActor.generated.h"


UCLASS()
class WORK_8TH_API AMenuLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	AMenuLevelScriptActor();
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UUserWidget> MenuWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	UUserWidget* MenuWidgetInstance;
	
};
