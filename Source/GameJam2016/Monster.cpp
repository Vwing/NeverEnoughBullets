// Fill out your copyright notice in the Description page of Project Settings.

#include "GameJam2016.h"
#include "Monster.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "Ship.h"

// Sets default values
AMonster::AMonster()
{
	PrimaryActorTick.bCanEverTick = true;
	MaxShotsUsed = 20;

	MonsterSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MonsterSprite"));
	ConstructorHelpers::FObjectFinder<UPaperSprite> MonsterSpriteAsset(TEXT("PaperSprite'/Game/Sprites/BossSprites/Boss_Idle1.Boss_Idle1'"));
	MonsterSprite->SetSprite(MonsterSpriteAsset.Object);
	MonsterSprite->SetVisibility(false);

	MonsterSprite->bGenerateOverlapEvents = true;
	MonsterSprite->SetNotifyRigidBodyCollision(true);
	MonsterSprite->GetBodyInstance()->bLockZTranslation = true;
	SetActorEnableCollision(true);
	MonsterSprite->SetSimulatePhysics(false);
	MonsterSprite->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	MonsterSprite->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MonsterSprite->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	MonsterSprite->SetEnableGravity(false);
	MonsterSprite->bMultiBodyOverlap = true;
	MonsterSprite->GetBodyInstance()->bUseCCD = true;

	ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> IdleAnimationAsset(TEXT("PaperFlipbook'/Game/Sprites/BossSprites/BossIdle.BossIdle'"));
	IdleAnim = IdleAnimationAsset.Get();

	ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> DamagedAnimationAsset(TEXT("PaperFlipbook'/Game/Sprites/BossSprites/BossDamaged.BossDamaged'"));
	DamagedAnim = DamagedAnimationAsset.Get();

	ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> FastShotAnimationAsset(TEXT("PaperFlipbook'/Game/Sprites/BossSprites/BossQuickShot.BossQuickShot'"));
	FastShotAnim = FastShotAnimationAsset.Get();

	ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> SlowShotAnimationAsset(TEXT("PaperFlipbook'/Game/Sprites/BossSprites/BossSlowSHot.BossSlowShot'"));
	SlowShotAnim = SlowShotAnimationAsset.Get();

	RootComponent = MonsterSprite;
	MonsterFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("MonsterFlipbook"));
	MonsterFlipbook->SetFlipbook(IdleAnim);
	MonsterFlipbook->AttachTo(MonsterSprite);

	DamagedSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DamagedSound"));
	ConstructorHelpers::FObjectFinder<USoundBase> DamagedSoundAsset(TEXT("SoundWave'/Game/SFX/Death.Death'"));
	DamagedSound->SetSound(DamagedSoundAsset.Object);

	
	ConstructorHelpers::FObjectFinder<UPaperSprite> SpreadProjectileAsset(TEXT("PaperSprite'/Game/Sprites/NormalShot.NormalShot'"));
	SpreadProjectilesLocations.Reserve(10);
	SpreadProjectilesArray.Reserve(10);
	for (int i = 0; i < 10; i++)
	{
		FVector newLocation = FVector(0.0f, 0.0f, 0.0f);
		SpreadProjectilesLocations.Push(newLocation);

		UPaperSpriteComponent* SpreadProjectile = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpreadProjectilez" + i));
		SpreadProjectile->SetSprite(SpreadProjectileAsset.Object);

		SpreadProjectile->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		SpreadProjectile->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SpreadProjectile->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		SpreadProjectile->SetVisibility(false);
		SpreadProjectile->SetEnableGravity(false);
		SpreadProjectile->SetSimulatePhysics(false);
		SpreadProjectile->SetAbsolute(true, true, true);

		SpreadProjectilesArray.Push(SpreadProjectile);
	}


	ConstructorHelpers::FObjectFinder<UPaperSprite> StraightProjectileAsset(TEXT("PaperSprite'/Game/Sprites/NormalShot.NormalShot'"));
	StraightProjectilesArray.Reserve(10);
	StraightProjectilesLocations.Reserve(10);
	for (int i = 0; i < 10; i++)
	{
		FVector newLocation = FVector(0.0f, 0.0f, 0.0f);
		StraightProjectilesLocations.Push(newLocation);

		UPaperSpriteComponent* StraightProjectile = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("StraightProjectile" + i));
		StraightProjectile->SetSprite(StraightProjectileAsset.Object);

		StraightProjectile->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		StraightProjectile->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		StraightProjectile->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		StraightProjectile->SetVisibility(false);
		StraightProjectile->SetEnableGravity(false);
		StraightProjectile->SetSimulatePhysics(false);
		StraightProjectile->SetAbsolute(true, true, true);

		StraightProjectilesArray.Push(StraightProjectile);
	}

	
	ConstructorHelpers::FObjectFinder<UPaperSprite> RadialProjectileAsset(TEXT("PaperSprite'/Game/Sprites/NormalShot.NormalShot'"));
	RadialProjectilesLocations.Reserve(10);
	RadialProjectilesArray.Reserve(10);
	for (int i = 0; i < 10; i++)
	{
		FVector newLocation = FVector(0.0f, 0.0f, 0.0f);
		RadialProjectilesLocations.Push(newLocation);

		UPaperSpriteComponent* RadialProjectile = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("RadialProjectilew" + i));
		RadialProjectile->SetSprite(RadialProjectileAsset.Object);

		RadialProjectile->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		RadialProjectile->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		RadialProjectile->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		RadialProjectile->SetVisibility(false);
		RadialProjectile->SetEnableGravity(false);
		RadialProjectile->SetSimulatePhysics(false);
		RadialProjectile->SetAbsolute(true, true, true);

		RadialProjectilesArray.Push(RadialProjectile);
	}
	
	Health = 50;
	ShotsInUse = 0;
	MaxShotsUsed = 30;

	ProjectileRotation = FRotator(90.0f, 90.0f, 0.0f);
	MonsterState = EMonsterStates::Idle;
	ShipLocation = FVector(0.0f, 0.0f, 0.0f);
	Difficulty = EDifficulty::Easy;
	ProjectileSpeed = 300.0f;

	EasyMinShotMultiplier = 1.5f;
	EasyMaxShotMultiplier = 2.0f;
	NormalMinShotMultiplier = 2.0f;
	NormalMaxShotMultiplier = 2.5f;
	HardMinShotMultiplier = 2.5f;
	HardMaxShotMultiplier = 3.0f;

	EasyMinShootAgainTime = .5f;
	EasyMaxShootAgainTime = 1.0f;
	NormalMinShootAgainTime = .3f;
	NormalMaxShootAgainTime = .7f;
	HardMinShootAgainTime = .2f;
	HardMaxShootAgainTime = .5f;

	NumberOfSpreadShots = 10;
	NumberOfRadialShots = 10;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	DamagedSound->Stop();

	
	for (int i = 0; i < SpreadProjectilesArray.Num(); i++)
	{
		SpreadProjectilesArray[i]->RelativeRotation = ProjectileRotation;
		SpreadProjectilesArray[i]->SetWorldLocation(FVector(200.0f, 300.0f, -100.0f));
	}

	for (int i = 0; i < StraightProjectilesArray.Num(); i++)
	{
		StraightProjectilesArray[i]->RelativeRotation = ProjectileRotation;
		StraightProjectilesArray[i]->SetWorldLocation(FVector(200.0f, 200.0f, -100.0f));
	}

	
	for (int i = 0; i < RadialProjectilesArray.Num(); i++)
	{
		RadialProjectilesArray[i]->RelativeRotation = ProjectileRotation;
		RadialProjectilesArray[i]->SetWorldLocation(FVector(200.0f, 400.0f, -100.0f));
	}
	

	MinShootTime = .5f;
	MaxShootTime = 2;
	bCanShoot = true;
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ShotsInUse > 0)
	{
		UpdateProjectiles(DeltaTime);
	}

	if (bCanShoot == true)
	{
		bCanShoot = false;
		float ShootAgainTime = GetShootAgainTime();
		SetRandomShot();

		GetWorldTimerManager().SetTimer(ShootAgainHandle, this, &AMonster::SetCanShoot, ShootAgainTime, false);
	}

	switch (MonsterState)
	{
	case EMonsterStates::Idle:
		break;

	case EMonsterStates::ShootingStraight:
		break;

	case EMonsterStates::ShootingSlow:
		break;

	case EMonsterStates::ShootingRetaliationShot:
		break;

	case EMonsterStates::Damaged:
		break;

	case EMonsterStates::ErrorState:
		break;
	}

}

void AMonster::SetDamagedState()
{
	MonsterState = EMonsterStates::Damaged;
	SetDamagedAnim();
}

void AMonster::SetDamagedAnim()
{
	MonsterFlipbook->SetFlipbook(DamagedAnim);
	DamagedSound->Play();
	Health -= 1;
	UpdateDifficulty();

	GetWorldTimerManager().SetTimer(ShootAgainHandle, this, &AMonster::SetShootingRetaliationState, 5.0f, false);
	GetWorldTimerManager().SetTimer(MonsterAnimHandle, this, &AMonster::SetShootingRetaliationState, .40f, false);
}

void AMonster::SetIdleAnim()
{
	MonsterFlipbook->SetFlipbook(IdleAnim);
}

void AMonster::SetIdleState()
{
	MonsterState = EMonsterStates::Idle;
	SetIdleAnim();
}

void AMonster::SetShootingRetaliationState()
{
	MonsterState = EMonsterStates::ShootingRetaliationShot;
	SetShootingRetaliationAnim();

	ShootRetaliationProjectile();
}

void AMonster::SetShootingRetaliationAnim()
{
	MonsterFlipbook->SetFlipbook(FastShotAnim);

	float ShootAgainTime = GetShootAgainTime();
	GetWorldTimerManager().SetTimer(ShootAgainHandle, this, &AMonster::SetCanShoot, ShootAgainTime, false);
	GetWorldTimerManager().SetTimer(MonsterAnimHandle, this, &AMonster::SetIdleState, 1.0f, false);
}


void AMonster::ShootRetaliationProjectile()
{
	for (int i = 0; i < StraightProjectilesArray.Num(); i++)
	{
		if (StraightProjectilesArray[i]->IsVisible())
		{
			continue;
		}
		else
		{
			FVector ProjectileLocation = GetActorLocation();
			FVector NewShipLocation = GetShipLocation();

			if (Difficulty == EDifficulty::Easy)
			{
				StraightProjectilesLocations[i] = NewShipLocation * EasyMaxShotMultiplier;
			}
			else if (Difficulty == EDifficulty::Normal)
			{
				StraightProjectilesLocations[i] = NewShipLocation* NormalMaxShotMultiplier;
			}
			else if (Difficulty == EDifficulty::Hard)
			{
				StraightProjectilesLocations[i] = NewShipLocation* HardMaxShotMultiplier;
			}

			StraightProjectilesArray[i]->SetWorldLocation(ProjectileLocation);
			StraightProjectilesArray[i]->SetVisibility(true);
			ShotsInUse++;
			return;
		}
	}
}

void AMonster::SetShootingStraightState()
{
	MonsterState = EMonsterStates::ShootingStraight;
}

void AMonster::SetFastShotAnim()
{
	MonsterFlipbook->SetFlipbook(FastShotAnim);
	ShootFastProjectile();

	GetWorldTimerManager().SetTimer(MonsterAnimHandle, this, &AMonster::SetIdleState, .70f, false);
}

void AMonster::SetSpreadShotAnim()
{
	MonsterFlipbook->SetFlipbook(SlowShotAnim);
	ShootSpreadProjectiles();

	GetWorldTimerManager().SetTimer(MonsterAnimHandle, this, &AMonster::SetIdleState, 1.2f, false);
}

void AMonster::SetRadialShotAnim()
{
	ShootRadialProjectiles();

	GetWorldTimerManager().SetTimer(MonsterAnimHandle, this, &AMonster::SetIdleState, .32f, false);
}

void AMonster::ShootRadialProjectiles()
{
	int counter = 0;
	FVector NewShipLocation = FVector(-1.0f, 0.0f, 0.0f)*ProjectileSpeed;

	for (int i = 0; i < RadialProjectilesArray.Num() && counter < NumberOfRadialShots; i++)
	{
		if (RadialProjectilesArray[i]->IsVisible())
		{
			continue;
		}
		else
		{
			FVector ProjectileLocation = GetActorLocation();
			if (counter % 2 == 0)
			{
				NewShipLocation = FVector(-1.0f, (counter * .15f), 0.0f)*ProjectileSpeed;
			}
			else
			{
				NewShipLocation = FVector(-1.0f, -(counter * .15f), 0.0f)*ProjectileSpeed;
			}

			if (Difficulty == EDifficulty::Easy)
			{
				float ShotSpeedMultiplier = FMath::FRandRange(EasyMinShotMultiplier, EasyMaxShotMultiplier);
				RadialProjectilesLocations[i] = NewShipLocation * ShotSpeedMultiplier;
			}
			else if (Difficulty == EDifficulty::Normal)
			{
				float ShotSpeedMultiplier = FMath::FRandRange(NormalMinShotMultiplier, NormalMinShotMultiplier);
				RadialProjectilesLocations[i] = NewShipLocation* ShotSpeedMultiplier;
			}
			else if (Difficulty == EDifficulty::Hard)
			{
				float ShotSpeedMultiplier = FMath::FRandRange(HardMinShotMultiplier, HardMaxShotMultiplier);
				RadialProjectilesLocations[i] = NewShipLocation* ShotSpeedMultiplier;
			}

			RadialProjectilesArray[i]->SetWorldLocation(ProjectileLocation);
			RadialProjectilesArray[i]->SetVisibility(true);
			ShotsInUse++;
			counter++;
		}
	}
}

void AMonster::ShootFastProjectile()
{
	for (int i = 0; i < StraightProjectilesArray.Num(); i++)
	{
		if (StraightProjectilesArray[i]->IsVisible())
		{
			continue;
		}
		else
		{
			FVector ProjectileLocation = GetActorLocation();
			FVector NewShipLocation = GetShipLocation();

			if (Difficulty == EDifficulty::Easy)
			{
				float ShotSpeedMultiplier = FMath::FRandRange(EasyMinShotMultiplier, EasyMaxShotMultiplier);
				StraightProjectilesLocations[i] = NewShipLocation * ShotSpeedMultiplier;
			}
			else if (Difficulty == EDifficulty::Normal)
			{
				float ShotSpeedMultiplier = FMath::FRandRange(NormalMinShotMultiplier, NormalMinShotMultiplier);
				StraightProjectilesLocations[i] = NewShipLocation* ShotSpeedMultiplier;
			}
			else if (Difficulty == EDifficulty::Hard)
			{
				float ShotSpeedMultiplier = FMath::FRandRange(HardMinShotMultiplier, HardMaxShotMultiplier);
				StraightProjectilesLocations[i] = NewShipLocation* ShotSpeedMultiplier;
			}

			StraightProjectilesArray[i]->SetWorldLocation(ProjectileLocation);
			StraightProjectilesArray[i]->SetVisibility(true);
			ShotsInUse++;
			return;
		}
	}
}

void AMonster::ShootSpreadProjectiles()
{
	int counter = 0;
	for (int i = 0; i < SpreadProjectilesArray.Num() && counter < NumberOfSpreadShots; i++)
	{
		if (SpreadProjectilesArray[i]->IsVisible())
		{
			continue;
		}
		else
		{
			FVector ProjectileLocation = GetActorLocation();
			FVector NewShipLocation = GetShipLocation() + counter*20;

			if (Difficulty == EDifficulty::Easy)
			{
				float ShotSpeedMultiplier = FMath::FRandRange(EasyMinShotMultiplier, EasyMaxShotMultiplier);
				SpreadProjectilesLocations[i] = NewShipLocation * ShotSpeedMultiplier;
			}
			else if (Difficulty == EDifficulty::Normal)
			{
				float ShotSpeedMultiplier = FMath::FRandRange(NormalMinShotMultiplier, NormalMinShotMultiplier);
				SpreadProjectilesLocations[i] = NewShipLocation* ShotSpeedMultiplier;
			}
			else if (Difficulty == EDifficulty::Hard)
			{
				float ShotSpeedMultiplier = FMath::FRandRange(HardMinShotMultiplier, HardMaxShotMultiplier);
				SpreadProjectilesLocations[i] = NewShipLocation* ShotSpeedMultiplier;
			}

			SpreadProjectilesArray[i]->SetWorldLocation(ProjectileLocation);
			SpreadProjectilesArray[i]->SetVisibility(true);
			ShotsInUse++;
			counter++;
		}
	}
}


void AMonster::UpdateProjectiles(float DeltaTime)
{
	for (int i = 0; i < StraightProjectilesArray.Num(); i++)
	{
		if (StraightProjectilesArray[i]->IsVisible())
		{
			StraightProjectilesArray[i]->GetOverlappingComponents(OverlappingComponents);
			if (UpdateOverlappingProjectiles(OverlappingComponents)) //check if its overlapping to destroy it
			{
				StraightProjectilesArray[i]->SetVisibility(false);
				StraightProjectilesArray[i]->SetWorldLocation(FVector(100.0f, 100.0f, -100.0f));
				continue;
			}

			FVector NewProjectileLocation = FVector(StraightProjectilesArray[i]->GetComponentLocation().X + StraightProjectilesLocations[i].X*DeltaTime,
				StraightProjectilesArray[i]->GetComponentLocation().Y + StraightProjectilesLocations[i].Y * DeltaTime, 0.0f);
			DebugLocation = StraightProjectilesArray[i]->GetComponentLocation();
			DebugLocation2 = StraightProjectilesLocations[i];

			StraightProjectilesArray[i]->SetWorldLocation(NewProjectileLocation);
		}
	}

	for (int i = 0; i < SpreadProjectilesArray.Num(); i++)
	{
		if (SpreadProjectilesArray[i]->IsVisible())
		{
			SpreadProjectilesArray[i]->GetOverlappingComponents(OverlappingComponents);
			if (UpdateOverlappingProjectiles(OverlappingComponents)) //check if its overlapping to destroy it
			{
				SpreadProjectilesArray[i]->SetVisibility(false);
				SpreadProjectilesArray[i]->SetWorldLocation(FVector(100.0f, 100.0f, -100.0f));
				continue;
			}

			FVector NewProjectileLocation = FVector(SpreadProjectilesArray[i]->GetComponentLocation().X + SpreadProjectilesLocations[i].X*DeltaTime,
				SpreadProjectilesArray[i]->GetComponentLocation().Y + SpreadProjectilesLocations[i].Y * DeltaTime, 0.0f);
			DebugLocation = SpreadProjectilesArray[i]->GetComponentLocation();
			DebugLocation2 = SpreadProjectilesLocations[i];

			SpreadProjectilesArray[i]->SetWorldLocation(NewProjectileLocation);
		}
	}

	for (int i = 0; i < RadialProjectilesArray.Num(); i++)
	{
		if (RadialProjectilesArray[i]->IsVisible())
		{
			RadialProjectilesArray[i]->GetOverlappingComponents(OverlappingComponents);
			if (UpdateOverlappingProjectiles(OverlappingComponents)) //check if its overlapping to destroy it
			{
				RadialProjectilesArray[i]->SetVisibility(false);
				RadialProjectilesArray[i]->SetWorldLocation(FVector(100.0f, 100.0f, -100.0f));
				continue;
			}

			FVector NewProjectileLocation = FVector(RadialProjectilesArray[i]->GetComponentLocation().X + RadialProjectilesLocations[i].X*DeltaTime,
				RadialProjectilesArray[i]->GetComponentLocation().Y + RadialProjectilesLocations[i].Y * DeltaTime, 0.0f);
			DebugLocation = RadialProjectilesArray[i]->GetComponentLocation();
			DebugLocation2 = RadialProjectilesLocations[i];

			RadialProjectilesArray[i]->SetWorldLocation(NewProjectileLocation);
		}
	}
}

void AMonster::SetRandomShot()
{
	float RandomShot = 2;//FMath::RandRange(0, 2);
	if (RandomShot == 1)
	{
		SetFastShotAnim();
	}
	else if (RandomShot == 0)
	{
		SetSpreadShotAnim();
	}
	else if (RandomShot == 2)
	{
		SetRadialShotAnim();
	}
}

bool AMonster::UpdateOverlappingProjectiles(TArray<UPrimitiveComponent*>& OverlappingComponents)
{ //might cause crashing when both referencing each other
	for (int i = 0; i < OverlappingComponents.Num(); i++)
	{
		if (OverlappingComponents[i]->GetName().Contains("ShipSprite"))
		{
			DebugString = "ShipSprite";
			AShip* ship = Cast<AShip>(OverlappingComponents[i]->GetAttachmentRootActor());

			ship->bIsDead = true;
			
			ShotsInUse -= 1;
			return true;
		}
		else if (OverlappingComponents[i]->GetName().Contains("AbsorbSprite"))
		{
			DebugString = "AbsorbSprite";
			AShip* ship = Cast<AShip>(OverlappingComponents[i]->GetAttachmentRootActor());
			ship->Ammo += 1;
			ShotsInUse -= 1;
			return true;
		}
		else if (OverlappingComponents[i]->GetName().Contains("LeftBoundsSprite"))
		{
			DebugString = "LeftBoundsSprite";
			ShotsInUse -= 1;
			return true;
		}
		else if (OverlappingComponents[i]->GetName().Contains("TopBoundsSprite"))
		{
			DebugString = "TopBoundsSprite";
			ShotsInUse -= 1;
			return true;
		}
		else if (OverlappingComponents[i]->GetName().Contains("BottomBoundsSprite"))
		{
			DebugString = "BottomBoundsSprite";
			ShotsInUse -= 1;
			return true;
		}
		else if (OverlappingComponents[i]->GetName().Contains("RightBoundsSprite"))
		{
			DebugString = "RightBoundsSprite";
			ShotsInUse -= 1;
			return true;
		}
	}
	return false;
}

void AMonster::UpdateDifficulty()
{
	if (Health > 30)
	{
		Difficulty = EDifficulty::Easy;
	}
	else if (Health > 15)
	{
		Difficulty = EDifficulty::Normal;
	}
	else
	{
		Difficulty = EDifficulty::Hard;
	}
}

void AMonster::SetCanShoot()
{
	bCanShoot = true;
}

float AMonster::GetShootAgainTime()
{
	if (Difficulty == EDifficulty::Easy)
	{
		return FMath::FRandRange(EasyMinShootAgainTime, EasyMaxShootAgainTime);
	}
	else if (Difficulty == EDifficulty::Normal)
	{
		return FMath::FRandRange(NormalMinShootAgainTime, NormalMaxShootAgainTime);
	}
	else if (Difficulty == EDifficulty::Hard)
	{
		return FMath::FRandRange(HardMinShootAgainTime, HardMaxShootAgainTime);
	}
	return 2;
}
FVector AMonster::GetShipLocation()
{
	FVector TargetDirection = ShipLocation - GetActorLocation();
	TargetDirection.Normalize(0.01f);
	return TargetDirection*ProjectileSpeed;	
}