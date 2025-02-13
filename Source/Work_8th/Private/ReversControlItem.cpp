// Fill out your copyright notice in the Description page of Project Settings.


#include "ReversControlItem.h"

#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"
AReversControlItem::AReversControlItem()
{
	ItemType = "ReversControl";
	Duration = 5.0f;
}

void AReversControlItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	if (Activator && Activator->ActorHasTag("Player"))
	{
		ASpartaCharacter* Player = Cast<ASpartaCharacter>(Activator);
		if (Player)
		{
			Player->ReversControl(Duration);
			DestroyItem();
		}
		
	}
}


