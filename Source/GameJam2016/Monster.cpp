// Fill out your copyright notice in the Description page of Project Settings.

#include "GameJam2016.h"
#include "Monster.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"

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

	RootComponent = MonsterSprite;
	MonsterFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("MonsterFlipbook"));
	MonsterFlipbook->SetFlipbook(IdleAnim);
	MonsterFlipbook->AttachTo(MonsterSprite);

	DamagedSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DamagedSound"));
	ConstructorHelpers::FObjectFinder<USoundBase> DamagedSoundAsset(TEXT("SoundWave'/Game/SFX/Death.Death'"));
	DamagedSound->SetSound(DamagedSoundAsset.Object);

	
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
	}

	Health = 50;
	ProjectileRotation = FRotator(90.0f, 90.0f, 0.0f);
	MonsterState = EMonsterStates::Idle;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	DamagedSound->Stop();

	for (int i = 0; i < SlowProjectilesArray.Num(); i++)
	{
		SlowProjectilesArray[i]->RelativeRotation = ProjectileRotation;
		SlowProjectilesArray[i]->SetWorldLocation(FVector(0.0f, 0.0f, -100.0f));
	}

}

// Called every frame
void AMonster::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

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

	GetWorldTimerManager().SetTimer(MonsterHandle, this, &AMonster::SetIdleState, 0.50f, false);
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