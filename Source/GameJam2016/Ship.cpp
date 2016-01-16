// Fill out your copyright notice in the Description page of Project Settings.

#include "GameJam2016.h"
#include "Ship.h"
#include "PaperSpriteComponent.h"


// Sets default values
AShip::AShip()
{
	PrimaryActorTick.bCanEverTick = true;

	ShipSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ShipSprite"));
	ConstructorHelpers::FObjectFinder<UPaperSprite> ShipSpriteAsset(TEXT("PaperSprite'/Game/Sprites/SpaceshipSprite.SpaceshipSprite'"));
	ShipSprite->SetSprite(ShipSpriteAsset.Object);
	RootComponent = ShipSprite;

	ShipState = EShipStates::Static;
	MaxVerticalSpeed = 500.0f;
	MaxHorizontalSpeed = 500.0f;
	CurrentVerticalSpeed = 0.0f;
	CurrentHorizontalSpeed = 0.0f;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShip::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
	CurrentVerticalSpeed *= DeltaTime;
	CurrentHorizontalSpeed *= DeltaTime;
	SetActorLocation(GetActorLocation() + FVector(CurrentHorizontalSpeed, CurrentVerticalSpeed, 0.0f));
		
	switch (ShipState)
	{
	case EShipStates::Static:
		break;

	case EShipStates::MovingVertical:
		break;

	case EShipStates::MovingHorizontal:
		break;

	case EShipStates::MovingHorizontalShooting:
		break;

	case EShipStates::MovingVerticalShooting:
		break;

	case EShipStates::Shooting:
		break;

	case EShipStates::Damaged:
		break;

	case EShipStates::ErrorState:
		break;

	case EShipStates::Closed:
		break;
	}
}

// Called to bind functionality to input
void AShip::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AShip::Fire);
	InputComponent->BindAxis("MoveUp", this, &AShip::MoveUp);
	InputComponent->BindAxis("MoveRight", this, &AShip::MoveRight);
}

void AShip::Fire()
{
	if (ShipState == EShipStates::ErrorState ||
		ShipState == EShipStates::Closed)
	{
		return;
	}
}

void AShip::MoveUp(float AxisValue)
{
	if (ShipState == EShipStates::ErrorState ||
		ShipState == EShipStates::Closed)
	{
		return;
	}
	CurrentVerticalSpeed = AxisValue*MaxVerticalSpeed;
}

void AShip::MoveRight(float AxisValue)
{
	if (ShipState == EShipStates::ErrorState ||
		ShipState == EShipStates::Closed)
	{
		return;
	}
	CurrentHorizontalSpeed = AxisValue*MaxHorizontalSpeed;	
}

