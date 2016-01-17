// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Monster.generated.h"

UCLASS()
class GAMEJAM2016_API AMonster : public AActor
{
	GENERATED_BODY()
	
public:	

	AMonster();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;


	UPROPERTY(EditAnywhere, Category = "Stats")
		float Health;
	UPROPERTY(EditAnywhere, Category = "Stats")
		FVector ShipLocation;
	UPROPERTY(EditAnywhere, Category = "Stats")
		TArray<UPrimitiveComponent*> OverlappingComponents;

//	UPROPERTY(EditAnywhere, Category = "Projectiles")
	//	TArray<class UPaperSpriteComponent*>  SinProjectilesArray;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		TArray<class UPaperSpriteComponent*>  StraightProjectilesArray;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		FRotator ProjectileRotation;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		int8 ShotsInUse;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		int8 MaxShotsUsed;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		TArray<FVector> FastProjectileLocations;

	UPROPERTY(EditAnywhere, Category = "Anim")
	class UPaperFlipbookComponent* MonsterFlipbook;
	UPROPERTY(EditAnywhere,Category="Anim")
	class UPaperFlipbook* IdleAnim;
	UPROPERTY(EditAnywhere, Category = "Anim")
	class UPaperFlipbook* DamagedAnim;
	UPROPERTY(EditAnywhere, Category = "Anim")
	class UPaperFlipbook* FastShotAnim;

	class UPaperSpriteComponent* MonsterSprite;

	UPROPERTY(EditAnywhere, Category = "Sound")
	class UAudioComponent* DamagedSound;

	UPROPERTY(EditAnywhere, Category = "Debug")
		FString DebugString;
	UPROPERTY(EditAnywhere, Category = "Debug")
		FVector DebugLocation;
	UPROPERTY(EditAnywhere, Category = "Debug")
		FVector DebugLocation2;

	struct EMonsterStates
	{
		enum Type
		{
			Idle,
			ShootingFast,
			ShootingSlow,
			Damaged,
			ErrorState,
			Closed
		};
	};

	EMonsterStates::Type MonsterState;
	
	void ShootFastProjectile();
	void UpdateProjectiles(float DeltaTime);
	bool UpdateOverlappingProjectiles(TArray<UPrimitiveComponent*>& OverlappingComponents);

	void SetDamagedState();
	void SetDamagedAnim();
	void SetIdleAnim();
	void SetIdleState();
	void SetShootingFastState();
	void SetFastShotAnim();
	FTimerHandle MonsterHandle;
};
