// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

class USphereComponent;

UCLASS()
class WORK_8TH_API ABaseItem : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	ABaseItem();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USceneComponent* ItemRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	UStaticMeshComponent* ItemMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* ItemCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effect")
	UParticleSystem* PickUpEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effect")
	USoundBase* PickUpSound;
	
protected:

	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
		) override;
	
	UFUNCTION()
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
		) override;
	
	virtual void ActivateItem(AActor* Activator) override;
	virtual void DestroyItem();
	virtual FName GetItemType() const override;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	FName ItemType;
};
