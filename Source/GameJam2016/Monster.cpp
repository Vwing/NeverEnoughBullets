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
	FastProjectileLocations.Reserve(10);
	for (int i = 0; i < 10; i++)
	{
		FVector newLocation = FVector(0.0f, 0.0f, 0.0f);
		FastProjectileLocations.Push(newLocation);

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
	MaxShotsUsed = 10;
	ProjectileRotation = FRotator(90.0f, 90.0f, 0.0f);
	MonsterState = EMonsterStates::Idle;
	ShipLocation = FVector(0.0f, 0.0f, 0.0f);

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
		StraightProjectilesArray[i]->SetWorldLocation(FVector(0.0f, 0.0f, -100.0f));
	}

}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ShotsInUse > 0)
	{
		UpdateProjectiles(DeltaTime);
	}
	switch (MonsterState)
	{
	case EMonsterStates::Idle:
		break;

	case EMonsterStates::ShootingFast:

		break;

	case EMonsterStates::ShootingSlow:
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

	GetWorldTimerManager().SetTimer(MonsterHandle, this, &AMonster::SetShootingFastState, 0.50f, false);
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

void AMonster::SetShootingFastState()
{
	MonsterState = EMonsterStates::ShootingFast;
	SetFastShotAnim();
}

void AMonster::SetFastShotAnim()
{
	MonsterFlipbook->SetFlipbook(FastShotAnim);
	ShootFastProjectile();

	GetWorldTimerManager().SetTimer(MonsterHandle, this, &AMonster::SetIdleState, 1.0f, false);
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
			FastProjectileLocations[i] = ShipLocation;

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
				StraightProjectilesArray[i]->SetWorldLocation(FVector(-100.0f, -100.0f, -100.0f));
				continue;
			}

			FVector NewProjectileLocation = FVector(StraightProjectilesArray[i]->GetComponentLocation().X + FastProjectileLocations[i].X*DeltaTime * 2,
				StraightProjectilesArray[i]->GetComponentLocation().Y + FastProjectileLocations[i].Y * DeltaTime * 2, 0.0f);
			DebugLocation = StraightProjectilesArray[i]->GetComponentLocation();
			DebugLocation2 = FastProjectileLocations[i];

			StraightProjectilesArray[i]->SetWorldLocation(NewProjectileLocation);
		}
	}
}

bool AMonster::UpdateOverlappingProjectiles(TArray<UPrimitiveComponent*>& OverlappingComponents)
{
	for (int i = 0; i < OverlappingComponents.Num(); i++)
	{
		if (OverlappingComponents[i]->GetName().Contains("ShipSprite"))
		{
			DebugString = "ShipSprite";
			AShip* ship = Cast<AShip>(OverlappingComponents[i]->GetAttachmentRootActor());

			ship->bIsDead = true;
			// ship death animation goes here
			//monster = nullptr;
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
