// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "ReversControlItem.generated.h"


UCLASS()
class WORK_8TH_API AReversControlItem : public ABaseItem
{
	GENERATED_BODY()
	AReversControlItem();
	float Duration;
public:
	FTimerHandle DebuffTimerHandle;
	
	virtual void ActivateItem(AActor* Activator) override;

};
