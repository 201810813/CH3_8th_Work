// Fill out your copyright notice in the Description page of Project Settings.

#include "MineItem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"


AMineItem::AMineItem()
{
	MineRadius = 300.0f;
	ItemType = "Mine";
	MineDamage = 30;
	MineDelay = 3.0f;
	bHadExploded = false;
	MineCollision = CreateDefaultSubobject<USphereComponent>(TEXT("MineCollision"));
	MineCollision->SetSphereRadius(MineRadius);
	MineCollision->SetCollisionProfileName("OverlapAllDynamic");
	MineCollision->SetupAttachment(ItemRoot);
}

void AMineItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	if (!bHadExploded)
	{
		GetWorld()->GetTimerManager().SetTimer(MineExplodeTimer,
		this,
		&AMineItem::Explode,
		MineDelay,
		false);
	}
	
}

void AMineItem::Explode()
{
	if (ExplodeEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ExplodeEffect,
			GetActorLocation(),
			GetActorRotation(),
			true);
	}
	if (ExplodeSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ExplodeSound,
			GetActorLocation(),
			GetActorRotation(),
			true);
	}
	
	TArray<AActor*> Actors;
	MineCollision->GetOverlappingActors(Actors);
	for (AActor* Actor : Actors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(
				Actor,
				MineDamage,
				nullptr,
				this,
				UDamageType::StaticClass());
			DestroyItem();
		}
	}
}
