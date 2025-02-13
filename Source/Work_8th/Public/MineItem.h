// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "MineItem.generated.h"

/**
 * 
 */
UCLASS()
class WORK_8TH_API AMineItem : public ABaseItem
{
	GENERATED_BODY()
public:
	AMineItem();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 MineDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float MineDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float MineRadius;

	FTimerHandle MineExplodeTimer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Component")
	USphereComponent* MineCollision;
	
	virtual void ActivateItem(AActor* Activator) override;
	void Explode();
};
