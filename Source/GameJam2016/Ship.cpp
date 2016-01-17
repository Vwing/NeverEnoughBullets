// Fill out your copyright notice in the Description page of Project Settings.

#include "GameJam2016.h"
#include "Ship.h"
#include "PaperSpriteComponent.h"
#include "Bounds.h"
#include "Projectile.h"

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

	ConstructorHelpers::FObjectFinder<UPaperSprite> PlayerProjectileAsset(TEXT("PaperSprite'/Game/Sprites/NormalShot.NormalShot'"));
	ProjectilesArray.Reserve(10);
	for (int i = 0; i < 10; i++)
	{
		//FString Name = "Projectile" + i;
		UPaperSpriteComponent* PlayerProjectile = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PlayerProjectile"+i));
		PlayerProjectile->SetSprite(PlayerProjectileAsset.Object);

		PlayerProjectile->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		PlayerProjectile->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		PlayerProjectile->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		PlayerProjectile->SetVisibility(true);
		PlayerProjectile->SetEnableGravity(false);
		PlayerProjectile->SetSimulatePhysics(false);
		PlayerProjectile->SetAbsolute(true, true, true);

		ProjectilesArray.Push(PlayerProjectile);
	}

	ShootingSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ShootingSound"));
	ConstructorHelpers::FObjectFinder<USoundBase> ShootingSoundAsset(TEXT("SoundWave'/Game/SFX/Burn.Burn'"));
	ShootingSound->SetSound(ShootingSoundAsset.Object);

	ShipState = EShipStates::Static;
	MaxVerticalSpeed = 500.0f;
	MaxHorizontalSpeed = 500.0f;
	CurrentVerticalSpeed = 0.0f;
	CurrentHorizontalSpeed = 0.0f;

	MaxShots = 5;
	ShotsInUse = 0;
	bCanMoveLeft = true;
	bCanShoot = true;
	ProjectileSpeed = 600.0f;
	ProjectileRotation = FRotator(90.0f, 90.0f, 0.0f);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	DebugString = "";
}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	ShootingSound->Stop();
	for (int i = 0; i < ProjectilesArray.Num(); i++)
	{
		ProjectilesArray[i]->RelativeRotation = ProjectileRotation;
	}
	Super::BeginPlay();

}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MakeMovements(DeltaTime);
	ShipSprite->GetOverlappingComponents(OverlappingComponents);
	UpdateOverlappingComponents(OverlappingComponents);
	if (ShotsInUse > 0)
	{
		UpdateProjectiles(DeltaTime);
	}

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
	InputComponent->BindAction("Fire", IE_Released, this, &AShip::StopFire);
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

	if (ShotsInUse <= MaxShots && bCanShoot)
	{
		ShootProjectile();
		ShootingSound->Play();
		bCanShoot = false;
	}
}

void AShip::StopFire()
{
	//use this because the shot gets shot multiple times with one click of the space bar
	bCanShoot = true;
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
				-GetActorLocation().Y - bounds->BottomBounds->Bounds.BoxExtent.Z,  // we rotated the ship, its actually portrait naturally, we rotated it, so we use X
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
				-GetActorLocation().Y, //+ bounds->BottomBounds->Bounds.BoxExtent.X,  // we rotated the ship, its actually portrait naturally, we rotated it, so we use X
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

void AShip::ShootProjectile()
{
	for (int i = 0; i < ProjectilesArray.Num(); i++)
	{
		if (ProjectilesArray[i]->IsVisible())
		{
			continue;
		}
		else
		{
			FVector ProjectileLocation = FVector(GetActorLocation().X ,//+ ShipSprite->Bounds.BoxExtent.X
				GetActorLocation().Y, 0.0f);

			ProjectilesArray[i]->SetWorldLocation(ProjectileLocation);
			ProjectilesArray[i]->SetVisibility(true);
			ShotsInUse++;
			return;
		}
	}
}

void AShip::UpdateProjectiles(float DeltaTIme)
{
	for (int i = 0; i < ProjectilesArray.Num(); i++)
	{
		if (ProjectilesArray[i]->IsVisible())
		{
			FVector NewProjectileLocation = FVector(ProjectilesArray[i]->GetComponentLocation().X + ProjectileSpeed*DeltaTIme,
				ProjectilesArray[i]->GetComponentLocation().Y, 0.0f);

			ProjectilesArray[i]->SetRelativeLocation(NewProjectileLocation);
		}
	}
}