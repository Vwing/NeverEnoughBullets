// Fill out your copyright notice in the Description page of Project Settings.

#include "GameJam2016.h"
#include "Projectile.h"
#include "Ship.h"
#include "Monster.h"
#include "Referencer.h"
#include "Background.h"
#include "Bounds.h"
#include "PaperSpriteComponent.h"

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;	
	bHasStarted = false;

	ConstructorHelpers::FObjectFinder<UPaperSprite> ProjectileAsset(TEXT("PaperSprite'/Game/Sprites/Ammo.Ammo'"));
	ProjectileHitBox = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ProjectileHitBox"));
	ProjectileHitBox->SetSprite(ProjectileAsset.Object);
	RootComponent = ProjectileHitBox;

	ProjectileHitBox->bGenerateOverlapEvents = true;
	ProjectileHitBox->bMultiBodyOverlap = true;
	ProjectileHitBox->GetBodyInstance()->bUseCCD = true;
	ProjectileHitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileHitBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	ProjectileHitBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);

	ProjectileSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ProjectileSprite"));
	ProjectileSprite->AttachTo(ProjectileHitBox);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	bHasStarted = false;
	SetActorLocation(FVector(1000.0f, 1000.0f, -1000.0f));
}

// Called every frame
void AProjectile::Tick (float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bHasStarted)
	{
		ProjectileHitBox->GetOverlappingComponents(OverlappingComponents);
		UpdateOverlappingComponents(OverlappingComponents);
		MakeMovements(DeltaTime);
	}	
}


void AProjectile::Start(float x, float y, int8 color, AReferencer* ref)
{
	XSpeed = x;
	YSpeed = y;
	Color = color;
	Ref = ref;
	SetActorLocation(Ref->Monster->GetActorLocation());
	SetActorRotation(Ref->Monster->ProjectileRotation);
	ProjectileHitBox->SetWorldScale3D(Ref->Monster->ProjectileSpriteScale);
	

	if (color == 0)
	{
		ProjectileSprite->SetSprite(Ref->Monster->YellowProjectile);
	}
	else
	{
		ProjectileSprite->SetSprite(Ref->Monster->BlueProjectile);
	}
	ProjectileSprite->SetWorldScale3D(Ref->Monster->ProjectileAnimScale);
	ProjectileSprite->AddWorldOffset(Ref->Monster->ProjectileAnimOffset);
	bHasStarted = true;
}


void AProjectile::MakeMovements(float DeltaTime)
{
	AddActorWorldOffset(FVector(XSpeed*DeltaTime, YSpeed*DeltaTime, 0.0f)*Ref->Ship->SlowMoMultiplier);
}

void AProjectile::UpdateOverlappingComponents(TArray<UPrimitiveComponent*>& OverlappingComponents)
{
	for (int i = 0; i < OverlappingComponents.Num(); i++)
	{
		if (OverlappingComponents[i]->GetName().Contains("ShipHitBox"))
		{
			if (Ref->Ship->bIsDead || Ref->Ship->bIsInvincible)
			{
				continue;
			}

			Ref->Ship->ShotDead();
			Ref->Monster->ShotsInUse -= 1;
			bHasStarted = false;
			Destroy();
			break;
		}
		else if (OverlappingComponents[i]->GetName().Contains("AbsorbSprite"))
		{
			if (Color == 1)
			{
				Ref->Ship->AddAmmo();
				Ref->Monster->CaughtShotSound->Play();
			}
			Ref->Monster->ShotsInUse -= 1;
			Destroy();
			bHasStarted = false;
			break;
		}
		else if(GetActorLocation().X < Ref->Background->LeftWall->GetComponentLocation().X)
		//else if (OverlappingComponents[i]->GetName().Contains("LeftBoundsSprite"))
		{
			Ref->Monster->ShotsInUse -= 1;
			Destroy();
			bHasStarted = false;
			break;
		}
		//else if (OverlappingComponents[i]->GetName().Contains("TopBoundsSprite"))
		else if(GetActorLocation().Y < Ref->Ship->TopInnerBounds->GetComponentLocation().Y - Ref->Ship->TopInnerBounds->Bounds.BoxExtent.Y*2)
		{
			if (YSpeed < 0)
			{
				Ref->Monster->ShotsInUse -= 1;
				Destroy();
				bHasStarted = false;
				break;
			}
		}
		//else if (OverlappingComponents[i]->GetName().Contains("BottomBoundsSprite"))
		else if (GetActorLocation().Y > Ref->Ship->BottomInnerBounds->GetComponentLocation().Y + Ref->Ship->BottomInnerBounds->Bounds.BoxExtent.Y * 2)
		{
			if (YSpeed > 0)
			{
				Ref->Monster->ShotsInUse -= 1;
				Destroy();
				bHasStarted = false;
				break;
			}
		}
		else if (OverlappingComponents[i]->GetName().Contains("RightBoundsSprite"))
		{
			if (XSpeed > 0)
			{
				Ref->Monster->ShotsInUse -= 1;
				Destroy();
				bHasStarted = false;
				break;
			}
		}
	}
}
