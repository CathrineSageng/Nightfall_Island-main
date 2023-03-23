// Fill out your copyright notice in the Description page of Project Settings.


#include "Nightfall_Character.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "Nightfall_HUD.h"
#include "Blueprint/UserWidget.h"
#include "WoodNode.h"
#include "Nightfall_HUD.h"
#include "Math/UnrealMathUtility.h"
#include "Collectibles.h"

// Sets default values
ANightfall_Character::ANightfall_Character()
{
 	// Set this character to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("'/Game/FirstPersonArms/Character/Mesh/SK_Mannequin_Arms.SK_Mannequin_Arms'"));
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetSkeletalMesh(SkeletalMesh.Object);
	Mesh1P->SetRelativeRotation(FRotator(0, -90, 0));
	Mesh1P->SetRelativeLocation(FVector(-30, 0, -150));

	// Mapping Context & HUD
	NightfallHUDClass = nullptr;
	NightfallHUD = nullptr;

	NI_IMC = CreateDefaultSubobject<UInputMappingContext>(TEXT("NI_IMC"));
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> MappingContext(TEXT("'/Game/FirstPerson/Inputs/NI_IMC.NI_IMC'"));
	static ConstructorHelpers::FClassFinder<UNightfall_HUD> HUDClass(TEXT("/Game/FirstPerson/Interface/BP_Nightfall_HUD"));
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveAction(TEXT("'/Game/FirstPerson/Inputs/Actions/IA_Move.IA_Move'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> JumpAction(TEXT("'/Game/FirstPerson/Inputs/Actions/IA_Jump.IA_Jump'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> EquipAction(TEXT("'/Game/FirstPerson/Inputs/Actions/IA_Equip.IA_Equip'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> LookAction(TEXT("'/Game/FirstPerson/Inputs/Actions/IA_Look.IA_Look'"));
	static ConstructorHelpers::FObjectFinder<UInputAction> ShootAction(TEXT("'/Game/FirstPerson/Inputs/Actions/IA_Shoot.IA_Shoot'"));
	NI_IMC = MappingContext.Object;
	IA_Move = MoveAction.Object;
	IA_Jump = JumpAction.Object;
	IA_Equip = EquipAction.Object;
	IA_Look = LookAction.Object;
	IA_Shoot = ShootAction.Object;
	NightfallHUDClass = HUDClass.Class;

	// Shooting cooldowns
	MaxShootCooldown = 0.5;
	CurrentShootCooldown = 0;

	Equipped = 1;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ANightfall_Character::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (subsystem)
		{
			subsystem->AddMappingContext(NI_IMC, 0);
		}
	}
	//Add HUD
	if (IsLocallyControlled() && NightfallHUDClass)
	{
		APlayerController* controller = GetWorld()->GetFirstPlayerController();
		check(controller);
		NightfallHUD = CreateWidget<UNightfall_HUD>(controller, NightfallHUDClass);
		check(NightfallHUD);
		NightfallHUD->AddToPlayerScreen();
	}
}

// Called every frame
void ANightfall_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentShootCooldown > 0.f)
	{
		CurrentShootCooldown -= DeltaTime;
	}
}

void ANightfall_Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ANightfall_Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}

}

void ANightfall_Character::Shoot()
{
	if (CurrentShootCooldown <= 0.f)
	{
		// code adapted from Fullstack Development's video on raycasting
		FHitResult OutHit;

		FVector Start = FirstPersonCameraComponent->GetComponentLocation();
		FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();

		Start += ForwardVector;
		FVector End = Start + (ForwardVector * 1000);

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this->GetOwner());

		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1, 0, 1);

		bool IsHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

		if (IsHit && FMath::IsNearlyZero(NightfallHUD->PrimaryDurability) || NightfallHUD->PrimaryDurability > 0.001f)
		{
			AWoodNode* WoodNode = Cast<AWoodNode>(OutHit.GetActor());
			if (WoodNode)
			{
				WoodNode->CurrentResourceAmount--;
				NightfallHUD->DamageDurability(Equipped, 0.2f);

				if (WoodNode->CurrentResourceAmount <= 0)
				{
					WoodNode->Destroy();
				}
			}
		}
		CurrentShootCooldown = MaxShootCooldown;
		UE_LOG(LogTemp, Warning, TEXT("Durability is: %s"), *FString::SanitizeFloat(NightfallHUD->PrimaryDurability));
	}
}

void ANightfall_Character::Equip()
{
	/*ACollectibles* OverlappingCollectibles = Cast<ACollectibles>(OverlappingItem);
	if (OverlappingCollectibles)
	{
		OverlappingCollectibles->Equip(GetMesh());
		SetActorHiddenInGame(true);
	}*/
	SetActorHiddenInGame(true);
}

// Called to bind functionality to input
void ANightfall_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
	//Jumping
	UE_LOG(LogTemp, Warning, TEXT("Binding Jump actions!"));
	EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	
	//Equip
	UE_LOG(LogTemp, Warning, TEXT("Binding Equip actions!"));
	EnhancedInputComponent->BindAction(IA_Equip, ETriggerEvent::Triggered, this, &ANightfall_Character::Equip);
	EnhancedInputComponent->BindAction(IA_Equip, ETriggerEvent::Completed, this, &ANightfall_Character::Equip);

	//Moving
	UE_LOG(LogTemp, Warning, TEXT("Binding Move action!"));
	EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ANightfall_Character::Move);
	
	//Looking
	UE_LOG(LogTemp, Warning, TEXT("Binding Look action!"));
	EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ANightfall_Character::Look);
	
	//Shooting
	UE_LOG(LogTemp, Warning, TEXT("Binding Shoot action!"));
	EnhancedInputComponent->BindAction(IA_Shoot, ETriggerEvent::Triggered, this, &ANightfall_Character::Shoot);
	}
}

