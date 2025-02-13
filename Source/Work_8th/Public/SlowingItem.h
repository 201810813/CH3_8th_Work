// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "SlowingItem.generated.h"

/**
 * 
 */
UCLASS()
class WORK_8TH_API ASlowingItem : public ABaseItem
{
	GENERATED_BODY()
	ASlowingItem();
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Debuff")
	float SlowRate;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Debuff")
	float SlowDuration;
	FTimerHandle DebuffTimer;
protected:
	UFUNCTION()
	virtual void ActivateItem(AActor* Activator) override;
};
