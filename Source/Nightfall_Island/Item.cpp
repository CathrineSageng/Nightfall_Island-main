// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/SphereComponent.h"
#include "Nightfall_Character.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
	
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ANightfall_Character* Nightfall_Character  = Cast<ANightfall_Character>(OtherActor);
	if (Nightfall_Character)
	{
		Nightfall_Character->SetOverlappingItem(this);
	}
	const FString OtherActorName = OtherActor->GetName();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Blue, OtherActorName);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ANightfall_Character * Nightfall_Character = Cast<ANightfall_Character>(OtherActor);
	if (Nightfall_Character)
	{
		Nightfall_Character->SetOverlappingItem(nullptr);
	}

	const FString OtherActorName = OtherActor->GetName();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, OtherActorName);
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
