// Fill out your copyright notice in the Description page of Project Settings.

#include "GameJam2016.h"
#include "Ship.h"
#include "PaperSpriteComponent.h"
#include "Bounds.h"
#include "Monster.h"
#include "Projectile.h"
#include "PaperFlipbookComponent.h"
// Sets default values
AShip::AShip()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileRotation = FRotator(90.0f, 90.0f, 0.0f);
	AbsorbSpriteScale = FVector(1.2f, 1.0f, 1.2);
	InnerBoundsRotation = FRotator(90.0f, 0.0f, 90.0f);
	ShipSpriteScale = FVector(1.3f, 1.0f, .50f);
	ShipFlipbookScale = FVector(3.0f, 3.0f, 3.0f);
	ProjectileAnimOffset = FVector(-75.0f, 0.0f, 1.0f);
	ProjectileAnimScale = FVector(8.0f, 8.0f, 8.0f);
	InnerBoundsScale = FVector(5.0f, 1.0f, 10.0f);
	ArrowSpriteScale = FVector(.4f, .4f, .4f);


	ShipSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ShipSprite"));
	ConstructorHelpers::FObjectFinder<UPaperSprite> ShipSpriteAsset(TEXT("PaperSprite'/Game/Sprites/InnerBounds.InnerBounds'"));
	ShipSprite->SetSprite(ShipSpriteAsset.Object);
	RootComponent = ShipSprite;

	IdleAnim = CreateDefaultSubobject<UPaperFlipbook>(TEXT("IdleAnim"));
	ConstructorHelpers::FObjectFinder<UPaperFlipbook> IdleAnimationAsset(TEXT("PaperFlipbook'/Game/Sprites/ShipSprites/ShipRocketForward.ShipRocketForward'"));
	IdleAnim = IdleAnimationAsset.Object;

	ShipFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("ShipFlipbook"));
	ShipFlipbook->SetFlipbook(IdleAnim);
	ShipFlipbook->AttachTo(RootComponent);
	ShipFlipbook->SetAbsolute(false,true,true);
	ShipFlipbook->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);

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
	ShipSprite->SetWorldScale3D(ShipSpriteScale);

	AbsorbSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("AbsorbSprite"));
	ConstructorHelpers::FObjectFinder<UPaperSprite> AbsorbSpriteAsset(TEXT("PaperSprite'/Game/Sprites/AbsorbSprite.AbsorbSprite'"));
	AbsorbSprite->SetSprite(AbsorbSpriteAsset.Object);
	AbsorbSprite->SetVisibility(true);

	/*
	UPaperFlipbookComponent* AbsorbFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("AbsorbFlipbook"));
	ConstructorHelpers::FObjectFinder<UPaperFlipbook> AbsorbFlipbookAsset(TEXT("PaperSprite'/Game/Sprites/AbsorbSprite.AbsorbSprite'"));
	AbsorbFlipbook->SetFlipbook(AbsorbFlipbookAsset.Object);
	AbsorbFlipbook->SetVisibility(true);
	*/


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
	AbsorbSprite->SetWorldLocation(FVector(-200.0f, -200.0f, -200.0f));

	ConstructorHelpers::FObjectFinder<UPaperSprite> PlayerProjectileSpriteAsset(TEXT("PaperSprite'/Game/Sprites/NormalShot.NormalShot'"));
	ConstructorHelpers::FObjectFinder<UPaperFlipbook> PlayerProjectileAnimAsset(TEXT("PaperFlipbook'/Game/Sprites/BulletSprites/PlayerBullet.PlayerBullet'"));
	ProjectilesArray.Reserve(10);
	for (int i = 0; i < 10; i++)
	{
		UPaperSpriteComponent* PlayerProjectile = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PlayerProjectileSprite" + i));
		PlayerProjectile->SetSprite(PlayerProjectileSpriteAsset.Object);

		PlayerProjectile->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		PlayerProjectile->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		PlayerProjectile->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		PlayerProjectile->SetVisibility(false);
		PlayerProjectile->SetEnableGravity(false);
		PlayerProjectile->SetSimulatePhysics(false);
		PlayerProjectile->SetAbsolute(true, true, true);

		UPaperFlipbookComponent* PlayerProjectileAnim = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("PlayerProjectileAnim" + i));
		PlayerProjectileAnim->SetFlipbook(PlayerProjectileAnimAsset.Object);
		PlayerProjectileAnim->AttachTo(PlayerProjectile);
		PlayerProjectileAnim->SetAbsolute(true, true, true);
		PlayerProjectileAnim->SetWorldScale3D(FVector(5.0f, 5.0f, 5.0f));
		PlayerProjectileAnim->SetVisibility(false);

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

	ConstructorHelpers::FObjectFinder<UPaperSprite> ArrowSpriteAsset(TEXT("PaperSprite'/Game/Sprites/Arrow_Sprite_0.Arrow_Sprite_0'"));
	TopArrowSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TopArrowSprite"));
	TopArrowSprite->AttachTo(RootComponent);
	TopArrowSprite->SetAbsolute(true, true, true);
	TopArrowSprite->SetWorldScale3D(ArrowSpriteScale);
	TopArrowSprite->SetWorldLocation(FVector(-200, -200, -400));

	BottomArrowSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BottomArrowSprite"));
	BottomArrowSprite->AttachTo(RootComponent);
	BottomArrowSprite->SetAbsolute(true, true, true);
	BottomArrowSprite->SetWorldScale3D(ArrowSpriteScale);
	BottomArrowSprite->SetWorldLocation(FVector(-200, -200, -400));

	ShootingSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ShootingSound"));
	ConstructorHelpers::FObjectFinder<USoundBase> ShootingSoundAsset(TEXT("SoundWave'/Game/SFX/Burn.Burn'"));
	ShootingSound->SetSound(ShootingSoundAsset.Object);

	AbsorbSound = CreateDefaultSubobject<UAudioComponent>(TEXT("AbsorbSound"));
	ConstructorHelpers::FObjectFinder<USoundBase> AbsorbSoundAsset(TEXT("SoundWave'/Game/SFX/Absorb.Absorb'"));
	AbsorbSound->SetSound(AbsorbSoundAsset.Object);

	SliceSound = CreateDefaultSubobject<UAudioComponent>(TEXT("SliceSound"));
	ConstructorHelpers::FObjectFinder<USoundBase> SliceSoundAsset(TEXT("SoundWave'/Game/SFX/Slice.Slice'"));
	SliceSound->SetSound(SliceSoundAsset.Object);

	BlasterSound = CreateDefaultSubobject<UAudioComponent>(TEXT("BlasterSound"));
	ConstructorHelpers::FObjectFinder<USoundBase> BlasterSoundAsset(TEXT("SoundWave'/Game/SFX/Blaster.Blaster'"));
	BlasterSound->SetSound(BlasterSoundAsset.Object);

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
	bIsExploding = false;
	bCanSlice = true;

	ProjectileSpeed = 1200.0f;
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	DebugString = "";
	MonsterReference = nullptr;
	AbsorbTimer = .3f;
}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	ShootingSound->Stop();
	AbsorbSound->Stop();
	SliceSound->Stop();
	BlasterSound->Stop();

	for (int i = 0; i < ProjectilesArray.Num(); i++)
	{
		ProjectilesArray[i]->RelativeRotation = ProjectileRotation;
		ProjectilesArray[i]->SetWorldLocation(FVector(-200.0f, -300.0f, -100.0f));

		ProjectilesArray[i]->GetChildComponent(0)->SetWorldScale3D(ProjectileAnimScale);
		ProjectilesArray[i]->GetChildComponent(0)->RelativeRotation = ProjectileRotation;
		ProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(FVector(-200.0f, -300.0f, -100.0f));
	}
	

	AbsorbSprite->RelativeRotation = ProjectileRotation;
	AbsorbSprite->SetWorldScale3D(AbsorbSpriteScale);
	AbsorbSprite->SetWorldLocation(FVector(-100.0f, -200.0f, -200.0f));

	ShipFlipbook->SetWorldScale3D(ShipFlipbookScale);
	ShipSprite->SetWorldScale3D(ShipSpriteScale);
	UpdateMonster();

	TopInnerBounds->SetWorldRotation(InnerBoundsRotation);
	TopInnerBounds->SetWorldScale3D(InnerBoundsScale);
	BottomInnerBounds->SetWorldRotation(InnerBoundsRotation);
	BottomInnerBounds->SetWorldScale3D(InnerBoundsScale);

	TopArrowSprite->SetWorldRotation(ProjectileRotation);
	BottomArrowSprite->SetWorldRotation(ProjectileRotation);

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
	if (!bCanAbsorb) // if currently active(!bCanAbsorb)
	{
		UpdateAbsorb(); 
	}
	if (!bIsDead)
	{
	//	BlasterSound->Play();
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

	SetActorLocation(GetActorLocation() + (FVector(CurrentHorizontalSpeed, CurrentVerticalSpeed, 0.0f)));
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

			FVector TeleportLocation = FVector(GetActorLocation().X,
				BottomInnerBounds->GetComponentLocation().Y - 10,
				0.0f);
			DebugLocation = BottomInnerBounds->GetComponentLocation();

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

			FVector TeleportLocation = FVector(GetActorLocation().X,
				TopInnerBounds->GetComponentLocation().Y,
				0.0f);

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
		else if (OverlappingComponents[i]->GetName().Contains("TopInnerBounds"))
		{
			//TopArrowSprite->SetWorldLocation(FVector(GetActorLocation().X, TopInnerBounds->GetComponentLocation().Y, 0.0f));
		}
	}
}

void AShip::ShootProjectile()
{
	for (int i = 0; i < ProjectilesArray.Num(); i++)
	{
		if (ProjectilesArray[i]->GetChildComponent(0)->IsVisible())
		{
			continue;
		}
		else
		{
			FVector ProjectileLocation = FVector(GetActorLocation().X,//+ ShipSprite->Bounds.BoxExtent.X
				GetActorLocation().Y, 0.0f);

			ProjectilesArray[i]->SetWorldLocation(ProjectileLocation);
			ProjectilesArray[i]->SetVisibility(false);
			ProjectilesArray[i]->GetChildComponent(0)->SetVisibility(true);
			ProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(ProjectileLocation + ProjectileAnimOffset);
			ShotsInUse++;
			return;
		}
	}
}

void AShip::UpdateProjectiles(float DeltaTIme)
{
	for (int i = 0; i < ProjectilesArray.Num(); i++)
	{
		if (ProjectilesArray[i]->GetChildComponent(0)->IsVisible())
		{
			ProjectilesArray[i]->GetOverlappingComponents(OverlappingComponents);
			if (UpdateOverlappingProjectiles(OverlappingComponents)) //check if its overlapping to destroy it
			{
				ProjectilesArray[i]->SetVisibility(false);
				ProjectilesArray[i]->GetChildComponent(0)->SetVisibility(false);
				ProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(FVector(-100.0f, -10.0f, -100.0f));
				ProjectilesArray[i]->SetWorldLocation(FVector(-100.0f, -10.0f, -100.0f));
				continue;
			}

			FVector NewProjectileLocation = FVector(ProjectilesArray[i]->GetComponentLocation().X + ProjectileSpeed*DeltaTIme,
				ProjectilesArray[i]->GetComponentLocation().Y, 0.0f);

			ProjectilesArray[i]->SetRelativeLocation(NewProjectileLocation);
			ProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(NewProjectileLocation + ProjectileAnimOffset);
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
			if (MonsterReference == nullptr ||
				MonsterReference == NULL)
			{
				ShipState = EShipStates::ErrorState;
				return false;
			}

			MonsterReference->ShipLocation = GetActorLocation();
			MonsterReference->SetDamagedState();
			
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
	AbsorbSprite->SetVisibility(false);
	AbsorbSprite->SetWorldLocation(FVector(-200.0f, -200.0f, -200.0f));

	GetWorldTimerManager().SetTimer(ShipHandle, this, &AShip::AllowAbsorb, AbsorbTimer, false); //timer to allow absorb again
}

void AShip::InitiateAbsorb()
{
	AbsorbSprite->SetVisibility(true);
	UpdateAbsorb();

	GetWorldTimerManager().SetTimer(ShipHandle, this, &AShip::StopAbsorb, AbsorbTimer, false);
}

void AShip::AllowAbsorb()
{
	bCanAbsorb = true;
	bCanSlice = true;
	AbsorbSprite->SetVisibility(false);
	AbsorbSprite->SetWorldLocation(FVector(-200.0f, -200.0f, -200.0f));
}

void AShip::UpdateMonster()
{
	if (MonsterReference == nullptr ||
		MonsterReference == NULL)
	{
		ShipState = EShipStates::ErrorState;
		return;
	}

	MonsterReference->ShipLocation = GetActorLocation();
	GetWorldTimerManager().SetTimer(UpdateMonsterHandle, this, &AShip::UpdateMonster, .50f, false);
}

void AShip::UpdateAbsorb()
{
	AbsorbSprite->SetWorldLocation(FVector(GetActorLocation().X + ShipSprite->Bounds.BoxExtent.X*2.0f,
		GetActorLocation().Y, 0.0f));

	if (bCanSlice)
	{
		AbsorbSprite->GetOverlappingComponents(OverlappingComponents);
		if (OverlappingComponents.Num() > 0)
		{
			for (int i = 0; i < OverlappingComponents.Num(); i++)
			{
				if (OverlappingComponents[i]->GetName().Contains("MonsterSprite"))
				{
					if (MonsterReference == nullptr ||
						MonsterReference == NULL)
					{
						return;
					}
					MonsterReference->Health -= 3;
					MonsterReference->SetDamagedState();
					SliceSound->Play();
					bCanSlice = false;
				}
			}
		}
	}
	
}
