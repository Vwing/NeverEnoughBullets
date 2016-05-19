// Fill out your copyright notice in the Description page of Project Settings.

#include "GameJam2016.h"
#include "Ship.h"
#include "PaperSpriteComponent.h"
#include "Bounds.h"
#include "Monster.h"
#include "Projectile.h"
#include "Referencer.h"
#include "Background.h"
#include "PaperFlipbookComponent.h"
// Sets default values
AShip::AShip()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileRotation = FRotator(90.0f, 0.0f, 0.0f);
	AbsorbSpriteScale = FVector(1.0f, 1.0f, 1.0f);
	InnerBoundsRotation = FRotator(90.0f, 0.0f, 90.0f);
	ShipSpriteScale = FVector(1.0f, 1.0f, .40f);
	ShipFlipbookScale = FVector(3.0f, 3.0f, 3.0f);
	ProjectileAnimOffset = FVector(-75.0f, 0.0f, 1.0f);
	ProjectileAnimScale = FVector(4.0f, 4.0f, 4.0f);
	InnerBoundsScale = FVector(5.0f, 1.0f, 10.0f);
	ArrowAnimScale = FVector(.4f, .4f, .4f);
	ShipFlipbookOffset = FVector(0.0f, 0.0f, -10.0f);
	MonsterTimer = 0;
	MonsterDuration = .5f;
	MaxAmmo = 5;

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
	ShipFlipbook->RelativeLocation = ShipFlipbookOffset;

	
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

	ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> ExplosionAnimAsset(TEXT("PaperFlipbook'/Game/Sprites/Explosion/ExplodeAnim.ExplodeAnim'"));
	ExplosionAnim = ExplosionAnimAsset.Get();

	ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> InvincibleAnimAsset(TEXT("PaperFlipbook'/Game/Sprites/ShipSprites/Invincible.Invincible'"));
	InvincibleAnim = InvincibleAnimAsset.Get();
		
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
	ProjectilesArray.Reserve(MaxAmmo);
	for (int i = 0; i < MaxAmmo; i++)
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
		PlayerProjectileAnim->SetWorldScale3D(ProjectileAnimScale);
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
	bCanMoveRight = true;
	bIsDead = false;
	bIsExploding = false;
	bCanSlice = true;
	bGameHasStarted = false;
	bAreArrowsOn = false;
	bIsSlowMo = false;
	bCanSlowMo = true;
	SlowMoMultiplier = 1.0f;
	ProjectileSpeed = 1200.0f;
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	DebugString = "";
	AbsorbAgainTimer= 1.25f;
	AbsorbDuration = .3f;
	SlowMoDuration = .50f;
	Deaths = 0;
	MaxDeathTime = 2;
	DeathTime = 0;
	MaxInvincibleTime = 2;
	bIsAbsorbing = false;
	AbsorbTimer = 0;
	AbsorbDuration = .75;
	AbsorbAgainTimer = 1.5f;
	bCanScrollLeft = false;
	bCanScrollRight = false;
	bIsInvincible = false;
	bIsScrollingRight = false;
	bIsScrollingLeft = false;
	bIsScrolling = false;
}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	ShootingSound->Stop();
	AbsorbSound->Stop();
	SliceSound->Stop();
	BlasterSound->Stop();

	for (int i = 0; i < MaxAmmo; i++)
	{
		ProjectilesArray[i]->RelativeRotation = ProjectileRotation;
		ProjectilesArray[i]->SetWorldLocation(FVector(-200.0f, -300.0f, -100.0f));

		ProjectilesArray[i]->GetChildComponent(0)->SetWorldScale3D(ProjectileAnimScale);
		ProjectilesArray[i]->GetChildComponent(0)->RelativeRotation = ProjectileRotation;
		ProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(FVector(-200.0f, -300.0f, -100.0f));
	}

	ShipFlipbook->SetWorldScale3D(ShipFlipbookScale);
	ShipFlipbook->AddWorldOffset(ShipFlipbookOffset);
	ShipHitBox->SetWorldScale3D(ShipSpriteScale);
	ShipHitBox->SetWorldRotation(InnerBoundsRotation);
	
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
	AbsorbSprite->SetWorldLocation(FVector(-1500.0f, -1500.0f, -200.0f));
	AbsorbAnimXOffset = ShipHitBox->Bounds.BoxExtent.X * 1.75;
	
	Super::BeginPlay();
}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasNull())
	{
		return;
	}
	else if (bIsScrollingLeft && bIsDead) // (ShipState != EShipStates::Absorb && ShipState != EShipStates::Idle &&
				//	ShipState != EShipStates::Sliced)) // if died
	{
		ScrollLeft(DeltaTime);	
		UpdateScrollingRules();
		bIsInvincible = true;
		//got rid of the return statement because needs to play exploding animation
	}
	else if(bIsScrollingLeft) //if sliced
	{
		ScrollLeft(DeltaTime);
		UpdateScrollingRules();
		bIsInvincible = true;
	}
	else 
	{ //regular protocol (no special cases)
		MakeMovements(DeltaTime);
		ShipHitBox->GetOverlappingComponents(OverlappingComponents);
		UpdateOverlappingComponents(OverlappingComponents);
		//UpdateMonster(DeltaTime);

		if (bIsInvincible) //make sure alive before updating invincibility
		{
			UpdateInvincibility(DeltaTime);
		}
		if (bIsAbsorbing)
		{
			UpdateAbsorb(DeltaTime);
		}
		if (ShotsInUse > 0)
		{
			UpdateProjectiles(DeltaTime);
		}
		if (bAreArrowsOn)
		{
			UpdateArrows();
		}
		
	}		

	switch (ShipState)
	{
	case EShipStates::Idle:
		DebugString2 = "Idle";
		SetAnim(ShipIdleAnim);
		break;

	case EShipStates::MoveUp:
		DebugString2 = "MoveUp";
		SetAnim(ShipMoveUpAnim);
		break;

	case EShipStates::MoveDown:
		DebugString2 = "MoveDown";
		SetAnim(ShipMoveDownAnim);
		break;

	case EShipStates::MoveForward:
		DebugString2 = "MoveForward";
		SetAnim(ShipMoveForwardAnim);
		break;

	case EShipStates::Absorb:
		DebugString2 = "Absorb";
		SetAnim(ShipBlueAnim);
		break;

	case EShipStates::Shooting:
		DebugString2 = "Shooting";
		break;

	case EShipStates::Damaged:
		DebugString2 = "Damaged";
		break;

	case EShipStates::Exploding:
		DebugString2 = "Explode";
		SetAnim(ExplosionAnim);
		CheckFinishedExploding(DeltaTime);
		break;

	case EShipStates::Dead:
		DebugString2 = "Dead";
		CheckForRevive(DeltaTime);
		break;

	case EShipStates::Invincible:
		DebugString2 = "Invincibility";
		SetAnim(InvincibleAnim);
		break;

	case EShipStates::Sliced:
		DebugString = "Sliced";
	
		if (!bIsScrollingLeft) //if finished scrolling left, go back to hittable state
		{
			ShipState = EShipStates::Absorb;
			DeathTime = 0;
			float HorizontalMove = Ref->Background->LeftScrollBounds->GetComponentLocation().X;// -(Ref->Background->LeftScrollBounds->GetComponentLocation().X - Ref->Background->RightScrollBounds->GetComponentLocation().X);
			SetActorLocation(FVector(HorizontalMove, 0.0f, 0.0f));
		}
		
		break;

	case EShipStates::ErrorState:
		DebugString2 = "ErrorState";
		break;

	case EShipStates::Closed:
		DebugString2 = "Closed";
		break;

	default:
		DebugString2 = "Default";
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
	InputComponent->BindAction("SlowMo", IE_Pressed, this, &AShip::SlowMo);
	InputComponent->BindAction("SlowMo", IE_Released, this, &AShip::StopSlowMo);

	InputComponent->BindAxis("MoveUp", this, &AShip::MoveUp);
	InputComponent->BindAxis("MoveRight", this, &AShip::MoveRight);

}

void AShip::SlowMo()
{
	if (ShipState == EShipStates::ErrorState ||
		ShipState == EShipStates::Closed ||
		ShipState == EShipStates::Dead ||
		ShipState == EShipStates::Exploding ||
		bIsDead == true)
	{
		return;
	}

	if (bCanSlowMo)
	{
		SlowMoMultiplier = 0.5f;
		Ref->Monster->SlowMoMultiplier = SlowMoMultiplier;
	}
}

void AShip::StopSlowMo()
{
	//if (ShipState == EShipStates::ErrorState ||
	//	ShipState == EShipStates::Closed || bIsDead == true)
	{
	//	return;
	}
	SlowMoMultiplier = 1.0f;
	Ref->Monster->SlowMoMultiplier = SlowMoMultiplier;
}


void AShip::Fire()
{
	if (ShipState == EShipStates::ErrorState ||
		ShipState == EShipStates::Closed || 
		ShipState == EShipStates::Dead ||
		ShipState == EShipStates::Exploding ||
		bIsDead == true)
	{
		return;
	}

	if (ShotsInUse <= MaxShotsUsed && bCanShoot && Ammo > 0)
	{
		ShootProjectile();
		Ammo -= 1;
		ShootingSound->Play();
		ShotsInUse++;
		bCanShoot = false;
	}
}

void AShip::AddAmmo()
{
	if (Ammo < MaxAmmo)
	{
		++Ammo;
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
		ShipState == EShipStates::Closed ||
		ShipState == EShipStates::Dead ||
		ShipState == EShipStates::Exploding ||
		bIsDead == true)
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
		ShipState == EShipStates::Closed ||
		ShipState == EShipStates::Dead ||
		ShipState == EShipStates::Exploding ||
		bIsDead == true)
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
	if (ShipState == EShipStates::ErrorState ||
		ShipState == EShipStates::Closed ||
		ShipState == EShipStates::Dead ||
		ShipState == EShipStates::Exploding ||
		bIsDead == true)
	{
		return;
	}

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
	
	//code for scrolling
	UpdateScrollingRules();
	if (CurrentVerticalSpeed == 0 && CurrentHorizontalSpeed == 0 && ShipState != EShipStates::Absorb && ShipState != EShipStates::Invincible) //dont move if this happens point blank period
	{
		ShipState = EShipStates::Idle;
	}
	else if( (!bIsInLeftZone && !bIsInRightZone ) || 
				(bIsInLeftZone && !bCanScrollLeft) ||
				(bIsInRightZone && !bCanScrollRight) ||
				(bIsInLeftZone && CurrentHorizontalSpeed > 0) ||
				(bIsInRightZone && CurrentHorizontalSpeed < 0))
	{
		bIsScrolling = false;
		Move();
	}
	else
	{
		bIsScrolling = true;
		Scroll();
	}
}

void AShip::CameraMove(FVector MoveVect)
{
	Ref->Camera->AddActorWorldOffset(MoveVect);
}

void AShip::Move()
{
	AddActorWorldOffset(FVector(CurrentHorizontalSpeed, CurrentVerticalSpeed, 0.0f)*SlowMoMultiplier);
}

void AShip::Scroll()
{
	FVector MoveVect = FVector(CurrentHorizontalSpeed*SlowMoMultiplier, 0.0f, 0.0f);

	AddActorWorldOffset(FVector(CurrentHorizontalSpeed*SlowMoMultiplier, CurrentVerticalSpeed*SlowMoMultiplier, 0.0f)); //move the vertical of the ship still 
	CameraMove(MoveVect);
	TopInnerBounds->AddWorldOffset(MoveVect);
	BottomInnerBounds->AddWorldOffset(MoveVect);

	Ref->Background->Scrollz(MoveVect);
	Ref->Bounds->AddActorWorldOffset(MoveVect);
}

void AShip::UpdateScrollingRules()
{
	Ref->Background->UpdateScrolls();
	bIsInLeftZone = Ref->Background->bIsInLeftZone;
	bIsInRightZone = Ref->Background->bIsInRightZone;
	bCanScrollLeft = Ref->Background->bCanScrollLeft;
	bCanScrollRight = Ref->Background->bCanScrollRight;
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
	for (int i = 0; i < MaxAmmo; i++)
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
			return;
		}
	}
}

void AShip::UpdateProjectiles(float DeltaTIme)
{
	for (int i = 0; i < MaxAmmo; i++)
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
				--ShotsInUse;
				continue;
			}

			FVector NewProjectileLocation = FVector(ProjectilesArray[i]->GetComponentLocation().X + ProjectileSpeed*DeltaTIme*SlowMoMultiplier,
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
			Ref->Monster->TookDamage();

			return true;
		}
		else if (OverlappingComponents[i]->GetName().Contains("RightBoundsSprite"))
		{
			return true;
		}
	}
	return false;
}

void AShip::Absorb()
{
	if (ShipState == EShipStates::ErrorState ||
		ShipState == EShipStates::Closed || 
		ShipState == EShipStates::Dead ||
		ShipState == EShipStates::Exploding ||
		bIsDead == true )
	{
		return;
	}

	if (!bIsAbsorbing)
	{
		bIsAbsorbing = true;
	}
}


void AShip::UpdateMonster(float DeltaTime)
{
	if (MonsterTimer >= MonsterDuration)
	{
		Ref->Monster->ShipLocation = GetActorLocation();
		Ref->Monster->bIsShipAlive = !bIsDead;
		MonsterTimer = 0;
	}
	else
	{
		MonsterTimer += DeltaTime;
	}
	
}

void AShip::SetAnim(UPaperFlipbook* AnimAsset)
{
	ShipFlipbook->SetFlipbook(AnimAsset);
}

void AShip::UpdateArrows()
{
	DebugString = "UpdateArrows";
	TopArrowAnim->SetVisibility(true);
	TopArrowAnim->SetWorldLocation(FVector(GetActorLocation().X, TopInnerBounds->GetComponentLocation().Y + TopArrowAnim->Bounds.BoxExtent.Y * 2, 0.0f));

	BottomArrowAnim->SetVisibility(true);
	BottomArrowAnim->SetWorldLocation(FVector(GetActorLocation().X, BottomInnerBounds->GetComponentLocation().Y - BottomArrowAnim->Bounds.BoxExtent.X * 2, 0.0f));
}

void AShip::TurnArrowsOff()
{
	bAreArrowsOn = false;
	TopArrowAnim->SetVisibility(false);
	BottomArrowAnim->SetVisibility(false);
}

void AShip::TurnSlowMoOn()
{
	SlowMoMultiplier = 0.5f;

	Ref->Monster->SlowMoMultiplier = SlowMoMultiplier;
	GetWorldTimerManager().SetTimer(SlowMoHandle, this, &AShip::TurnSlowMoOff, SlowMoDuration, false);
}

void AShip::TurnSlowMoOff()
{
	Ref->Monster->SlowMoMultiplier = 1.0f;
	SlowMoMultiplier = 1.0f;

}

void AShip::Die()
{
	Deaths += 1;
	bIsDead = true;
	ShipState = EShipStates::Exploding;
	bIsScrollingLeft = true;
	bIsInvincible = true;

	Ref->Monster->ExplosionSound->Play();
	Ref->Monster->bIsShipAlive = false;
	
}

void AShip::ShotDead()
{
	Deaths += 1;
	bIsDead = true;
	ShipState = EShipStates::Exploding;
	bIsInvincible = true;
	bIsScrollingLeft = true;

	Ref->Monster->ExplosionSound->Play();
	Ref->Monster->bIsShipAlive = false;
	Ref->Monster->CurrentTimer = 0;	
}

void AShip::CheckFinishedExploding(float DeltaTime)
{
	DeathTime += DeltaTime;
	if (DeathTime >= 1)
	{
		ShipState = EShipStates::Dead;
		DeathTime = 0;
	}
}

void AShip::CheckForRevive(float DeltaTime)
{
	DeathTime += DeltaTime;
	if (DeathTime >= MaxDeathTime)
	{
		ReviveShip();
		DeathTime = 0;
	}
	else
	{
		SetActorLocation(FVector(-500.0f, 0.0f, -500.0f));
	}
}

void AShip::ReviveShip()
{
	bIsDead = false;
	bCanShoot = true;
	Ref->Monster->bIsShipAlive = true;
	bIsInvincible = true;
	ShipState = EShipStates::Invincible;
	
	Ammo = 0;
	DeathTime = 0;

	float HorizontalMove = Ref->Background->LeftScrollBounds->GetComponentLocation().X;
	SetActorLocation(FVector(HorizontalMove, 0.0f, 0.0f));
}

void AShip::UpdateInvincibility(float DeltaTime)
{
	if (DeathTime >= MaxInvincibleTime)
	{
		if (ShipState != EShipStates::Dead && ShipState != EShipStates::Exploding)
		{
			ShipState = EShipStates::Idle;
		}
		
		DeathTime = 0;
		bIsInvincible = false;
		ShipFlipbook->SetVisibility(true);
	}
	else
	{
		DeathTime += DeltaTime;
		ShipFlipbook->SetVisibility(!(ShipFlipbook->bVisible));
	}
}

void AShip::UpdateAbsorb(float DeltaTime)
{
	if (ShipState == EShipStates::Dead || ShipState == EShipStates::Exploding )
	{
		AbsorbSprite->SetVisibility(true);
		AbsorbSprite->SetWorldLocation(FVector(-200.0f, -200.0f, -200.0f));
		AbsorbTimer = 0;
		bIsAbsorbing = false;
		bCanSlice = true;
		return;
	}
	else if (ShipState == EShipStates::Absorb)
	{
		//do nothing but stop the next check from occurring
	}
	else if (AbsorbTimer < AbsorbDuration && 
		(ShipState == EShipStates::Idle || ShipState == EShipStates::MoveDown || ShipState == EShipStates::MoveForward || // change the naimation if any of these
		ShipState == EShipStates::MoveUp || ShipState == EShipStates::Shooting) )
	{
		ShipState = EShipStates::Absorb;
	}

	if (AbsorbTimer == 0) // if it just started play the sound
	{
		AbsorbSound->Play();
		AbsorbSprite->SetWorldLocation(FVector(GetActorLocation().X + AbsorbAnimXOffset,
			GetActorLocation().Y, 0.0f));
		UpdateSlice();
	}
	else if (AbsorbTimer >= AbsorbAgainTimer) // if can absorb again
	{
		bIsAbsorbing = false;
		bCanSlice = true;
		AbsorbTimer = 0;
		return;
	}
	else if (AbsorbTimer >= AbsorbDuration) // if the absorb has finished its time turn it off
	{
		AbsorbSprite->SetWorldLocation(FVector(-200.0f, -200.0f, -200.0f));
		ShipState = EShipStates::Idle;
	}
	else // if still on then update the location of the sprite and slice if possible 
	{
		AbsorbSprite->SetWorldLocation(FVector(GetActorLocation().X + AbsorbAnimXOffset,
			GetActorLocation().Y, 0.0f));
		UpdateSlice();
	}
		
	AbsorbTimer += DeltaTime; // update the duration
	
}

void AShip::UpdateSlice()
{
	if (bCanSlice)
	{
		AbsorbSprite->GetOverlappingComponents(OverlappingComponents);
		if (OverlappingComponents.Num() > 0)
		{
			for (int i = 0; i < OverlappingComponents.Num(); i++)
			{
				if (OverlappingComponents[i]->GetName().Contains("MonsterSprite"))
				{
					Ref->Monster->Health -= 5;
					TurnSlowMoOn();

					Ref->Monster->TookDamage();
					SliceSound->Play();
					bCanSlice = false;
					
					bIsScrollingLeft = true;
					ShipState = EShipStates::Sliced;
					bIsInvincible = true;

					AbsorbSprite->SetWorldLocation(FVector(-500.0f, -500.0f, -500.0f));
				}
			}
		}
	}
}

bool AShip::HasNull()
{
	if (Ref == nullptr ||
		Ref == NULL)
	{
		ShipState = EShipStates::ErrorState;
		NullString = "Ref is Null";
		return true;
	}
	else if (Ref->HasNullActors())
	{
		ShipState = EShipStates::ErrorState;
		NullString = "Ref has Null Actors";
		return true;
	}
	else
	{
		return false;
	}
}

void AShip::SetClosedState()
{
	ShipState = EShipStates::Closed;
	bIsInvincible = true;
}

void AShip::SetIdleState()
{
	ShipState = EShipStates::Idle;
	bIsInvincible = false;
}

void AShip::KeepScroll(float HorizontalMovement)
{
	AddActorWorldOffset(FVector(HorizontalMovement, 0.0f, 0.0f));
	AbsorbSprite->SetWorldLocation(FVector(-500.0f, -500.0f, -500.0f));
	Ref->Monster->CurrentTimer = 0;
}

void AShip::SlicedScroll(float HorizontalMovement)
{
	AddActorWorldOffset(FVector(HorizontalMovement*-2, 0.0f, 0.0f));
	
	AbsorbSprite->SetWorldLocation(FVector(-500.0f, -500.0f, -500.0f));
	Ref->Monster->CurrentTimer = 0;
}

void AShip::DisableAllShots()
{
	for (int i = 0; i < ProjectilesArray.Num(); ++i)
	{
		if (ProjectilesArray[i]->GetChildComponent(0)->IsVisible())
		{
			ProjectilesArray[i]->SetVisibility(false);
			ProjectilesArray[i]->GetChildComponent(0)->SetVisibility(false);
			ProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(FVector(-100.0f, -10.0f, -100.0f));
			ProjectilesArray[i]->SetWorldLocation(FVector(-100.0f, -10.0f, -100.0f));
			continue;
		}
	}
}

void AShip::ScrollLeft(float DeltaTime)
{
	FVector MoveVect = FVector(-DeltaTime*MaxHorizontalSpeed*3.0, 0.0f, 0.0f);

	AddActorWorldOffset(MoveVect*1.5); //move the ship
	CameraMove(MoveVect); 
	TopInnerBounds->AddWorldOffset(MoveVect);
	BottomInnerBounds->AddWorldOffset(MoveVect);

	Ref->Background->Scrollz(MoveVect);
	Ref->Bounds->AddActorWorldOffset(MoveVect);
	Ref->Monster->CurrentTimer = 0;
}

