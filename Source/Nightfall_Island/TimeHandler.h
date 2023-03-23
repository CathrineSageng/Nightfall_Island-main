// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeHandler.generated.h"

class UDirectionalLightComponent;
enum DayStates
{
    Night = 0,
    Day = 1
};
UCLASS()
class NIGHTFALL_ISLAND_API ATimeHandler : public AActor
{
	GENERATED_BODY()

public:    
    ATimeHandler();

protected:
    virtual void BeginPlay() override;

public:    
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, Category = "Sun")
    float RotationSpeed = 20.f;

    UPROPERTY(VisibleAnywhere, Category = "Sun")
    UDirectionalLightComponent* SunLight = nullptr;

    UPROPERTY(VisibleAnywhere, Category = "Night")
    UDirectionalLightComponent* NightLight = nullptr;

    UPROPERTY(VisibleAnywhere, Category = "Night")
    float ChosenNightIntensity = 3;
    float CurrentNightIntensity = 0;
};
