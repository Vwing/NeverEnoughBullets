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
	virtual void Tick(float DeltaSeconds) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float Health;
	UPROPERTY(EditAnywhere, Category = "Stats")
		FVector ShipLocation;
	UPROPERTY(VisibleAnywhere, Category = "Stats")
		TArray<UPrimitiveComponent*> OverlappingComponents;

	//	UPROPERTY(EditAnywhere, Category = "Projectiles")
	//	TArray<class UPaperSpriteComponent*>  SinProjectilesArray;
	UPROPERTY(VisibleAnywhere, Category = "Projectiles")
		TArray<class UPaperSpriteComponent*>  StraightProjectilesArray;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		FRotator ProjectileRotation;
	UPROPERTY(VisibleAnywhere, Category = "Projectiles")
		int8 ShotsInUse;
	UPROPERTY(VisibleAnywhere, Category = "Projectiles")
		int8 MaxShotsUsed;
	UPROPERTY(VisibleAnywhere, Category = "Projectiles")
		TArray<FVector> StraightProjectilesLocations;
	

	UPROPERTY(EditAnywhere, Category = "Anim")
	class UPaperFlipbookComponent* MonsterFlipbook;
	UPROPERTY(EditAnywhere, Category = "Anim")
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
			ShootingStraight,
			ShootingSlow,
			ShootingRetaliationShot,
			Damaged,
			ErrorState,
			Closed
		};
	};

	EMonsterStates::Type MonsterState;

	struct EDifficulty
	{
		enum Type
		{
			Easy,
			Normal,
			Hard
		};
	};
	EDifficulty::Type Difficulty;

	UPROPERTY(EditAnywhere, Category="Projectiles")
	float MinShootTime;
	UPROPERTY(EditAnywhere, Category="Projectiles")
	float MaxShootTime;

	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float EasyMinShotMultiplier;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float EasyMaxShotMultiplier;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float NormalMinShotMultiplier;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float NormalMaxShotMultiplier;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float HardMinShotMultiplier;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float HardMaxShotMultiplier;

	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float EasyMinShootAgainTime;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float EasyMaxShootAgainTime;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float NormalMinShootAgainTime;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float NormalMaxShootAgainTime;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float HardMinShootAgainTime;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float HardMaxShootAgainTime;

	bool bCanShoot;

	void SetCanShoot();

	void ShootFastProjectile();
	void UpdateProjectiles(float DeltaTime);
	bool UpdateOverlappingProjectiles(TArray<UPrimitiveComponent*>& OverlappingComponents);

	void SetDamagedState();
	void SetDamagedAnim();

	void SetRandomShot();

	void SetIdleAnim();
	void SetIdleState();

	void SetShootingStraightState();
	void SetShootingRetaliationState();
	void SetShootingRetaliationAnim();
	void ShootRetaliationProjectile();

	void SetFastShotAnim();
	void UpdateDifficulty();

	float GetShootAgainTime();

	FTimerHandle MonsterAnimHandle;
	FTimerHandle ShootAgainHandle;
};
