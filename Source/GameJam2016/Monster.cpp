#include "GameJam2016.h"
#include "Monster.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "Ship.h"

// Sets default values
AMonster::AMonster()
{
	PrimaryActorTick.bCanEverTick = true;
	MaxShotsUsed = 30;
	ProjectileAnimScale = FVector(3.0f, 3.0f, 3.0f);
	ProjectileSpriteScale = FVector(1.0f, 1.0f, 1.0f);

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
	ConstructorHelpers::FObjectFinder<USoundBase> DamagedSoundAsset(TEXT("SoundWave'/Game/SFX/Monster_Hurt.Monster_Hurt'"));
	DamagedSound->SetSound(DamagedSoundAsset.Object);

	DangerSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DangerSound"));
	ConstructorHelpers::FObjectFinder<USoundBase> DangerSoundAsset(TEXT("SoundWave'/Game/SFX/Danger.Danger'"));
	DangerSound->SetSound(DangerSoundAsset.Object);

	IdleSound = CreateDefaultSubobject<UAudioComponent>(TEXT("IdleSound"));
	ConstructorHelpers::FObjectFinder<USoundBase> IdleSoundAsset(TEXT("SoundWave'/Game/SFX/Monster_Idle.Monster_Idle'"));
	IdleSound->SetSound(IdleSoundAsset.Object);

	RetaliationSound = CreateDefaultSubobject<UAudioComponent>(TEXT("RetaliationSound"));
	ConstructorHelpers::FObjectFinder<USoundBase> RetaliationSoundAsset(TEXT("SoundWave'/Game/SFX/Retaliation_Noise.Retaliation_Noise'"));
	RetaliationSound->SetSound(RetaliationSoundAsset.Object);

	RoarSound = CreateDefaultSubobject<UAudioComponent>(TEXT("RoarSound"));
	ConstructorHelpers::FObjectFinder<USoundBase> RoarSoundAsset(TEXT("SoundWave'/Game/SFX/Roar.Roar'"));
	RoarSound->SetSound(RoarSoundAsset.Object);

	ExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ExplosionSound"));
	ConstructorHelpers::FObjectFinder<USoundBase> ExplosionSoundAsset(TEXT("SoundWave'/Game/SFX/Explosion.Explosion'"));
	ExplosionSound->SetSound(ExplosionSoundAsset.Object);

	CaughtShotSound = CreateDefaultSubobject<UAudioComponent>(TEXT("CaughtShotSound"));
	ConstructorHelpers::FObjectFinder<USoundBase> CaughtShotSoundAsset(TEXT("SoundWave'/Game/SFX/Caught_Shot.Caught_Shot'"));
	CaughtShotSound->SetSound(CaughtShotSoundAsset.Object);


	ConstructorHelpers::FObjectFinder<UPaperSprite> SpreadProjectileAsset(TEXT("PaperSprite'/Game/Sprites/NormalShot.NormalShot'"));
	ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> YellowProjectileAnimAsset(TEXT("PaperFlipbook'/Game/Sprites/BulletSprites/MonsterBulletYellow.MonsterBulletYellow'"));
	ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> BlueProjectileAnimAsset(TEXT("PaperFlipbook'/Game/Sprites/BulletSprites/MonsterBulletBlue.MonsterBulletBlue'"));

	SpreadProjectilesLocations.Reserve(10);
	SpreadProjectilesArray.Reserve(10);
	for (int i = 0; i < 10; i++)
	{
		FVector newLocation = FVector(0.0f, 0.0f, 0.0f);
		SpreadProjectilesLocations.Push(newLocation);

		UPaperSpriteComponent* SpreadProjectile = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpreadProjectilez" + i));
		SpreadProjectile->SetSprite(SpreadProjectileAsset.Object);

		SpreadProjectile->AttachTo(RootComponent);

		SpreadProjectile->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		SpreadProjectile->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SpreadProjectile->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		SpreadProjectile->SetVisibility(false);
		SpreadProjectile->SetEnableGravity(false);
		SpreadProjectile->SetSimulatePhysics(false);
		SpreadProjectile->SetAbsolute(true, true, true);

		UPaperFlipbookComponent* SpreadProjectileAnim = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("SpreadProjectileAnim" + i));
		SpreadProjectileAnim->SetFlipbook(YellowProjectileAnimAsset.Get());

		if (i == 3 || i == 6 || i == 9)
		{
			SpreadProjectileAnim->SetFlipbook(BlueProjectileAnimAsset.Get());
		}
		SpreadProjectileAnim->AttachTo(SpreadProjectile);
		SpreadProjectileAnim->SetAbsolute(true, true, true);
		SpreadProjectileAnim->SetWorldScale3D(ProjectileAnimScale);
		SpreadProjectileAnim->SetVisibility(false);

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
		StraightProjectile->AttachTo(RootComponent);

		StraightProjectile->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		StraightProjectile->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		StraightProjectile->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		StraightProjectile->SetVisibility(false);
		StraightProjectile->SetEnableGravity(false);
		StraightProjectile->SetSimulatePhysics(false);
		StraightProjectile->SetAbsolute(true, true, true);

		UPaperFlipbookComponent* StraightProjectileAnim = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("StraightProjcAnim" + i));
		StraightProjectileAnim->SetFlipbook(YellowProjectileAnimAsset.Get());

		if (i == 3 || i == 6 || i == 9)
		{
			StraightProjectileAnim->SetFlipbook(BlueProjectileAnimAsset.Get());
		}
		StraightProjectileAnim->AttachTo(StraightProjectile);
		StraightProjectileAnim->SetAbsolute(true, true, true);
		StraightProjectileAnim->SetWorldScale3D(ProjectileAnimScale);
		StraightProjectileAnim->SetVisibility(false);

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
		RadialProjectile->AttachTo(RootComponent);

		UPaperFlipbookComponent* RadialProjectileAnim = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("RadialProjectiAnim" + i));
		RadialProjectileAnim->SetFlipbook(YellowProjectileAnimAsset.Get());

		if (i == 3 || i == 6 || i == 9)
		{
			RadialProjectileAnim->SetFlipbook(BlueProjectileAnimAsset.Get());
		}
		RadialProjectileAnim->AttachTo(RadialProjectile);
		RadialProjectileAnim->SetAbsolute(true, true, true);
		RadialProjectileAnim->SetWorldScale3D(ProjectileAnimScale);
		RadialProjectileAnim->SetVisibility(false);

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

	ProjectileAnimOffset = FVector(50.0f, 0.0f, 1.0f);
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();

	DamagedSound->Stop();
	DangerSound->Stop();
	IdleSound->Stop();
	RetaliationSound->Stop();
	RoarSound->Stop();
	ExplosionSound->Stop();
	CaughtShotSound->Stop();

	for (int i = 0; i < SpreadProjectilesArray.Num(); i++)
	{
		SetInitialProjectileSettings(SpreadProjectilesArray[i]);
	}

	for (int i = 0; i < StraightProjectilesArray.Num(); i++)
	{
		SetInitialProjectileSettings(StraightProjectilesArray[i]);
	}

	for (int i = 0; i < RadialProjectilesArray.Num(); i++)
	{
		SetInitialProjectileSettings(RadialProjectilesArray[i]);
	}


	MinShootTime = .5f;
	MaxShootTime = 2;
	bCanShoot = false;
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
	Health -= 1;
	UpdateDifficulty();

	SetDamagedAnim();
	SetShootingRetaliationState();
}

void AMonster::SetDamagedAnim()
{
	MonsterFlipbook->SetFlipbook(DamagedAnim);
	DamagedSound->Play();

	GetWorldTimerManager().SetTimer(ShootAgainHandle, this, &AMonster::SetShootingRetaliationState, 3.0f, false);
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
	RetaliationSound->Play();
	float ShootAgainTime = GetShootAgainTime();
	GetWorldTimerManager().SetTimer(ShootAgainHandle, this, &AMonster::SetCanShoot, ShootAgainTime, false);
	GetWorldTimerManager().SetTimer(MonsterAnimHandle, this, &AMonster::SetIdleState, 1.0f, false);
}


void AMonster::ShootRetaliationProjectile()
{
	for (int i = 0; i < StraightProjectilesArray.Num(); i++)
	{
		if (StraightProjectilesArray[i]->GetChildComponent(0)->IsVisible())
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
			//	StraightProjectilesArray[i]->SetVisibility(true);
			StraightProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(ProjectileLocation + ProjectileAnimOffset);
			StraightProjectilesArray[i]->GetChildComponent(0)->SetVisibility(true);
			ShotsInUse++;
			return;
		}
	}
}

void AMonster::SetShootingStraightState()
{
	MonsterState = EMonsterStates::ShootingStraight;
	SetFastShotAnim();
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
	for (int i = 0; i < RadialProjectilesArray.Num() && counter < NumberOfRadialShots; i++)
	{
		if (RadialProjectilesArray[i]->GetChildComponent(0)->IsVisible())
		{
			continue;
		}
		else
		{
			FVector ProjectileLocation = GetActorLocation();
			FVector NewShipLocation = FVector(-1.0f, 0.0f, 0.0f);

			if (counter % 2 == 0)
			{
				NewShipLocation.Y = (counter*.15);
			}
			else
			{
				NewShipLocation.Y = (counter*.15)*-1;
			}
			NewShipLocation.Normalize(0.01f);
			NewShipLocation *= ProjectileSpeed;


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
			//	RadialProjectilesArray[i]->SetVisibility(true);
			RadialProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(ProjectileLocation + ProjectileAnimOffset);
			RadialProjectilesArray[i]->GetChildComponent(0)->SetVisibility(true);
			ShotsInUse++;
			counter++;
		}
	}
}

void AMonster::ShootFastProjectile()
{
	for (int i = 0; i < StraightProjectilesArray.Num(); i++)
	{
		if (StraightProjectilesArray[i]->GetChildComponent(0)->IsVisible())
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
			//StraightProjectilesArray[i]->SetVisibility(true);
			StraightProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(ProjectileLocation + ProjectileAnimOffset);
			StraightProjectilesArray[i]->GetChildComponent(0)->SetVisibility(true);
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
		if (SpreadProjectilesArray[i]->GetChildComponent(0)->IsVisible())
		{
			continue;
		}
		else
		{
			FVector ProjectileLocation = GetActorLocation();
			FVector NewShipLocation = GetShipLocation() + counter * 20;

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
			//	SpreadProjectilesArray[i]->SetVisibility(true);

			SpreadProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(ProjectileLocation + ProjectileAnimOffset);
			SpreadProjectilesArray[i]->GetChildComponent(0)->SetVisibility(true);
			ShotsInUse++;
			counter++;
		}
	}
}


void AMonster::UpdateProjectiles(float DeltaTime)
{
	for (int i = 0; i < StraightProjectilesArray.Num(); i++)
	{
		if (StraightProjectilesArray[i]->GetChildComponent(0)->IsVisible())
		{
			StraightProjectilesArray[i]->GetOverlappingComponents(OverlappingComponents);
			if (UpdateOverlappingProjectiles(OverlappingComponents)) //check if its overlapping to destroy it
			{
				StraightProjectilesArray[i]->SetVisibility(false);
				StraightProjectilesArray[i]->SetWorldLocation(FVector(100.0f, 100.0f, -100.0f));
				StraightProjectilesArray[i]->GetChildComponent(0)->SetVisibility(false);
				StraightProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(FVector(100.0f, 100.0f, -100.0f));
				continue;
			}

			FVector NewProjectileLocation = FVector(StraightProjectilesArray[i]->GetComponentLocation().X + StraightProjectilesLocations[i].X*DeltaTime,
				StraightProjectilesArray[i]->GetComponentLocation().Y + StraightProjectilesLocations[i].Y * DeltaTime, 0.0f);

			StraightProjectilesArray[i]->SetWorldLocation(NewProjectileLocation);
			StraightProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(NewProjectileLocation + ProjectileAnimOffset);
		}
	}

	for (int i = 0; i < SpreadProjectilesArray.Num(); i++)
	{
		if (SpreadProjectilesArray[i]->GetChildComponent(0)->IsVisible())
		{
			SpreadProjectilesArray[i]->GetOverlappingComponents(OverlappingComponents);
			if (UpdateOverlappingProjectiles(OverlappingComponents)) //check if its overlapping to destroy it
			{
				SpreadProjectilesArray[i]->SetVisibility(false);
				SpreadProjectilesArray[i]->SetWorldLocation(FVector(100.0f, 100.0f, -100.0f));

				SpreadProjectilesArray[i]->GetChildComponent(0)->SetVisibility(false);
				SpreadProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(FVector(100.0f, 100.0f, -100.0f));
				continue;
			}

			FVector NewProjectileLocation = FVector(SpreadProjectilesArray[i]->GetComponentLocation().X + SpreadProjectilesLocations[i].X*DeltaTime,
				SpreadProjectilesArray[i]->GetComponentLocation().Y + SpreadProjectilesLocations[i].Y * DeltaTime, 0.0f);
			DebugLocation = SpreadProjectilesArray[i]->GetComponentLocation();
			DebugLocation2 = SpreadProjectilesLocations[i];

			SpreadProjectilesArray[i]->SetWorldLocation(NewProjectileLocation);
			SpreadProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(NewProjectileLocation + ProjectileAnimOffset);
		}
	}

	for (int i = 0; i < RadialProjectilesArray.Num(); i++)
	{
		if (RadialProjectilesArray[i]->GetChildComponent(0)->IsVisible())
		{
			RadialProjectilesArray[i]->GetOverlappingComponents(OverlappingComponents);
			if (UpdateOverlappingProjectiles(OverlappingComponents)) //check if its overlapping to destroy it
			{
				RadialProjectilesArray[i]->SetVisibility(false);
				RadialProjectilesArray[i]->SetWorldLocation(FVector(100.0f, 100.0f, -100.0f));

				RadialProjectilesArray[i]->GetChildComponent(0)->SetVisibility(false);
				RadialProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(FVector(100.0f, 100.0f, -100.0f));
				continue;
			}

			FVector NewProjectileLocation = FVector(RadialProjectilesArray[i]->GetComponentLocation().X + RadialProjectilesLocations[i].X*DeltaTime,
				RadialProjectilesArray[i]->GetComponentLocation().Y + RadialProjectilesLocations[i].Y * DeltaTime, 0.0f);

			RadialProjectilesArray[i]->SetWorldLocation(NewProjectileLocation);
			RadialProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(NewProjectileLocation + ProjectileAnimOffset);
		}
	}
}

void AMonster::SetRandomShot()
{
	float RandomShot = FMath::RandRange(0, 2);
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
			CaughtShotSound->Play();
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

void AMonster::SetInitialProjectileSettings(UPaperSpriteComponent*& PrimComp)
{
	PrimComp->RelativeRotation = ProjectileRotation;
	PrimComp->SetWorldLocation(FVector(200.0f, 300.0f, -100.0f));
	PrimComp->SetWorldScale3D(ProjectileSpriteScale);

	PrimComp->GetChildComponent(0)->RelativeRotation = ProjectileRotation;
	PrimComp->GetChildComponent(0)->SetWorldLocation(FVector(200.0f, 300.0f, -100.0f));
	PrimComp->GetChildComponent(0)->SetWorldScale3D(ProjectileAnimScale);
}