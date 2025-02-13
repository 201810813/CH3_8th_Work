// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingItem.h"
#include "SpartaCharacter.h"
AHealingItem::AHealingItem()
{
	HealingValue = 20;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASpartaCharacter* Character = Cast<ASpartaCharacter>(Activator))
		{
			Character->Heal(HealingValue);	
		}
		DestroyItem();
	}
}
