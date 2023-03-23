// Fill out your copyright notice in the Description page of Project Settings.


#include "Nightfall_HUD.h"
#include "Components/RadialSlider.h"


//Calls to damage and update durability of bars, first parameter is durability damage, second parameter is the index of bars
void UNightfall_HUD::DamageDurability(int DurabilityIndex, float DurabilityDamage)
{
	if (DurabilityIndex == Primary)
	{
		PrimaryDurability -= DurabilityDamage;
		PrimaryDurabilityBar->SetValue(PrimaryDurability);
	}
	if (DurabilityIndex == Secondary)
	{
		SecondaryDurability -= DurabilityDamage;
		SecondaryDurabilityBar->SetValue(SecondaryDurability);
	}
	if (DurabilityIndex == Tertiary)
	{
		TertiaryDurability -= DurabilityDamage;
		TertiaryDurabilityBar->SetValue(TertiaryDurability);
	}
}