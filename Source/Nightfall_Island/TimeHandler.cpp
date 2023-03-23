// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeHandler.h"
#include "Components/DirectionalLightComponent.h"

ATimeHandler::ATimeHandler()
{
    PrimaryActorTick.bCanEverTick = true;

    SunLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("SunLight"));
    RootComponent = SunLight;
    NightLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("NightLight"));

    SunLight->ForwardShadingPriority = 0;
    NightLight->ForwardShadingPriority = 1;
    NightLight->SetLightColor(FLinearColor(0.55, 0, 1, 1));
    NightLight->SetIntensity(CurrentNightIntensity);

    SunLight->AddLocalRotation(FRotator(270, 0, 0));
    NightLight->AddLocalRotation(FRotator(-90, 0, 0));
}

void ATimeHandler::BeginPlay()
{
    Super::BeginPlay();

}

void ATimeHandler::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Get the current sun rotation
    FRotator CurrentRotation = SunLight->GetRelativeRotation();
    /*
    UE_LOG(LogTemp, Warning, TEXT("pitch is: %s"), *FString::SanitizeFloat(CurrentRotation.Pitch));
    */

    // Check if the sun is above the horizon
    if (CurrentRotation.Pitch <= 0 && CurrentRotation.Pitch >= -90)
    {
        // If the sun is above the horizon, rotate it normally
        float RotationAmount = RotationSpeed * DeltaTime;
        FRotator NewRotation = FRotator(RotationAmount, 0, 0);
        SunLight->AddLocalRotation(NewRotation);
        // If it is day, decrease night intensity until gone
        if (CurrentNightIntensity >= 0) {
            NightLight->SetIntensity(CurrentNightIntensity -= 5 * DeltaTime);
        }
    }
    else
    {
        // If the sun is below the horizon, rotate it twice as fast
        float RotationAmount = RotationSpeed * DeltaTime * 2;
        FRotator NewRotation = FRotator(RotationAmount, 0, 0);
        SunLight->AddLocalRotation(NewRotation);
        // If it is night, increase night intensity until chosen value
        if (CurrentNightIntensity <= ChosenNightIntensity) {
            NightLight->SetIntensity(CurrentNightIntensity += 5 * DeltaTime);
        }
    }
}

