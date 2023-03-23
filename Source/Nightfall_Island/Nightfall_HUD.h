// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Nightfall_HUD.generated.h"

/**
 * 
 */
UCLASS()
class NIGHTFALL_ISLAND_API UNightfall_HUD : public UUserWidget
{
	GENERATED_BODY()
	enum DurabilityIndexes
	{
		Primary = 1,
		Secondary = 2,
		Tertiary = 3
	};

public:
	void DamageDurability(int DurabilityIndex, float DurabilityDamage);

// Health HUD
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UProgressBar* HealthBar;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UImage* HeartImage;

// Time HUD
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* DayText;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* TimeText;

// Durability HUD
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class URadialSlider* PrimaryDurabilityBar;
	float PrimaryDurability = 1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class URadialSlider* SecondaryDurabilityBar;
	float SecondaryDurability = 1;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class URadialSlider* TertiaryDurabilityBar;
	float TertiaryDurability = 1;
};
