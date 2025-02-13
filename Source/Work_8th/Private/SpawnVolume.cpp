// Fill out your copyright notice in the Description page of Project Settings.
#include "SpawnVolume.h"
#include "Components/BoxComponent.h"


// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(Root);
	ItemDataTable = nullptr;
}

AActor* ASpawnVolume::SpawnRandomItem()
{
	if (FItemSpawnRow* SelectedRow = GetRandomItem())
	{
		if (UClass* ActualClass = SelectedRow->ItemClass.Get())
		{
			return SpawnItem(ActualClass);
		}
	}
	return nullptr;
}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
	//박스의 절반크기 가져오기 
	FVector BoxExtent = Box->GetScaledBoxExtent();
	FVector BoxCenter = Box->GetComponentLocation();
	return BoxCenter + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
		);
}

AActor* ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	if (ItemClass == nullptr) return nullptr;
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		ItemClass,
		GetRandomPointInVolume(),
		FRotator::ZeroRotator
		);
	return SpawnedActor;
}

FItemSpawnRow* ASpawnVolume::GetRandomItem() const
{
	if (!ItemDataTable) return nullptr;

	TArray<FItemSpawnRow*> Rows;
	static const FString ContextString(TEXT("ItemSpawnContext"));
	ItemDataTable->GetAllRows(ContextString,Rows);

	if (Rows.IsEmpty()) return nullptr;
	float TotalChance = 0.0f;
	for (const FItemSpawnRow* Row : Rows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnRate;
		}
	}

	const float RandomChance = FMath::FRandRange(0.0f,TotalChance);
	float AccumulatedChance = 0.0f;
	for (FItemSpawnRow* Row : Rows)
	{
		AccumulatedChance += Row->SpawnRate;
		if (RandomChance <= AccumulatedChance)
		{
			return Row;
		}
	}
	return nullptr;
}


