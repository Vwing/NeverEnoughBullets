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

	/*
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
	SlowProjectilesArray.Reserve(10);
	for (int i = 0; i < 10; i++)
	{
		//FString Name = "Projectile" + i;
		UPaperSpriteComponent* SlowProjectile = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SlowProjectile" + i));
		SlowProjectile->SetSprite(SlowProjectileAsset.Object);

		SlowProjectile->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		SlowProjectile->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SlowProjectile->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		SlowProjectile->SetVisibility(false);
		SlowProjectile->SetEnableGravity(false);
		SlowProjectile->SetSimulatePhysics(false);
		SlowProjectile->SetAbsolute(true, true, true);

		SlowProjectilesArray.Push(SlowProjectile);
	} // actually closes here but cant comment whole section out
	


	ConstructorHelpers::FObjectFinder<UPaperSprite> FastProjectileAsset(TEXT("PaperSprite'/Game/Sprites/NormalShot.NormalShot'"));
	FastProjectilesArray.Reserve(10);
	FastProjectileLocations.Reserve(10);
	for (int i = 0; i < 10; i++)
	{
		//FString Name = "Projectile" + i;
		FVector newLocation = FVector(0.0f, 0.0f, 0.0f);
		FastProjectileLocations.Push(newLocation);

		UPaperSpriteComponent* FastProjectile = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("FastProjectile" + i));
		FastProjectile->SetSprite(FastProjectileAsset.Object);

		FastProjectile->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		FastProjectile->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		FastProjectile->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		FastProjectile->SetVisibility(false);
		FastProjectile->SetEnableGravity(false);
		FastProjectile->SetSimulatePhysics(false);
		FastProjectile->SetAbsolute(true, true, true);

		FastProjectilesArray.Push(FastProjectile);
	}

	Health = 50;
	ShotsInUse = 0;
	MaxShotsUsed = 10;
	ProjectileRotation = FRotator(90.0f, 90.0f, 0.0f);
	MonsterState = EMonsterStates::Idle;
	ShipLocation = FVector(0.0f, 0.0f, 0.0f);
	*/
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	//DamagedSound->Stop();

	/*
	for (int i = 0; i < SlowProjectilesArray.Num(); i++)
	{
		SlowProjectilesArray[i]->RelativeRotation = ProjectileRotation;
		SlowProjectilesArray[i]->SetWorldLocation(FVector(0.0f, 0.0f, -100.0f));
	}
	*/
	/*
	for (int i = 0; i < FastProjectilesArray.Num(); i++)
	{
		FastProjectilesArray[i]->RelativeRotation = ProjectileRotation;
		FastProjectilesArray[i]->SetWorldLocation(FVector(0.0f, 0.0f, -100.0f));
	}
	*/
}

// Called every frame
void AMonster::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
/*
	switch (MonsterState)
	{
	case EMonsterStates::Idle:
		break;

	case EMonsterStates::ShootingFast:
		UpdateProjectiles(DeltaTime);
		break;

	case EMonsterStates::ShootingSlow:
		break;

	case EMonsterStates::Damaged:
		
		break;

	case EMonsterStates::ErrorState:
		break;
	}
	*/
}
/*
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
	for (int i = 0; i < FastProjectilesArray.Num(); i++)
	{
		if (FastProjectilesArray[i]->IsVisible())
		{
			continue;
		}
		else
		{
			FVector ProjectileLocation = FVector(0.0f,
				0.0f, 0.0f);
			FastProjectileLocations[i] = ShipLocation;

			FastProjectilesArray[i]->SetWorldLocation(ProjectileLocation);
			FastProjectilesArray[i]->SetVisibility(true);
			ShotsInUse++;
			return;
		}
	}
}


void AMonster::UpdateProjectiles(float DeltaTime)
{
	for (int i = 0; i < FastProjectilesArray.Num(); i++)
	{
		if (FastProjectilesArray[i]->IsVisible())
		{
			FastProjectilesArray[i]->GetOverlappingComponents(OverlappingComponents);
			if (UpdateOverlappingProjectiles(OverlappingComponents)) //check if its overlapping to destroy it
			{
				FastProjectilesArray[i]->SetVisibility(false);
				FastProjectilesArray[i]->SetWorldLocation(FVector(-100.0f,-100.0f,-100.0f));
				continue;
			}

			FVector NewProjectileLocation = FVector(FastProjectilesArray[i]->GetComponentLocation().X + FastProjectileLocations[i].X*DeltaTime*2,
				FastProjectilesArray[i]->GetComponentLocation().Y + FastProjectileLocations[i].Y * DeltaTime*2, 0.0f);

			FastProjectilesArray[i]->SetRelativeLocation(NewProjectileLocation);
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
*/