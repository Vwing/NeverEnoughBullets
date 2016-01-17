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
	MonsterSprite->SetVisibility(true);

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

	RootComponent = MonsterSprite;
	MonsterFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("MonsterFlipbook"));
	MonsterFlipbook->SetFlipbook(IdleAnim);
	MonsterFlipbook->AttachTo(MonsterSprite);

	DamagedSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DamagedSound"));
	ConstructorHelpers::FObjectFinder<USoundBase> DamagedSoundAsset(TEXT("SoundWave'/Game/SFX/Death.Death'"));
	DamagedSound->SetSound(DamagedSoundAsset.Object);

	/*
	ConstructorHelpers::FObjectFinder<UPaperSprite> SlowProjectileAsset(TEXT("PaperSprite'/Game/Sprites/NormalShot.NormalShot'"));
	SinProjectilesArray.Reserve(10);
	for (int i = 0; i < 10; i++)
	{
	UPaperSpriteComponent* SlowProjectile = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SinProjectile" + i));
	SlowProjectile->SetSprite(SlowProjectileAsset.Object);

	SlowProjectile->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	SlowProjectile->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SlowProjectile->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SlowProjectile->SetVisibility(false);
	SlowProjectile->SetEnableGravity(false);
	SlowProjectile->SetSimulatePhysics(false);
	SlowProjectile->SetAbsolute(true, true, true);

	SinProjectilesArray.Push(SlowProjectile);
	} // actually closes here but cant comment whole section out
	*/
	


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

	Health = 50;
	ShotsInUse = 0;
	ProjectileRotation = FRotator(90.0f, 90.0f, 0.0f);
	MonsterState = EMonsterStates::Idle;
	ShipLocation = FVector(0.0f, 0.0f, 0.0f);
	Difficulty = EDifficulty::Easy;

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
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	DamagedSound->Stop();

	/*
	for (int i = 0; i < SinProjectilesArray.Num(); i++)
	{
	SinProjectilesArray[i]->RelativeRotation = ProjectileRotation;
	SinProjectilesArray[i]->SetWorldLocation(FVector(0.0f, 0.0f, -100.0f));
	}
	*/

	for (int i = 0; i < StraightProjectilesArray.Num(); i++)
	{
		StraightProjectilesArray[i]->RelativeRotation = ProjectileRotation;
		StraightProjectilesArray[i]->SetWorldLocation(FVector(200.0f, 200.0f, -100.0f));
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
			if (Difficulty == EDifficulty::Easy)
			{
				StraightProjectilesLocations[i] = ShipLocation * EasyMaxShotMultiplier;
			}
			else if (Difficulty == EDifficulty::Normal)
			{
				StraightProjectilesLocations[i] = ShipLocation* NormalMaxShotMultiplier;
			}
			else if (Difficulty == EDifficulty::Hard)
			{
				StraightProjectilesLocations[i] = ShipLocation* HardMaxShotMultiplier;
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
			if (Difficulty == EDifficulty::Easy)
			{
				float ShotSpeedMultiplier = FMath::FRandRange(EasyMinShotMultiplier, EasyMaxShotMultiplier);
				StraightProjectilesLocations[i] = ShipLocation * ShotSpeedMultiplier;
			}
			else if (Difficulty == EDifficulty::Normal)
			{
				float ShotSpeedMultiplier = FMath::FRandRange(NormalMinShotMultiplier, NormalMinShotMultiplier);
				StraightProjectilesLocations[i] = ShipLocation* ShotSpeedMultiplier;
			}
			else if (Difficulty == EDifficulty::Hard)
			{
				float ShotSpeedMultiplier = FMath::FRandRange(HardMinShotMultiplier, HardMaxShotMultiplier);
				StraightProjectilesLocations[i] = ShipLocation* ShotSpeedMultiplier;
			}

			StraightProjectilesArray[i]->SetWorldLocation(ProjectileLocation);
			StraightProjectilesArray[i]->SetVisibility(true);
			ShotsInUse++;
			return;
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
}

void AMonster::SetRandomShot()
{
	SetFastShotAnim();
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