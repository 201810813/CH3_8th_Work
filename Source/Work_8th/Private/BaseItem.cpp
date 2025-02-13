// Fill out your copyright notice in the Description page of Project Settings.
#include "BaseItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ItemRoot"));
	SetRootComponent(ItemRoot);

	ItemCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ItemCollision"));
	ItemCollision->SetupAttachment(ItemRoot);
	ItemCollision->SetSphereRadius(50.0f);
	ItemCollision->SetCollisionProfileName("OverlapAllDynamic");
	ItemCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);
	ItemCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(ItemCollision);
	
	ItemType = FName("Item");
	
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseItem::OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		ActivateItem(OtherActor);
	}
}

void ABaseItem::OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex)
{
	
}

void ABaseItem::ActivateItem(AActor* Activator)
{
	if (PickUpEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			PickUpEffect,
			GetActorLocation(),
			GetActorRotation(),
			true);
	}
	if (PickUpSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			PickUpSound,
			GetActorLocation(),
			GetActorRotation(),
			true);
	}
}

void ABaseItem::DestroyItem()
{
	Destroy();
}

FName ABaseItem::GetItemType() const
{
	return ItemType;
}

