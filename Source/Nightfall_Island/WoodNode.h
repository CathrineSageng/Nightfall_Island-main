// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WoodNode.generated.h"

UCLASS()
class NIGHTFALL_ISLAND_API AWoodNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWoodNode();

	UPROPERTY(EditAnywhere, Category = "Resources")
	float MaxResourceAmount;
	float CurrentResourceAmount;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private: 
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* TreeMesh;
};
