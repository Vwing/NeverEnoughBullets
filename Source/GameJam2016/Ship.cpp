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

	ProjectileRotation = FRotator(90.0f, 0.0f, 0.0f);
	AbsorbSpriteScale = FVector(1.2f, 1.0f, 1.2);
	InnerBoundsRotation = FRotator(90.0f, 0.0f, 90.0f);
	ShipSpriteScale = FVector(1.3f, 1.0f, .50f);
	ShipFlipbookScale = FVector(3.0f, 3.0f, 3.0f);
	ProjectileAnimOffset = FVector(-75.0f, 0.0f, 1.0f);
	ProjectileAnimScale = FVector(8.0f, 8.0f, 8.0f);
	InnerBoundsScale = FVector(5.0f, 1.0f, 10.0f);
	ArrowAnimScale = FVector(.4f, .4f, .4f);
	


	ShipHitBox = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ShipHitBox"));
	ConstructorHelpers::FObjectFinder<UPaperSprite> ShipHitBoxAsset(TEXT("PaperSprite'/Game/Sprites/InnerBounds.InnerBounds'"));
	ShipHitBox->SetSprite(ShipHitBoxAsset.Object);
	RootComponent = ShipHitBox;

	ShipHitBox->bGenerateOverlapEvents = true;
	ShipHitBox->SetNotifyRigidBodyCollision(true);
	ShipHitBox->GetBodyInstance()->bLockZTranslation = true;
	SetActorEnableCollision(true);
	ShipHitBox->SetSimulatePhysics(false);
	ShipHitBox->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	ShipHitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ShipHitBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	ShipHitBox->SetEnableGravity(false);
	ShipHitBox->bMultiBodyOverlap = true;
	ShipHitBox->GetBodyInstance()->bUseCCD = true;
	ShipHitBox->SetWorldScale3D(ShipSpriteScale);


	ShipFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("ShipFlipbook"));
	ShipFlipbook->AttachTo(RootComponent);
	ShipFlipbook->SetAbsolute(false,true,true);
	ShipFlipbook->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);	

	
	ShipIdleAnim = CreateDefaultSubobject<UPaperFlipbook>(TEXT("ShipIdleAnim"));
	ShipMoveForwardAnim = CreateDefaultSubobject<UPaperFlipbook>(TEXT("ShipMoveForwardAnim"));
	ShipMoveUpAnim = CreateDefaultSubobject<UPaperFlipbook>(TEXT("ShipMoveUpAnim"));
	ShipMoveDownAnim = CreateDefaultSubobject<UPaperFlipbook>(TEXT("ShipMoveDownAnim"));
	ShipBlueAnim = CreateDefaultSubobject<UPaperFlipbook>(TEXT("ShipBlueAnim"));

	ConstructorHelpers::FObjectFinder<UPaperFlipbook> ShipIdleAnimAsset(TEXT("PaperFlipbook'/Game/Sprites/ShipSprites/ShipAnimIdle.ShipAnimIdle'"));
	ShipIdleAnim = ShipIdleAnimAsset.Object;
	ConstructorHelpers::FObjectFinder<UPaperFlipbook> ShipMoveForwardAnimAsset(TEXT("PaperFlipbook'/Game/Sprites/ShipSprites/ShipAnimMoveForward.ShipAnimMoveForward'"));
	ShipMoveForwardAnim = ShipMoveForwardAnimAsset.Object;
	ConstructorHelpers::FObjectFinder<UPaperFlipbook> ShipMoveUpAnimAsset(TEXT("PaperFlipbook'/Game/Sprites/ShipSprites/ShipAnimMoveUp.ShipAnimMoveUp'"));
	ShipMoveUpAnim = ShipMoveUpAnimAsset.Object;
	ConstructorHelpers::FObjectFinder<UPaperFlipbook> ShipMoveDownAnimAsset(TEXT("PaperFlipbook'/Game/Sprites/ShipSprites/ShipAnimMoveDown.ShipAnimMoveDown'"));
	ShipMoveDownAnim = ShipMoveDownAnimAsset.Object;
	ConstructorHelpers::FObjectFinder<UPaperFlipbook> ShipBlueAnimAsset(TEXT("PaperFlipbook'/Game/Sprites/ShipSprites/ShipAnimBlue.ShipAnimBlue'"));
	ShipBlueAnim = ShipBlueAnimAsset.Object;

		
	ShipFlipbook->SetFlipbook(ShipIdleAnim);


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
	
	//arrow sprite
	ConstructorHelpers::FObjectFinder<UPaperFlipbook> ArrowAnimAsset(TEXT("PaperFlipbook'/Game/Sprites/AnimArrow.AnimArrow'"));

	TopArrowAnim = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("TopArrowAnim"));
	TopArrowAnim->SetFlipbook(ArrowAnimAsset.Object);
	TopArrowAnim->SetVisibility(false);
	TopArrowAnim->AttachTo(TopInnerBounds);
	TopArrowAnim->SetAbsolute(true, true, true);

	BottomArrowAnim = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("BottomArrowAnim"));
	BottomArrowAnim->SetFlipbook(ArrowAnimAsset.Object);
	BottomArrowAnim->SetVisibility(false);
	BottomArrowAnim->AttachTo(BottomInnerBounds);
	BottomArrowAnim->SetAbsolute(true, true, true);
	

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

	SliceSound = CreateDefaultSubobject<UAudioComponent>(TEXT("SliceSound"));
	ConstructorHelpers::FObjectFinder<USoundBase> SliceSoundAsset(TEXT("SoundWave'/Game/SFX/Slice.Slice'"));
	SliceSound->SetSound(SliceSoundAsset.Object);

	BlasterSound = CreateDefaultSubobject<UAudioComponent>(TEXT("BlasterSound"));
	ConstructorHelpers::FObjectFinder<USoundBase> BlasterSoundAsset(TEXT("SoundWave'/Game/SFX/Blaster.Blaster'"));
	BlasterSound->SetSound(BlasterSoundAsset.Object);

	ShipState = EShipStates::Idle;
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
	bGameHasStarted = false;
	bAreArrowsOn = false;
	ProjectileSpeed = 1200.0f;
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	DebugString = "";
	MonsterReference = nullptr;
	AbsorbAgainTimer= 1.25f;
	AbsorbDuration = .3f;
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

	ShipFlipbook->SetWorldScale3D(ShipFlipbookScale);
	ShipHitBox->SetWorldScale3D(ShipSpriteScale);
	ShipHitBox->SetWorldRotation(InnerBoundsRotation);
	UpdateMonster();

	TopInnerBounds->SetWorldRotation(InnerBoundsRotation);
	TopInnerBounds->SetWorldScale3D(InnerBoundsScale);
	BottomInnerBounds->SetWorldRotation(InnerBoundsRotation);
	BottomInnerBounds->SetWorldScale3D(InnerBoundsScale);

	BottomArrowAnim->SetWorldRotation(ProjectileRotation);
	BottomArrowAnim->SetWorldScale3D(ArrowAnimScale);

	TopArrowAnim->SetWorldRotation(ProjectileRotation);
	TopArrowAnim->AddRelativeRotation(FRotator(0.0f, 0.0f, 180.0f));
	TopArrowAnim->SetWorldScale3D(ArrowAnimScale);

	AbsorbSprite->RelativeRotation = ProjectileRotation;
	AbsorbSprite->SetWorldScale3D(AbsorbSpriteScale);
	AbsorbSprite->SetWorldLocation(FVector(-100.0f, -200.0f, -200.0f));
	AbsorbAnimXOffset = ShipHitBox->Bounds.BoxExtent.X * 1.75;

	Super::BeginPlay();
}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MakeMovements(DeltaTime);
	ShipHitBox->GetOverlappingComponents(OverlappingComponents);
	UpdateOverlappingComponents(OverlappingComponents);

	if (ShotsInUse > 0)
	{
		UpdateProjectiles(DeltaTime);
	}
	if (!bIsDead)
	{
	//	BlasterSound->Play();
	}
	if (bAreArrowsOn)
	{
		UpdateArrows();
	}

	switch (ShipState)
	{
	case EShipStates::Idle:
		SetAnim(ShipIdleAnim);
		break;

	case EShipStates::MoveUp:
		SetAnim(ShipMoveUpAnim);
		break;

	case EShipStates::MoveDown:
		SetAnim(ShipMoveDownAnim);
		break;

	case EShipStates::MoveForward:
		SetAnim(ShipMoveForwardAnim);
		break;

	case EShipStates::Absorb:
		SetAnim(ShipBlueAnim);
		if (!bCanAbsorb) // if currently active(!bCanAbsorb)
		{
			UpdateAbsorb();
		}
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

	if (AxisValue != 0)
	{
		if (ShipState == EShipStates::Idle ||
			ShipState == EShipStates::MoveForward ||
			ShipState == EShipStates::MoveDown ||
			ShipState == EShipStates::MoveUp)
		{
			if (AxisValue > 0)
			{
				ShipState = EShipStates::MoveUp;
			}
			else if (AxisValue < 0)
			{
				ShipState = EShipStates::MoveDown;
			}
		}
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

	if (AxisValue != 0)
	{
		if (ShipState == EShipStates::Idle)
		{
			ShipState = EShipStates::MoveForward;
		}
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
	
	if (CurrentVerticalSpeed == 0 && CurrentHorizontalSpeed == 0 && ShipState != EShipStates::Absorb)
	{
		ShipState = EShipStates::Idle;
	}
	else
	{
		SetActorLocation(GetActorLocation() + (FVector(CurrentHorizontalSpeed, CurrentVerticalSpeed, 0.0f)));
	}
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
			bAreArrowsOn = true;
			GetWorldTimerManager().SetTimer(ArrowHandle, this, &AShip::TurnArrowsOff, .3f, false);
		}
		else if (OverlappingComponents[i]->GetName().Contains("BottomInnerBounds"))
		{
			bAreArrowsOn = true;
			GetWorldTimerManager().SetTimer(ArrowHandle, this, &AShip::TurnArrowsOff, .3f, false);
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
			FVector ProjectileLocation = FVector(GetActorLocation().X,//+ ShipHitBoxHolder->Bounds.BoxExtent.X
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
			bGameHasStarted = true;
			
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
		ShipState = EShipStates::Absorb;
		AbsorbSound->Play();
		bCanAbsorb = false;
		InitiateAbsorb();
	}
}

void AShip::StopAbsorb()
{
	AbsorbSprite->SetVisibility(true);
	AbsorbSprite->SetWorldLocation(FVector(-200.0f, -200.0f, -200.0f));
	ShipState = EShipStates::Idle;

	GetWorldTimerManager().SetTimer(ShipHandle, this, &AShip::AllowAbsorb, AbsorbAgainTimer, false); //timer to allow absorb again
}

void AShip::InitiateAbsorb()
{
	AbsorbSprite->SetVisibility(true);
	UpdateAbsorb();

	GetWorldTimerManager().SetTimer(ShipHandle, this, &AShip::StopAbsorb, AbsorbDuration, false);
}

void AShip::AllowAbsorb()
{
	bCanAbsorb = true;
	bCanSlice = true;
}

void AShip::UpdateAbsorb()
{
	AbsorbSprite->SetWorldLocation(FVector(GetActorLocation().X + AbsorbAnimXOffset,
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
					bGameHasStarted = true;
				}
			}
		}
	}
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
	GetWorldTimerManager().SetTimer(UpdateMonsterHandle, this, &AShip::UpdateMonster, 0.750f, false);
}

void AShip::SetAnim(UPaperFlipbook* AnimAsset)
{
	ShipFlipbook->SetFlipbook(AnimAsset);
}

void AShip::UpdateArrows()
{
	DebugString = "UpdateArrows";
	TopArrowAnim->SetVisibility(true);
	TopArrowAnim->SetWorldLocation(FVector(GetActorLocation().X, TopInnerBounds->GetComponentLocation().Y + TopArrowAnim->Bounds.BoxExtent.Y*2, 0.0f));

	BottomArrowAnim->SetVisibility(true);
	BottomArrowAnim->SetWorldLocation(FVector(GetActorLocation().X, BottomInnerBounds->GetComponentLocation().Y - BottomArrowAnim->Bounds.BoxExtent.X*2, 0.0f));
}

void AShip::TurnArrowsOff()
{
	bAreArrowsOn = false;
	TopArrowAnim->SetVisibility(false);
	BottomArrowAnim->SetVisibility(false);
}