// Fill out your copyright notice in the Description page of Project Settings.

#include "GameJam2016.h"
#include "Projectile.h"
#include "PaperSpriteComponent.h"

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	//ProjectileSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ProjectileSprite"));
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectile::Tick (float DeltaTime )
{
	Super::Tick( DeltaTime );
/*
	ProjectileSprite->GetOverlappingComponents(OverlappingComponents);
	UpdateOverlappingComponents(OverlappingComponents);

	switch (ProjectileState)
	{
	case EProjectileStates::PlayerShot:
		MakeMovements(DeltaTime);
		break;

	case EProjectileStates::MonsterShotStraight:
		break;

	case EProjectileStates::MonsterShotSin:
		break;
	}
	*/
}
/*
void AProjectile::MakeMovements(float DeltaTime)
{
	YSpeed *= DeltaTime;
	XSpeed *= DeltaTime;
	SetActorLocation(GetActorLocation() + FVector(XSpeed, YSpeed, 0.0f));
}

void AProjectile::SetPlayerShot(int XBulletSpeed, int YBulletSpeed, FVector ProjectileLocation)
{
	ProjectileSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PlayerShot"));
	ConstructorHelpers::FObjectFinder<UPaperSprite> ProjectileSpriteAsset(TEXT("PaperSprite'/Game/Sprites/NormalShot.NormalShot'"));
	ProjectileSprite->SetSprite(ProjectileSpriteAsset.Object);

	XSpeed = XBulletSpeed;
	YSpeed = YBulletSpeed;

	ProjectileState = EProjectileStates::PlayerShot;
	SetActorLocation(ProjectileLocation);
}

void AProjectile::UpdateOverlappingComponents(TArray<UPrimitiveComponent*>& OverlappingComponents)
{
	for (int i = 0; i < OverlappingComponents.Num(); ++i)
	{
		if (OverlappingComponents[i]->GetName().Contains("RightBoundsSprite"))
		{
			Destroy();
		}
	}
}
*/