// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Collectibles.generated.h"

/**
 * 
 */
UCLASS()
class NIGHTFALL_ISLAND_API ACollectibles : public AItem
{
	GENERATED_BODY()
public:
	void Equip(USceneComponent* InParent);

protected:
	//It is a UFUNCtion macro here, we can just not see it. We can't have UFUNCTION, we will get an compilation error. 
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
