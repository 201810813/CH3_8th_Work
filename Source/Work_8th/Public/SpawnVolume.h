// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemSpawnRow.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;


UCLASS()
class WORK_8TH_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	UBoxComponent* Box;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	UDataTable* ItemDataTable;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	AActor* SpawnRandomItem();
	
	FVector GetRandomPointInVolume() const;
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass);
	FItemSpawnRow* GetRandomItem() const;
};
