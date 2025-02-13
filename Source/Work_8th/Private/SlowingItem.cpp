// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowingItem.h"
#include "SpartaCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ASlowingItem::ASlowingItem()
{
	SlowRate = 0.5f;
	SlowDuration = 5.0f;
	ItemType = "SlowingItem";
}


void ASlowingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	if (Activator && Activator -> ActorHasTag("Player"))
	{
		if (ASpartaCharacter* Player = Cast<ASpartaCharacter>(Activator))
		{
			Player->SlowMovement(SlowRate,SlowDuration);
			DestroyItem();
		}
	}

}

