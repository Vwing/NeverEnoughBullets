#include "GameJam2016.h"
#include "Monster.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "Referencer.h"
#include "Projectile.h"
#include "Blueprint/UserWidget.h"
#include "Ship.h"

// Sets default values
AMonster::AMonster()
{
	PrimaryActorTick.bCanEverTick = true;
	MaxStraightShots = 20;
	MaxSpreadShots = 20;
	MaxRadialShots = 20;
	MaxShotsUsed = MaxStraightShots + MaxSpreadShots + MaxRadialShots;
	ShotsInUse = 0;
	Health = 50;

	ConstructorHelpers::FObjectFinder<UPaperSprite> BlueProjectileAsset(TEXT("PaperSprite'/Game/Sprites/BulletSprites/MonsterBullets_Sprite4.MonsterBullets_Sprite4'"));
	ConstructorHelpers::FObjectFinder<UPaperSprite> YellowProjectileAsset(TEXT("PaperSprite'/Game/Sprites/BulletSprites/MonsterBullets_Sprite6.MonsterBullets_Sprite6'"));
	BlueProjectile = BlueProjectileAsset.Object;
	YellowProjectile = YellowProjectileAsset.Object;
	
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

	BuildingSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BuildingSprite"));
	ConstructorHelpers::FObjectFinder<UPaperSprite> BuildingSpriteAsset(TEXT("PaperSprite'/Game/Sprites/Building_sprite_Sprite.Building_sprite_Sprite'"));
	BuildingSprite->SetSprite(BuildingSpriteAsset.Object);
	BuildingSprite->AttachTo(RootComponent);
	BuildingSprite->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	BuildingSprite->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BuildingSprite->bGenerateOverlapEvents = true;
	BuildingSprite->SetAbsolute(true,false,true);

	ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> IdleAnimationAsset(TEXT("PaperFlipbook'/Game/Sprites/BossSprites/BossIdle.BossIdle'"));
	IdleAnim = IdleAnimationAsset.Get();

	ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> DamagedAnimationAsset(TEXT("PaperFlipbook'/Game/Sprites/BossSprites/BossDamaged.BossDamaged'"));
	DamagedAnim = DamagedAnimationAsset.Get();

	ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> FastShotAnimationAsset(TEXT("PaperFlipbook'/Game/Sprites/BossSprites/BossQuickShot.BossQuickShot'"));
	FastShotAnim = FastShotAnimationAsset.Get();

	ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> SlowShotAnimationAsset(TEXT("PaperFlipbook'/Game/Sprites/BossSprites/BossSlowSHot.BossSlowShot'"));
	SlowShotAnim = SlowShotAnimationAsset.Get();

	ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> LaughingAnimAsset(TEXT("PaperFlipbook'/Game/Sprites/BossSprites/BossTaunt.BossTaunt'"));
	LaughingAnim = LaughingAnimAsset.Get();

	ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> ExplodingAnimAsset(TEXT("PaperFlipbook'/Game/Sprites/BossSprites/BossExplode.BossExplode'"));
	ExplodingAnim = ExplodingAnimAsset.Get();

	RootComponent = MonsterSprite;
	MonsterFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("MonsterFlipbook"));
	MonsterFlipbook->SetFlipbook(IdleAnim);
	MonsterFlipbook->AttachTo(MonsterSprite);

	DamagedSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DamagedSound"));
	ConstructorHelpers::FObjectFinder<USoundBase> DamagedSoundAsset(TEXT("SoundWave'/Game/SFX/Monster_Hurt.Monster_Hurt'"));
	DamagedSound->SetSound(DamagedSoundAsset.Object);

	DangerSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DangerSound"));
	ConstructorHelpers::FObjectFinder<USoundBase> DangerSoundAsset(TEXT("SoundWave'/Game/SFX/Low_Health_nonRepeat_.Low_Health_nonRepeat_'"));
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

	SpreadProjectilesLocations.Reserve(MaxSpreadShots);
	SpreadProjectilesArray.Reserve(MaxSpreadShots);
	for (int i = 0; i < MaxSpreadShots; i++)
	{
		FVector newLocation = FVector(0.0f, 0.0f, 0.0f);
		int8 color = 0; // 0 == yellow, 1 == blue

		FString ProjectileName = "SpreadProjex" + FString::FromInt(i);
		FName PName = *ProjectileName;

		UPaperSpriteComponent* SpreadProjectile = CreateDefaultSubobject<UPaperSpriteComponent>(PName);
		
		SpreadProjectile->SetSprite(SpreadProjectileAsset.Object);
		SpreadProjectile->CurrentTag = 1;

		SpreadProjectile->AttachTo(RootComponent);

		SpreadProjectile->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		SpreadProjectile->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SpreadProjectile->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		SpreadProjectile->SetVisibility(false);
		SpreadProjectile->SetEnableGravity(false);
		SpreadProjectile->SetSimulatePhysics(false);
		SpreadProjectile->SetAbsolute(true, true, true);

		FString ProjectileAName = "SpreadAProjex" + FString::FromInt(i);
		FName PAName = *ProjectileAName;
		UPaperFlipbookComponent* SpreadProjectileAnim = CreateDefaultSubobject<UPaperFlipbookComponent>(PAName);
		SpreadProjectileAnim->SetFlipbook(YellowProjectileAnimAsset.Get());

		if (i % 2 == 0)
		{
			color = 1;
			SpreadProjectileAnim->SetFlipbook(BlueProjectileAnimAsset.Get());
		}
		SpreadProjectileAnim->AttachTo(SpreadProjectile);
		SpreadProjectileAnim->SetAbsolute(true, true, true);
		SpreadProjectileAnim->SetWorldScale3D(ProjectileAnimScale);
		SpreadProjectileAnim->SetVisibility(false);

		SpreadProjectilesArray.Push(SpreadProjectile);
		SpreadProjectilesLocations.Push(newLocation);
		SpreadProjectilesColor.Push(color);
	}

	ConstructorHelpers::FObjectFinder<UPaperSprite> StraightProjectileAsset(TEXT("PaperSprite'/Game/Sprites/NormalShot.NormalShot'"));
	
	StraightProjectilesArray.Reserve(MaxStraightShots);
	StraightProjectilesLocations.Reserve(MaxStraightShots);
	for (int i = 0; i < MaxStraightShots; i++)
	{
		FVector newLocation = FVector(0.0f, 0.0f, 0.0f);
		int color = 0;

		FString ProjectileName = "StraightProjex" + FString::FromInt(i);
		FName PName = *ProjectileName;

		UPaperSpriteComponent* StraightProjectile = CreateDefaultSubobject<UPaperSpriteComponent>(PName);	

		StraightProjectile->SetSprite(StraightProjectileAsset.Object);
		StraightProjectile->CurrentTag = 1;
		StraightProjectile->AttachTo(RootComponent);

		StraightProjectile->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		StraightProjectile->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		StraightProjectile->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		StraightProjectile->SetVisibility(false);
		StraightProjectile->SetEnableGravity(false);
		StraightProjectile->SetSimulatePhysics(false);
		StraightProjectile->SetAbsolute(true, true, true);

		FString ProjectileAName = "StraightPrAnim" + FString::FromInt(i);
		FName PAName = *ProjectileAName;
		UPaperFlipbookComponent* StraightProjectileAnim = CreateDefaultSubobject<UPaperFlipbookComponent>(PAName);
		StraightProjectileAnim->SetFlipbook(YellowProjectileAnimAsset.Get());

		if (i % 2 == 0)
		{
			color = 1;
			StraightProjectileAnim->SetFlipbook(BlueProjectileAnimAsset.Get());
		}
		StraightProjectileAnim->AttachTo(StraightProjectile);
		StraightProjectileAnim->SetAbsolute(true, true, true);
		StraightProjectileAnim->SetWorldScale3D(ProjectileAnimScale);
		StraightProjectileAnim->SetVisibility(false);

		StraightProjectilesArray.Push(StraightProjectile);
		StraightProjectilesLocations.Push(newLocation);
		StraightProjectilesColor.Push(color);
	}


	ConstructorHelpers::FObjectFinder<UPaperSprite> RadialProjectileAsset(TEXT("PaperSprite'/Game/Sprites/NormalShot.NormalShot'"));
	RadialProjectilesLocations.Reserve(MaxRadialShots);
	RadialProjectilesArray.Reserve(MaxRadialShots);
	for (int i = 0; i < MaxRadialShots; i++)
	{
		FVector newLocation = FVector(0.0f, 0.0f, 0.0f);
		int color = 0;

		FString ProjectileName = "RadialProjex" + FString::FromInt(i);
		FName PName = *ProjectileName;
		NameArray.Add(PName);

		UPaperSpriteComponent* RadialProjectile = CreateDefaultSubobject<UPaperSpriteComponent>(PName);
		RadialProjectile->SetSprite(RadialProjectileAsset.Object);
		RadialProjectile->CurrentTag = 1;
		RadialProjectile->AttachTo(RootComponent);

		RadialProjectile->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		RadialProjectile->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		RadialProjectile->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		RadialProjectile->SetVisibility(false);
		RadialProjectile->SetEnableGravity(false);
		RadialProjectile->SetSimulatePhysics(false);
		RadialProjectile->SetAbsolute(true, true, true);
		
		FString ProjectileAName = "RadialAProjex" + FString::FromInt(i);
		FName PAName = *ProjectileAName;
		UPaperFlipbookComponent* RadialProjectileAnim = CreateDefaultSubobject<UPaperFlipbookComponent>(PAName);
		RadialProjectileAnim->SetFlipbook(YellowProjectileAnimAsset.Get());

		if (i % 2 == 0)
		{
			color = 1;
			RadialProjectileAnim->SetFlipbook(BlueProjectileAnimAsset.Get());
		}
		RadialProjectileAnim->AttachTo(RadialProjectile);
		RadialProjectileAnim->SetAbsolute(true, true, true);
		RadialProjectileAnim->SetWorldScale3D(ProjectileAnimScale);
		RadialProjectileAnim->SetVisibility(false);

		RadialProjectilesArray.Push(RadialProjectile);
		RadialProjectilesLocations.Push(newLocation);
		RadialProjectilesColor.Push(color);
	}

	

	ProjectileRotation = FRotator(90.0f, 0.0f, 0.0f);
	MonsterState = EMonsterStates::Idle;
	ShipLocation = FVector(0.0f, 0.0f, 0.0f);
	ProjectileSpeed = 300.0f;
	ProjectileAnimScale = FVector(5.0f, 5.0f, 5.0f);
	ProjectileSpriteScale = FVector(.750f, .750f, .750f);
	ProjectileAnimOffset = FVector(30.0f, 0.0f, 1.0f);

	EasySlowShotMulti = 1.0f;
	EasyFastShotMulti = 1.75f;
	NormalSlowShotMulti = 1.25f;
	NormalFastShotMulti = 2.5f;
	HardSlowShotMulti = 1.50f;
	HardFastShotMulti = 3.0f;

	VeryEasyShootAgainTime = 1.0f;
	EasyShootAgainTime = .8f;
	NormalShootAgainTime = .7f;
	HardShootAgainTime = .6;

	NumberOfSpreadShots = 5;
	NumberOfRadialShots = 10;

	bIsMoving = false;
	bIsJumping = true;
	bIsFalling = false;
	bIsSlowMo = false;
	bIsShipAlive = true;

	MonsterMovements = FVector(0.0f, -100.0f, 0.0f);
	BuildingLocation = FVector(650.0f, 570.0f, -.01f);
	TopJumpLocation = FVector(650.0f, -450.0f, 0.0f);
	GroundLocation = FVector(GetActorLocation().X, 0.0f , 0.0f);
	
	MaxTimer = 10;
	CurrentTimer = 0;
	SlowMoMultiplier = 1.0f;
	JumpAgainTimer = 3.0f;
	ExplosionAnimationTime = 2;
	DamagedTimer = 0;
	DamagedDuration = .55f;
	ShootAgainDuration = .75f;
	CurrentShots = 0;
	SpreadSize = 75.0f;
	Difficulty = EDifficulty::VeryEasy;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();

	MaxShotsUsed = MaxStraightShots + MaxSpreadShots + MaxRadialShots;

	DamagedSound->Stop();
	DangerSound->Stop();
	IdleSound->Stop();
	RetaliationSound->Stop();
	RoarSound->Stop();
	ExplosionSound->Stop();
	CaughtShotSound->Stop();

	for (int i = 0; i < MaxSpreadShots; i++)
	{
		SetInitialProjectileSettings(SpreadProjectilesArray[i]);
	}

	for (int i = 0; i < MaxStraightShots; i++)
	{
		SetInitialProjectileSettings(StraightProjectilesArray[i]);
	}

	for (int i = 0; i < MaxRadialShots; i++)
	{
		SetInitialProjectileSettings(RadialProjectilesArray[i]);
	}

	bCanShoot = false;
	ShootAgainDuration = VeryEasyShootAgainTime;
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasNull() || MonsterState == EMonsterStates::Closed)
	{
		return;
	}

	//if (ShotsInUse > 0)
//	{
	//	UpdateProjectiles(DeltaTime);
//	}

	CheckIfCanShoot(DeltaTime);
 	if (bCanShoot)
	{
		SetShot();
		bCanShoot = false;
	}
	if (bIsShipAlive)
	{
		UpdateDeathTimer(DeltaTime);
	}

	//UpdateMonsterMovements();
	//MoveMonster(DeltaTime);

	switch (MonsterState)
	{
	case EMonsterStates::Idle:
		DebugString = "Idle";
		MonsterFlipbook->SetFlipbook(IdleAnim);
		break;

	case EMonsterStates::ShootingStraight:
		DebugString = "ShootingStraight";
		MonsterFlipbook->SetFlipbook(FastShotAnim);
		break;

	case EMonsterStates::ShootingSlow:
		DebugString = "ShootingSlow";
		MonsterFlipbook->SetFlipbook(SlowShotAnim);
		break;

	case EMonsterStates::ShootingFast:
		DebugString = "ShootingFast";
		MonsterFlipbook->SetFlipbook(FastShotAnim);
		break;

	case EMonsterStates::ShootingRetaliationShot:
		DebugString = "ShootingRetaliationSho";
		MonsterFlipbook->SetFlipbook(FastShotAnim);
		break;

	case EMonsterStates::Laughing:
		DebugString = "Laughing";
		MonsterFlipbook->SetFlipbook(LaughingAnim);
		break;

	case EMonsterStates::Exploding:
		DebugString = "Exploding";
		MonsterFlipbook->SetFlipbook(ExplodingAnim);
		CheckExplosionFinished(DeltaTime);
		break;

	case EMonsterStates::Damaged:
		DebugString = "Damaged";
		MonsterFlipbook->SetFlipbook(DamagedAnim);
		CheckForRetaliate(DeltaTime);
		break;

	case EMonsterStates::ErrorState:
		DebugString = "ErrorState";
		break;
	
	case EMonsterStates::Closed:
		DebugString = "Closed";
		break;

	default:
		DebugString = "Default";
		break;
	}

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

void AMonster::UpdateProjectiles(float DeltaTime)
{
	for (int i = 0; i < MaxStraightShots; i++)
	{
		if (StraightProjectilesArray[i]->GetChildComponent(0)->IsVisible())
		{
			StraightProjectilesArray[i]->GetOverlappingComponents(OverlappingComponents);
			if (IsProjectileTrajectoryOff(StraightProjectilesLocations[i], StraightProjectilesArray[i],i) ||
				UpdateOverlappingProjectiles(OverlappingComponents,StraightProjectilesColor[i])) //check if its overlapping to destroy it
			{
				StraightProjectilesArray[i]->SetVisibility(false);
				StraightProjectilesArray[i]->SetWorldLocation(FVector(100.0f, 100.0f, -100.0f));
				StraightProjectilesArray[i]->GetChildComponent(0)->SetVisibility(false);
				StraightProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(FVector(100.0f, 100.0f, -100.0f));
				--ShotsInUse;
				continue;
			}

			FVector NewProjectileLocation = FVector(StraightProjectilesArray[i]->GetComponentLocation().X + StraightProjectilesLocations[i].X*DeltaTime*SlowMoMultiplier,
				StraightProjectilesArray[i]->GetComponentLocation().Y + StraightProjectilesLocations[i].Y * DeltaTime*SlowMoMultiplier, 0.0f);

			StraightProjectilesArray[i]->SetWorldLocation(NewProjectileLocation);
			StraightProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(NewProjectileLocation + ProjectileAnimOffset);
		}
	}

	for (int i = 0; i < MaxSpreadShots; i++)
	{
		if (SpreadProjectilesArray[i]->GetChildComponent(0)->IsVisible())
		{
			SpreadProjectilesArray[i]->GetOverlappingComponents(OverlappingComponents);
			if (IsProjectileTrajectoryOff(SpreadProjectilesLocations[i], SpreadProjectilesArray[i], i) ||
				UpdateOverlappingProjectiles(OverlappingComponents,SpreadProjectilesColor[i])) //check if its overlapping to destroy it
			{
				SpreadProjectilesArray[i]->SetVisibility(false);
				SpreadProjectilesArray[i]->SetWorldLocation(FVector(100.0f, 100.0f, -100.0f));

				SpreadProjectilesArray[i]->GetChildComponent(0)->SetVisibility(false);
				SpreadProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(FVector(100.0f, 100.0f, -100.0f));
				--ShotsInUse;
				continue;
			}

			FVector NewProjectileLocation = FVector(SpreadProjectilesArray[i]->GetComponentLocation().X + SpreadProjectilesLocations[i].X*DeltaTime*SlowMoMultiplier,
				SpreadProjectilesArray[i]->GetComponentLocation().Y + SpreadProjectilesLocations[i].Y * DeltaTime*SlowMoMultiplier, 0.0f);
			DebugLocation = SpreadProjectilesArray[i]->GetComponentLocation();
			DebugLocation2 = SpreadProjectilesLocations[i];

			SpreadProjectilesArray[i]->SetWorldLocation(NewProjectileLocation);
			SpreadProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(NewProjectileLocation + ProjectileAnimOffset);
		}
	}

	for (int i = 0; i < MaxRadialShots; i++)
	{
		if (RadialProjectilesArray[i]->GetChildComponent(0)->IsVisible())
		{
			RadialProjectilesArray[i]->GetOverlappingComponents(OverlappingComponents);
			if (IsProjectileTrajectoryOff(RadialProjectilesLocations[i], RadialProjectilesArray[i], i) ||
				UpdateOverlappingProjectiles(OverlappingComponents,SpreadProjectilesColor[i])) //check if its overlapping to destroy it
			{
				RadialProjectilesArray[i]->SetVisibility(false);
				RadialProjectilesArray[i]->SetWorldLocation(FVector(100.0f, 100.0f, -100.0f));

				RadialProjectilesArray[i]->GetChildComponent(0)->SetVisibility(false);
				RadialProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(FVector(100.0f, 100.0f, -100.0f));
				--ShotsInUse;
				continue;
			}

			FVector NewProjectileLocation = FVector(RadialProjectilesArray[i]->GetComponentLocation().X + RadialProjectilesLocations[i].X*DeltaTime*SlowMoMultiplier,
				RadialProjectilesArray[i]->GetComponentLocation().Y + RadialProjectilesLocations[i].Y * DeltaTime*SlowMoMultiplier, 0.0f);

			RadialProjectilesArray[i]->SetWorldLocation(NewProjectileLocation);
			RadialProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(NewProjectileLocation + ProjectileAnimOffset);
		}
	}
}

bool AMonster::UpdateOverlappingProjectiles(TArray<UPrimitiveComponent*>& OverlappingComponents, int color) // 1 == yellow, 2 == blue
{ //might cause crashing when both referencing each other
	for (int i = 0; i < OverlappingComponents.Num(); i++)
	{
		if (OverlappingComponents[i]->GetName().Contains("ShipHitBox"))
		{
			if (Ref->Ship->bIsDead || Ref->Ship->bIsInvincible)
			{
				continue;
			}

			Ref->Ship->ShotDead();
			MonsterState = EMonsterStates::Laughing;

			return true;
		}
		else if (OverlappingComponents[i]->GetName().Contains("AbsorbSprite"))
		{
			if (color == 1)
			{
				Ref->Ship->AddAmmo();
			}
			
			CaughtShotSound->Play();
			CurrentTimer = 0;
			return true;
		}
		else if (OverlappingComponents[i]->GetName().Contains("LeftBoundsSprite"))
		{
			return true;
		}
		else if (OverlappingComponents[i]->GetName().Contains("TopBoundsSprite") && bIsMonsterOnScreen)
		{
			return true;
		}
		else if (OverlappingComponents[i]->GetName().Contains("BottomBoundsSprite"))
		{
			return true;
		}
		else if (OverlappingComponents[i]->GetName().Contains("RightBoundsSprite") && bIsMonsterOnScreen)
		{
			return true;
		}
	}
	return false;
}

void AMonster::UpdateDifficulty()
{
	if (Health >= 40)
	{
		Difficulty = EDifficulty::VeryEasy;
	}
	else if (Health >= 30)
	{
		Difficulty = EDifficulty::Easy;
	}
	else if (Health >= 15)
	{
		Difficulty = EDifficulty::Normal;
	}
	else if (Health >= 0)
	{
		Difficulty = EDifficulty::Hard;
	}
	else
	{
		//UGameplayStatics::OpenLevel(GetWorld(), TEXT("World'/Game/test.test'"));
		Ref->EndGame();
	}
}

FVector AMonster::GetShipLocation()
{
	//FVector TargetDirection = ShipLocation - GetActorLocation();
	FVector TargetDirection = Ref->Ship->GetActorLocation() - GetActorLocation();
	TargetDirection.Normalize(0.01f);
	return TargetDirection*ProjectileSpeed;
}

void AMonster::SetInitialProjectileSettings(UPaperSpriteComponent*& PrimComp)
{
	if (PrimComp == nullptr || PrimComp == NULL)
	{
		return;
	}

	PrimComp->RelativeRotation = ProjectileRotation;
	PrimComp->SetWorldLocation(FVector(200.0f, 300.0f, -100.0f));
	PrimComp->SetWorldScale3D(ProjectileSpriteScale);

	PrimComp->GetChildComponent(0)->RelativeRotation = ProjectileRotation;
	PrimComp->GetChildComponent(0)->SetWorldLocation(FVector(200.0f, 300.0f, -100.0f));
	PrimComp->GetChildComponent(0)->SetWorldScale3D(ProjectileAnimScale);
}

void AMonster::MoveMonster(float DeltaTime)
{
	if (bIsJumping)
	{
		SetActorLocation(GetActorLocation() - MonsterMovements*DeltaTime*SlowMoMultiplier);
		if (GetActorLocation().Y <= TopJumpLocation.Y)
		{
			bIsJumping = false;
			bIsFalling = true;
		}
	}
	else if (bIsFalling)
	{
		SetActorLocation(GetActorLocation() + MonsterMovements*DeltaTime*SlowMoMultiplier);

		if (GetActorLocation().Y >= GroundLocation.Y)
		{
			bIsFalling = false;
			bIsMoving = false;

			GetWorldTimerManager().SetTimer(JumpHandle, this, &AMonster::StartJump, JumpAgainTimer, false);
		}
	}
}

void AMonster::StartJump()
{
	bIsJumping = true;
	bIsMoving = true;
}

void AMonster::UpdateMonsterMovements()
{
	if (Difficulty == EDifficulty::Easy || Difficulty == EDifficulty::VeryEasy)
	{
		MonsterMovements = FVector(0.0f, 200.0f, 0.0f);
		JumpAgainTimer = 3.0f;
	}
	else if (Difficulty == EDifficulty::Normal)
	{
		MonsterMovements = FVector(0.0f, 350.0f, 0.0f);
		JumpAgainTimer = 2.0f;
	}
	else if (Difficulty == EDifficulty::Hard)
	{
		MonsterMovements = FVector(0.0f, 450.0f, 0.0f);
		JumpAgainTimer = 1.0f;
	}
}

void AMonster::UpdateDeathTimer(float DeltaTime)
{
	if (bIsShipAlive == false)
	{
		return;
	}

	CurrentTimer += DeltaTime;
	if (CurrentTimer >= MaxTimer)
	{		
		MonsterState = EMonsterStates::Exploding;
		bIsShipAlive = false;
	}
	else if (CurrentTimer >= MaxTimer*.75 && DangerSound->bIsActive == false)
	{
		DangerSound->Play();
	}
}

void AMonster::CheckExplosionFinished(float DeltaTime)
{
	ExplosionTimer += DeltaTime;
	if (ExplosionTimer >= ExplosionAnimationTime)
	{
		DangerSound->Stop();
		ExplosionSound->Play();

		Ref->Ship->Die();
		MonsterState = EMonsterStates::Laughing;
		ExplosionTimer = 0;
		CurrentTimer = 0; //affects the hud
	}
}

void AMonster::CheckIfCanShoot(float DeltaTime)
{
	if (bCanShoot == true)
	{
		ShootTimer = 0;
		return; //only need to check if is false
	}

	if (bIsShipAlive && ShotsInUse < MaxShotsUsed  && MonsterState != EMonsterStates::Damaged)
	{
		if (ShootTimer >= ShootAgainDuration)
		{
			bCanShoot = true;
			ShootTimer = 0;
		}
		ShootTimer += DeltaTime;
	}
	else
	{
		bCanShoot = false;
		ShootTimer = 0;
	}
}

void AMonster::UpdateShootingTimers()
{
	if (Difficulty == EDifficulty::VeryEasy)
	{
		ShootAgainDuration = VeryEasyShootAgainTime;
	}
	else if (Difficulty == EDifficulty::Easy)
	{
		ShootAgainDuration = EasyShootAgainTime;
	}
	else if (Difficulty == EDifficulty::Normal)
	{
		ShootAgainDuration = NormalShootAgainTime;
	}
	else
	{
		ShootAgainDuration = HardShootAgainTime;
	}
}

void AMonster::SetShot()
{
	if (Difficulty == EDifficulty::VeryEasy)
	{
		VeryEasyShootingPattern();
	}
	else if (Difficulty == EDifficulty::Easy)
	{
		EasyShootingPattern();
	}
	else if (Difficulty == EDifficulty::Normal)
	{
		NormalShootingPattern();
	}
	else if (Difficulty == EDifficulty::Hard)
	{
		HardShootingPattern();
	}
}

void AMonster::ShootFast()
{
	MonsterState = EMonsterStates::ShootingStraight;
	ShootFastProjectile(); //currently testing the add actor projectiles
}


void AMonster::ShootFastProjectile()
{
	FVector WhereToShoot = GetShipLocation();
	int8 PendingShots = 1;

	if (Difficulty == EDifficulty::Easy || Difficulty == EDifficulty::VeryEasy)
	{
		WhereToShoot *= EasyFastShotMulti;
	}
	else if (Difficulty == EDifficulty::Normal)
	{
		WhereToShoot *= NormalFastShotMulti;
		PendingShots = 2;
	}
	else
	{
		WhereToShoot *= HardFastShotMulti;
		PendingShots = 3;
	}

	for (int8 i = 0; ShotsInUse < MaxShotsUsed && i < PendingShots; ++i)
	{
		FVector ShotDirection;
		int8 color = 0;

		if (i % 2 == 0)
		{
			ShotDirection = WhereToShoot + 30.0f*i;
		}
		else
		{
			ShotDirection = WhereToShoot + (30.0f*i)*-1;
		}

		if (ShotsInUse % 5 == 0) { color = 1; }

		AProjectile* NewProjectile = GetWorld()->SpawnActor<AProjectile>(AProjectile::StaticClass());
		NewProjectile->Start(ShotDirection.X, ShotDirection.Y, color, Ref);
		++ShotsInUse;
	}
}

void AMonster::ShootSpreadProjectiles()
{
	FVector WhereToShoot = GetShipLocation();
	int8 PendingShots = 3;

	if (Difficulty == EDifficulty::Easy || Difficulty == EDifficulty::VeryEasy)
	{
		WhereToShoot *= EasySlowShotMulti;
	}
	else if (Difficulty == EDifficulty::Normal)
	{
		WhereToShoot *= NormalSlowShotMulti;
		PendingShots = 5;
	}
	else
	{
		WhereToShoot *= HardSlowShotMulti;
		PendingShots = 7;
	}


	for (int8 i = 0; ShotsInUse < MaxShotsUsed && i < PendingShots; i++)
	{
		FVector ShotDirection = WhereToShoot;
		int8 color = 0;

		if (i % 2 == 0)
		{
			if (i == 0)
			{
				// if its the first shot, shoot right in the middle
			}
			else
			{
				ShotDirection.X += (i * SpreadSize) - SpreadSize;
				ShotDirection.Y -= ((i-1)* SpreadSize);
			}
		}
		else
		{
			ShotDirection.X += (i * SpreadSize);
			ShotDirection.Y += (i*SpreadSize);
		}

		if (ShotsInUse % 5 == 0){ color = 1;}

		AProjectile* NewProjectile = GetWorld()->SpawnActor<AProjectile>(AProjectile::StaticClass());
		NewProjectile->Start(ShotDirection.X, ShotDirection.Y, color, Ref);
		ShotsInUse++;
	}
	
}

void AMonster::ShootRadialProjectiles()
{
	float ShotMulti;
	int8 PendingShots = 5;

	if (Difficulty == EDifficulty::Easy || Difficulty == EDifficulty::VeryEasy)
	{
		ShotMulti = EasySlowShotMulti;
	}
	else if (Difficulty == EDifficulty::Normal)
	{
		ShotMulti = NormalSlowShotMulti;
		PendingShots = 7;
	}
	else
	{
		ShotMulti = HardSlowShotMulti;
		PendingShots = 9;
	}

	for (int8 i = 0; ShotsInUse < MaxShotsUsed && i < PendingShots; i++)
	{
		FVector WhereToShoot = FVector(-1.0f, 0.0f, 0.0f);
		int8 color = 0;

		if (i % 2 == 0) // get the y rotation of the radial shot
		{
			WhereToShoot.Y = (i*.20);
		}
		else
		{
			WhereToShoot.Y = (i*.20)*-1;
		}

		WhereToShoot.Normalize(0.01f);
		WhereToShoot *= ProjectileSpeed * ShotMulti;
		
		if (ShotsInUse % 5 == 0) { color = 1; }
		
		AProjectile* NewProjectile = GetWorld()->SpawnActor<AProjectile>(AProjectile::StaticClass());
		NewProjectile->Start(WhereToShoot.X, WhereToShoot.Y, color, Ref);
		ShotsInUse++;
	}
	
}

void AMonster::ShootRetaliationProjectile()
{
	FVector WhereToShoot = GetShipLocation();
	int8 color = 0;

	if (Difficulty == EDifficulty::Easy || Difficulty == EDifficulty::VeryEasy)
	{
		WhereToShoot *=  EasyFastShotMulti;
	}
	else if (Difficulty == EDifficulty::Normal)
	{
		WhereToShoot *=  NormalFastShotMulti;
	}
	else if (Difficulty == EDifficulty::Hard)
	{
		WhereToShoot *= HardFastShotMulti;
	}

	if (ShotsInUse % 5 == 0) { color = 1; }
	AProjectile* NewProjectile = GetWorld()->SpawnActor<AProjectile>(AProjectile::StaticClass());
	NewProjectile->Start(WhereToShoot.X, WhereToShoot.Y, color, Ref);
	ShotsInUse++;		
}
/*
void AMonster::ShootFastProjectile()
{
	float CurrentShotSpeed;
	int8 PendingShots = 0;
	int8 counter = 0;

	if (Difficulty == EDifficulty::Easy || Difficulty == EDifficulty::VeryEasy)
	{
		CurrentShotSpeed = EasyFastShotMulti;
		PendingShots = 1;
	}
	else if (Difficulty == EDifficulty::Normal)
	{
		CurrentShotSpeed = NormalFastShotMulti;
		PendingShots = 2;
	}
	else
	{
		CurrentShotSpeed = HardFastShotMulti;
		PendingShots = 3;
	}

	FVector WhereToShoot = GetShipLocation() * CurrentShotSpeed;

	for (int i = 0; i < MaxStraightShots && counter < PendingShots; ++i)
	{
		if (StraightProjectilesArray[i]->GetChildComponent(0)->IsVisible())
		{
			continue;
		}
		else
		{
			if (i % 2 == 0)
			{
				StraightProjectilesLocations[i] = WhereToShoot + 20.0f*i;
			}
			else
			{
				StraightProjectilesLocations[i] = WhereToShoot + (20.0f*i)*-1;
			}

			StraightProjectilesArray[i]->SetWorldLocation(GetActorLocation());
			StraightProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(GetActorLocation() + ProjectileAnimOffset);
			StraightProjectilesArray[i]->GetChildComponent(0)->SetVisibility(true);
			++ShotsInUse;
			++counter;
		}
	}
}
*/

void AMonster::ShootSpread()
{
	MonsterState = EMonsterStates::ShootingSlow;
	ShootSpreadProjectiles();
}
/*
void AMonster::ShootSpreadProjectiles()
{
	float CurrentShotSpeed;
	FVector CurrentShipLocation = GetShipLocation();
	int8 counter = 0;
	int8 PendingShots = 0;

	if (Difficulty == EDifficulty::Easy || Difficulty == EDifficulty::VeryEasy)
	{
		CurrentShotSpeed = EasySlowShotMulti;
		PendingShots = 3;
	}
	else if (Difficulty == EDifficulty::Normal)
	{
		CurrentShotSpeed = NormalSlowShotMulti;
		PendingShots = 5;
	}
	else 
	{
		CurrentShotSpeed = HardSlowShotMulti;
		PendingShots = 7;
	}
	
	
	for (int i = 0; i < MaxSpreadShots && counter < PendingShots; i++)
	{
		if (SpreadProjectilesArray[i]->GetChildComponent(0)->IsVisible())
		{
			continue;
		}
		else
		{
			FVector NewShipLocation;

			if (counter % 2 == 0)
			{
				if (counter == 0)
				{
					NewShipLocation = (CurrentShipLocation * CurrentShotSpeed); // if its the first shot, shoot right in the middle
				}
				else
				{
					NewShipLocation = (CurrentShipLocation * CurrentShotSpeed) + (counter * 40) - 40;
				}
			}
			else
			{
				NewShipLocation = (CurrentShipLocation * CurrentShotSpeed) + (counter * 40 * -1);
			}

			SpreadProjectilesLocations[i] = NewShipLocation;
			SpreadProjectilesArray[i]->SetWorldLocation(GetActorLocation());
			SpreadProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(GetActorLocation() + ProjectileAnimOffset);
			SpreadProjectilesArray[i]->GetChildComponent(0)->SetVisibility(true);
			ShotsInUse++;
			counter++;
		}
	}
}
*/

void AMonster::ShootRadial()
{
	MonsterState = EMonsterStates::ShootingFast;
	ShootRadialProjectiles();
}

/*
void AMonster::ShootRadialProjectiles()
{
	float CurrentShotSpeed;
	int8 counter = 0;
	int8 PendingShots = 0;

	if (Difficulty == EDifficulty::Easy || Difficulty == EDifficulty::VeryEasy)
	{
		CurrentShotSpeed = EasyFastShotMulti;
		PendingShots = 5;
	}
	else if (Difficulty == EDifficulty::Normal)
	{
		CurrentShotSpeed = NormalFastShotMulti;
		PendingShots = 7;
	}
	else
	{
		CurrentShotSpeed = HardFastShotMulti;
		PendingShots = 9;
	}

	for (int i = 0; i < MaxRadialShots && counter < PendingShots; i++)
	{
		if (RadialProjectilesArray[i]->GetChildComponent(0)->IsVisible())
		{
			continue;
		}
		else
		{
			FVector NewShipLocation = FVector(-1.0f, 0.0f, 0.0f);

			if (counter % 2 == 0) // get the y rotation of the radial shot
			{
				NewShipLocation.Y = (counter*.20);
			}
			else
			{
				NewShipLocation.Y = (counter*.20)*-1;
			}

			NewShipLocation.Normalize(0.01f);
			NewShipLocation *= ProjectileSpeed * CurrentShotSpeed;

			RadialProjectilesLocations[i] = NewShipLocation; //where the projcetile is aiming toward but actually the speed
			RadialProjectilesArray[i]->SetWorldLocation(GetActorLocation());
			RadialProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(GetActorLocation() + ProjectileAnimOffset);
			RadialProjectilesArray[i]->GetChildComponent(0)->SetVisibility(true);
			ShotsInUse++;
			counter++;
		}
	}
}
*/
void AMonster::TookDamage()
{
	Health -= 1;
	if (MonsterState != EMonsterStates::Damaged &&
		MonsterState != EMonsterStates::ShootingRetaliationShot &&
		MonsterState != EMonsterStates::Exploding)
	{
		MonsterState = EMonsterStates::Damaged;
		DamagedSound->Play();
		UpdateDifficulty();
		UpdateShootingTimers();
	}
}

void AMonster::CheckForRetaliate(float DeltaTime)
{

	if (DamagedTimer >= DamagedDuration)
	{
		MonsterState = EMonsterStates::ShootingRetaliationShot;
		ShootRetaliationProjectile();
		RetaliationSound->Play();
		DamagedTimer = 0;
	}
	else
	{
		DamagedTimer += DeltaTime;
	}
	
}

/*
void AMonster::ShootRetaliationProjectile()
{
	for (int i = 0; i < MaxStraightShots; i++)
	{
		if (StraightProjectilesArray[i]->GetChildComponent(0)->IsVisible())
		{
			continue;
		}
		else
		{
			FVector NewShipLocation = GetShipLocation();
			NewShipLocation.Normalize(0.01f);

			if (Difficulty == EDifficulty::Easy || Difficulty == EDifficulty::VeryEasy)
			{
				StraightProjectilesLocations[i] = ProjectileSpeed * NewShipLocation * EasyFastShotMulti;
			}
			else if (Difficulty == EDifficulty::Normal)
			{
				StraightProjectilesLocations[i] = ProjectileSpeed * NewShipLocation* NormalFastShotMulti;
			}
			else if (Difficulty == EDifficulty::Hard)
			{
				StraightProjectilesLocations[i] = ProjectileSpeed * NewShipLocation* HardFastShotMulti;
			}

			StraightProjectilesArray[i]->SetWorldLocation(GetActorLocation());
			StraightProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(GetActorLocation() + ProjectileAnimOffset);
			StraightProjectilesArray[i]->GetChildComponent(0)->SetVisibility(true);
			ShotsInUse++;
			return;
		}
	}
}
*/

void AMonster::VeryEasyShootingPattern()
{
	if (CurrentShots == 0)
	{
		ShootFast();
		++CurrentShots;
	}
	else
	{
		ShootRadial();
		CurrentShots = 0;
	}
	
}

void AMonster::EasyShootingPattern()
{
	if (CurrentShots == 0)
	{
		ShootFast();
		++CurrentShots;
	}
	else if (CurrentShots == 1)
	{
		ShootRadial();
		++CurrentShots;
	}
	else if (CurrentShots == 2)
	{
		ShootFast();
		++CurrentShots;
	}
	else
	{
		ShootSpread();
		CurrentShots = 0;
	}
}

void AMonster::NormalShootingPattern()
{
	if (CurrentShots == 0)
	{
		ShootFast();
		++CurrentShots;
	}
	else if (CurrentShots == 1)
	{
		ShootRadial();
		++CurrentShots;
	}
	else if(CurrentShots == 2)
	{
		ShootFast();
		++CurrentShots;
	}
	else if (CurrentShots == 3)
	{
		ShootSpread();
		CurrentShots = 0;
	}
}

void AMonster::HardShootingPattern()
{
	if (CurrentShots == 0)
	{
		ShootFast();
		++CurrentShots;
	}
	else if (CurrentShots == 1)
	{
		ShootRadial();
		++CurrentShots;
	}
	else if(CurrentShots == 2)
	{
		ShootFast();
		++CurrentShots;
	}
	else
	{
		ShootSpread();
		CurrentShots = 0;
	}
}


void AMonster::Scroll(FVector MoveVect)
{
	AddActorWorldOffset(MoveVect);
	BuildingSprite->AddRelativeLocation(MoveVect);
	ScrollProjectiles(MoveVect);
}

void AMonster::ScrollProjectiles(FVector MoveVect)
{
	for (int i = 0; i < MaxStraightShots; i++)
	{
		if (StraightProjectilesArray[i]->GetChildComponent(0)->IsVisible())
		{
			FVector NewProjectileLocation = StraightProjectilesArray[i]->GetComponentLocation() + MoveVect;
			StraightProjectilesArray[i]->SetWorldLocation(NewProjectileLocation);
			StraightProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(NewProjectileLocation + ProjectileAnimOffset);
		}
	}

	for (int i = 0; i < MaxSpreadShots; i++)
	{
		if (SpreadProjectilesArray[i]->GetChildComponent(0)->IsVisible())
		{
			FVector NewProjectileLocation = SpreadProjectilesArray[i]->GetComponentLocation() + MoveVect;
			SpreadProjectilesArray[i]->SetWorldLocation(NewProjectileLocation);
			SpreadProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(NewProjectileLocation + ProjectileAnimOffset);
		}
	}

	for (int i = 0; i < MaxRadialShots; i++)
	{
		if (RadialProjectilesArray[i]->GetChildComponent(0)->IsVisible())
		{
			FVector NewProjectileLocation = RadialProjectilesArray[i]->GetComponentLocation() + MoveVect;
			RadialProjectilesArray[i]->SetWorldLocation(NewProjectileLocation);
			RadialProjectilesArray[i]->GetChildComponent(0)->SetWorldLocation(NewProjectileLocation + ProjectileAnimOffset);
		}
	}
}

bool AMonster::HasNull()
{
	if (Ref == NULL ||
		Ref == nullptr)
	{
		MonsterState = EMonsterStates::ErrorState;
		NullString = "Ref Is Null";
		return true;
	}
	else if (Ref->HasNullActors())
	{
		MonsterState = EMonsterStates::ErrorState;
		NullString = "Ref Has Null";
		return true;
	}
	else
	{
		return false;
	}
}

void AMonster::SetClosedState()
{
	MonsterState = EMonsterStates::Closed;
}


bool AMonster::IsProjectileTrajectoryOff(FVector& Location, UPaperSpriteComponent*& Projectile, int index)
{
	// if the projectile is above the bounds and is moving up, "delete" it
	if (Projectile->GetComponentLocation().Y < (Ref->Ship->TopInnerBounds->GetComponentLocation().Y - Ref->Ship->TopInnerBounds->Bounds.BoxExtent.Y*2) &&
		Location.Y < 0)
	{
		return true;
	}
	else if (Projectile->GetComponentLocation().Y > (Ref->Ship->BottomInnerBounds->GetComponentLocation().Y  + Ref->Ship->TopInnerBounds->Bounds.BoxExtent.Y * 2) &&
		Location.Y > 0)
	{
		return true;
	}
	return false;
}
