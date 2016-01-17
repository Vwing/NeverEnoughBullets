// Fill out your copyright notice in the Description page of Project Settings.

#include "GameJam2016.h"
#include "Ship.h"
#include "PaperSpriteComponent.h"
#include "Bounds.h"
#include "Monster.h"
#include "Projectile.h"

// Sets default values
AShip::AShip()
{
	PrimaryActorTick.bCanEverTick = true;

	
	
	ShipSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ShipSprite"));
	ConstructorHelpers::FObjectFinder<UPaperSprite> ShipSpriteAsset(TEXT("PaperSprite'/Game/Sprites/ShipSprite.ShipSprite'"));
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

	AbsorbSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("AbsorbSprite"));
	ConstructorHelpers::FObjectFinder<UPaperSprite> AbsorbSpriteAsset(TEXT("PaperSprite'/Game/Sprites/AbsorbSprite.AbsorbSprite'"));
	AbsorbSprite->SetSprite(AbsorbSpriteAsset.Object);
	AbsorbSprite->SetVisibility(true);

	AbsorbSprite->bGenerateOverlapEvents = true;
	AbsorbSprite->SetNotifyRigidBodyCollision(true);
	AbsorbSprite->GetBodyInstance()->bLockZTranslation = true;
	AbsorbSprite->SetSimulatePhysics(false);
	AbsorbSprite->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	AbsorbSprite->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AbsorbSprite->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	AbsorbSprite->SetEnableGravity(false);
	AbsorbSprite->bMultiBodyOverlap = true;
	AbsorbSprite->GetBodyInstance()->bUseCCD = true;
	AbsorbSprite->SetAbsolute(true, true, true);

	ConstructorHelpers::FObjectFinder<UPaperSprite> PlayerProjectileAsset(TEXT("PaperSprite'/Game/Sprites/NormalShot.NormalShot'"));
	ProjectilesArray.Reserve(10);
	for (int i = 0; i < 10; i++)
	{
		UPaperSpriteComponent* PlayerProjectile = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PlayerProjectile"+i));
		PlayerProjectile->SetSprite(PlayerProjectileAsset.Object);

		PlayerProjectile->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		PlayerProjectile->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		PlayerProjectile->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		PlayerProjectile->SetVisibility(false);
		PlayerProjectile->SetEnableGravity(false);
		PlayerProjectile->SetSimulatePhysics(false);
		PlayerProjectile->SetAbsolute(true, true, true);

		ProjectilesArray.Push(PlayerProjectile);
	}


	TopInnerBounds = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TopInnerBounds"));
	ConstructorHelpers::FObjectFinder<UPaperSprite> InnerBoundsAsset(TEXT("PaperSprite'/Game/Sprites/InnerBounds.InnerBounds'"));
	TopInnerBounds->SetSprite(InnerBoundsAsset.Object);
	TopInnerBounds->AttachTo(RootComponent);
	TopInnerBounds->SetAbsolute(true, true, true);

	BottomInnerBounds = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BottomInnerBounds"));
	BottomInnerBounds->SetSprite(InnerBoundsAsset.Object);
	BottomInnerBounds->AttachTo(RootComponent);
	BottomInnerBounds->SetAbsolute(true, true, true);


	ShootingSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ShootingSound"));
	ConstructorHelpers::FObjectFinder<USoundBase> ShootingSoundAsset(TEXT("SoundWave'/Game/SFX/Burn.Burn'"));
	ShootingSound->SetSound(ShootingSoundAsset.Object);

	AbsorbSound = CreateDefaultSubobject<UAudioComponent>(TEXT("AbsorbSound"));
	ConstructorHelpers::FObjectFinder<USoundBase> AbsorbSoundAsset(TEXT("SoundWave'/Game/SFX/Absorb.Absorb'"));
	AbsorbSound->SetSound(AbsorbSoundAsset.Object);
	

	ShipState = EShipStates::Static;
	MaxVerticalSpeed = 900.0f;
	MaxHorizontalSpeed = 900.0f;
	CurrentVerticalSpeed = 0.0f;
	CurrentHorizontalSpeed = 0.0f;

	MaxShotsUsed = 5;
	ShotsInUse = 0;
	Ammo = 1;
	bCanMoveLeft = true;
	bCanShoot = true;
	bCanAbsorb = true;
	bCanMoveRight = true;
	bIsDead = false;

	ProjectileSpeed = 700.0f;
	ProjectileRotation = FRotator(90.0f, 90.0f, 0.0f);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	DebugString = "";
	MonsterReference = nullptr;
	
}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	
	ShootingSound->Stop();
	AbsorbSound->Stop();
	for (int i = 0; i < ProjectilesArray.Num(); i++)
	{
		ProjectilesArray[i]->RelativeRotation = ProjectileRotation;
		ProjectilesArray[i]->SetWorldLocation(FVector(0.0f, 0.0f, -100.0f));
	}

	AbsorbSprite->RelativeRotation = ProjectileRotation;
	AbsorbSprite->SetWorldLocation(FVector(-100.0f, 200.0f, -200.0f));
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
	InputComponent->BindAction("Absorb", IE_Pressed, this, &AShip::Absorb);

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

	if (ShotsInUse <= MaxShotsUsed && bCanShoot && Ammo > 0)
	{
		ShootProjectile();
		Ammo -= 1;
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
	else if (bCanMoveRight == false) //code for blocking Right movements
	{
		if (CurrentHorizontalSpeed < 0.0f)
		{
			bCanMoveRight = true;
			CurrentHorizontalSpeed *= DeltaTime;
		}
		else
		{
			CurrentHorizontalSpeed = 0.0f;
		}
	}
	else
	{
		bCanMoveRight = true;
		bCanMoveLeft = true;
		CurrentHorizontalSpeed *= DeltaTime;
	}

	SetActorLocation(GetActorLocation() + FVector(CurrentHorizontalSpeed, CurrentVerticalSpeed, 0.0f));
}

void AShip::UpdateOverlappingComponents(TArray<UPrimitiveComponent*>& OverlappingComponents)
{
	for (int i = 0; i < OverlappingComponents.Num(); ++i)
	{
		DebugString = OverlappingComponents[i]->GetName();
		if (OverlappingComponents[i]->GetName().Contains("TopBoundsSprite"))
		{
			ABounds* bounds = Cast<ABounds>(OverlappingComponents[i]->GetAttachmentRootActor());

			if (bounds == nullptr || bounds == NULL)
			{
				ShipState = EShipStates::ErrorState;
				return;
			}

			FVector TeleportLocation = FVector(GetActorLocation().X, bounds->BottomBounds->GetComponentLocation().Y + 10,0.0f);
				//bounds->BottomInnerBounds->GetComponentLocation().Y,
			
				

			SetActorLocation(TeleportLocation);
		}
		else if (OverlappingComponents[i]->GetName().Contains("BottomBoundsSprite")) //if hit the bottom bound teleport up
		{
			ABounds* bounds = Cast<ABounds>(OverlappingComponents[i]->GetAttachmentRootActor());

			if (bounds == nullptr || bounds == NULL)
			{
				ShipState = EShipStates::ErrorState;
				return;
			}

			FVector TeleportLocation = FVector(GetActorLocation().X, bounds->TopBounds->GetComponentLocation().Y - 10, 0.0f);
				//bounds->TopInnerBounds->GetComponentLocation().Y,  
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
			ABounds* bounds = Cast<ABounds>(OverlappingComponents[i]->GetAttachmentRootActor());

			if (bounds == nullptr || bounds == NULL)
			{
				ShipState = EShipStates::ErrorState;
				return;
			}
			FVector TeleportLocation = FVector(GetActorLocation().X - CurrentHorizontalSpeed - 3.0f,
				GetActorLocation().Y, 0.0f); // code fore teleportation
			bCanMoveRight = false;

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
			ProjectilesArray[i]->GetOverlappingComponents(OverlappingComponents);
			if (UpdateOverlappingProjectiles(OverlappingComponents)) //check if its overlapping to destroy it
			{
				ProjectilesArray[i]->SetVisibility(false);
				ProjectilesArray[i]->SetWorldLocation(FVector(0.0f, 0.0f, -100.0f));
				continue;
			}

			FVector NewProjectileLocation = FVector(ProjectilesArray[i]->GetComponentLocation().X + ProjectileSpeed*DeltaTIme,
				ProjectilesArray[i]->GetComponentLocation().Y, 0.0f);

			ProjectilesArray[i]->SetRelativeLocation(NewProjectileLocation);
		}
	}
}

bool AShip::UpdateOverlappingProjectiles(TArray<UPrimitiveComponent*>& OverlappingComponents)
{
	for (int i = 0; i < OverlappingComponents.Num(); i++)
	{
		DebugString = OverlappingComponents[i]->GetName();
		if (OverlappingComponents[i]->GetName().Contains("MonsterSprite"))
		{
			AMonster* monster = Cast<AMonster>(OverlappingComponents[i]->GetAttachmentRootActor());
			monster->Health -= 1;
			monster->SetDamagedState();
			monster->ShipLocation = GetActorLocation();
			//MonsterReference->Health -= 1;
		//	MonsterReference->SetDamagedState();
		//	MonsterReference->ShipLocation = GetActorLocation();
			//UpdateMonster();
			//monster = nullptr;
			ShotsInUse -= 1;
			return true;
		}
		else if (OverlappingComponents[i]->GetName().Contains("RightBoundsSprite"))
		{
			ShotsInUse -= 1;
			return true;
		}
	}
	return false;
}

void AShip::Absorb()
{
	if (bCanAbsorb)
	{
		AbsorbSound->Play();
		bCanAbsorb = false;
		InitiateAbsorb();
	}
}

void AShip::StopAbsorb()
{
	bCanAbsorb = true;
	AbsorbSprite->SetVisibility(false);
	AbsorbSprite->SetWorldLocation(FVector(-200.0f, -200.0f, -200.0f));

}

void AShip::InitiateAbsorb()
{
	AbsorbSprite->SetVisibility(true);
	AbsorbSprite->SetWorldLocation(FVector(GetActorLocation().X + ShipSprite->Bounds.BoxExtent.X*1.2, 
										GetActorLocation().Y, 0.0f));

	GetWorldTimerManager().SetTimer(ShipHandle, this, &AShip::StopAbsorb, 0.4f, false);
}

void AShip::UpdateMonster()
{
	if (MonsterReference == nullptr ||
		MonsterReference == NULL)
	{
		ShipState = EShipStates::ErrorState;
		return;
	}
}
