// Fill out your copyright notice in the Description page of Project Settings.


#include "WoodNode.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AWoodNode::AWoodNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TreeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TreeMesh"));
	TreeMesh->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		TreeMeshFinder(TEXT("'/Game/Assets/Meshes/SM_LeafTree.SM_LeafTree'"));
	TreeMesh->SetStaticMesh(TreeMeshFinder.Object);
	TreeMesh->SetRelativeLocation(FVector(0, 0, 425));

	MaxResourceAmount = 3;
	CurrentResourceAmount = MaxResourceAmount;
}

// Called when the game starts or when spawned
void AWoodNode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWoodNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

