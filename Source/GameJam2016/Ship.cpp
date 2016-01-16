// Fill out your copyright notice in the Description page of Project Settings.

#include "GameJam2016.h"
#include "Ship.h"
#include "PaperSpriteComponent.h"
#include "Bounds.h"

// Sets default values
AShip::AShip()
{
	PrimaryActorTick.bCanEverTick = true;

	ShipSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ShipSprite"));
	ConstructorHelpers::FObjectFinder<UPaperSprite> ShipSpriteAsset(TEXT("PaperSprite'/Game/Sprites/SpaceshipSprite.SpaceshipSprite'"));
	ShipSprite->SetSprite(ShipSpriteAsset.Object);
	RootComponent = ShipSprite;

	ShipSprite->bGenerateOverlapEvents = true;
	ShipSprite->SetNotifyRigidBodyCollision(true);
	ShipSprite->GetBodyInstance()->bLockZTranslation = true;
	SetActorEnableCollision(true);
	ShipSprite->SetSimulatePhysics(false);
	ShipSprite->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	ShipSprite->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ShipSprite->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	ShipSprite->SetEnableGravity(false);
	ShipSprite->bMultiBodyOverlap = true;
	ShipSprite->GetBodyInstance()->bUseCCD = true;

	ShootingSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ShootingSound"));
	ConstructorHelpers::FObjectFinder<USoundBase> ShootingSoundAsset(TEXT("SoundWave'/Game/SFX/Burn.Burn'"));
	ShootingSound->SetSound(ShootingSoundAsset.Object);

	ShipState = EShipStates::Static;
	MaxVerticalSpeed = 500.0f;
	MaxHorizontalSpeed = 500.0f;
	CurrentVerticalSpeed = 0.0f;
	CurrentHorizontalSpeed = 0.0f;
	bCanMoveLeft = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	DebugString = "";
}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	ShootingSound->Stop();
	Super::BeginPlay();

}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MakeMovements(DeltaTime);
	ShipSprite->GetOverlappingComponents(OverlappingComponents);
	UpdateOverlappingComponents(OverlappingComponents);

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
	ShootingSound->Play();
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

void AShip::MakeMovements(float DeltaTime)
{
	CurrentVerticalSpeed *= DeltaTime;

	if (bCanMoveLeft == false) //code for blocking left movements
	{
		if (CurrentHorizontalSpeed > 0.0f)
		{
			bCanMoveLeft = true;
			CurrentHorizontalSpeed *= DeltaTime;
		}
		else
		{
			CurrentHorizontalSpeed = 0.0f;
		}
	}
	else
	{
		bCanMoveLeft = true;
		CurrentHorizontalSpeed *= DeltaTime;
	}

	SetActorLocation(GetActorLocation() + FVector(CurrentHorizontalSpeed, CurrentVerticalSpeed, 0.0f));
}

void AShip::UpdateOverlappingComponents(TArray<UPrimitiveComponent*>& OverlappingComponents)
{
	for (int i = 0; i < OverlappingComponents.Num(); ++i)
	{
		if (OverlappingComponents[i]->GetName().Contains("TopBoundsSprite"))
		{
			ABounds* bounds = Cast<ABounds>(OverlappingComponents[i]->GetAttachmentRootActor());

			if (bounds == nullptr || bounds == NULL)
			{
				ShipState = EShipStates::ErrorState;
				return;
			}
			
			FVector TeleportLocation = FVector(GetActorLocation().X, 
				-GetActorLocation().Y - ShipSprite->Bounds.BoxExtent.X,  // we rotated the ship, its actually portrait naturally, we rotated it, so we use X
				0.0f); // code fore teleportation
				

			SetActorLocation(TeleportLocation);
		}
		else if (OverlappingComponents[i]->GetName().Contains("BottomBoundsSprite")) //if hit the bottom bound teleport up
		{
			ABounds* bounds = Cast<ABounds>(OverlappingComponents[i]->GetAttachmentRootActor());

		//	if (bounds == nullptr || bounds == NULL)
			{
		//		ShipState = EShipStates::ErrorState;
		//		return;
			}

			FVector TeleportLocation = FVector(GetActorLocation().X,
				-GetActorLocation().Y + ShipSprite->Bounds.BoxExtent.X,  // we rotated the ship, its actually portrait naturally, we rotated it, so we use X
				0.0f); // code fore teleportation
			SetActorLocation(TeleportLocation);
		}
		else if (OverlappingComponents[i]->GetName().Contains("LeftBoundsSprite"))
		{
			ABounds* bounds = Cast<ABounds>(OverlappingComponents[i]->GetAttachmentRootActor());

			if (bounds == nullptr || bounds == NULL)
			{
				ShipState = EShipStates::ErrorState;
				return;
			}

			FVector TeleportLocation = FVector(GetActorLocation().X + CurrentHorizontalSpeed + 3.0f,
				GetActorLocation().Y, 0.0f); // code fore teleportation
			bCanMoveLeft = false;

			SetActorLocation(TeleportLocation);
		}
		else if (OverlappingComponents[i]->GetName().Contains("RightBoundsSprite"))
		{
			//ABounds* bounds = Cast<ABounds>(OverlappingComponents[i]->GetAttachmentRootActor());

		//	if (bounds == nullptr || bounds == NULL)
		//	{
			//	ShipState = EShipStates::ErrorState;
		//		return;
		//	}

			FVector TeleportLocation = FVector(OverlappingComponents[i]->GetComponentLocation().X + 1.0f,
				GetActorLocation().Y, 0.0f); // code fore teleportation
			SetActorLocation(TeleportLocation);
		}
	}
}