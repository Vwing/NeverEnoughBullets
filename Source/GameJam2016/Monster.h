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
		int32 Health;
	UPROPERTY(EditAnywhere, Category = "Stats")
		FVector ShipLocation;
	UPROPERTY(VisibleAnywhere, Category = "Stats")
		TArray<UPrimitiveComponent*> OverlappingComponents;

	UPROPERTY(EditAnywhere, Category = "Projectiles")
	class UPaperSprite* BlueProjectile;

	UPROPERTY(EditAnywhere, Category = "Projectiles")
	class UPaperSprite* YellowProjectile;


	UPROPERTY(EditAnywhere, Category = "Projectiles")
		TArray<class UPaperSpriteComponent*>  SpreadProjectilesArray;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		TArray<class UPaperSpriteComponent*>  StraightProjectilesArray;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		TArray<class UPaperSpriteComponent*>  RadialProjectilesArray;

	UPROPERTY(VisibleAnywhere, Category = "Projectiles")
		TArray<FVector> StraightProjectilesLocations;
	UPROPERTY(VisibleAnywhere, Category = "Projectiles")
		TArray<FVector> SpreadProjectilesLocations;
	UPROPERTY(VisibleAnywhere, Category = "Projectiles")
		TArray<FVector> RadialProjectilesLocations;
	UPROPERTY(VisibleAnywhere, Category = "Projectiles")
		TArray<int8> StraightProjectilesColor;
	UPROPERTY(VisibleAnywhere, Category = "Projectiles")
		TArray<int8> SpreadProjectilesColor;
	UPROPERTY(VisibleAnywhere, Category = "Projectiles")
		TArray<int8> RadialProjectilesColor;

	UPROPERTY(EditAnywhere, Category = "Anim")
	class UPaperFlipbookComponent* MonsterFlipbook;
	UPROPERTY(EditAnywhere, Category = "Anim")
	class UPaperFlipbook* IdleAnim;
	UPROPERTY(EditAnywhere, Category = "Anim")
	class UPaperFlipbook* DamagedAnim;
	UPROPERTY(EditAnywhere, Category = "Anim")
	class UPaperFlipbook* FastShotAnim;
	UPROPERTY(EditAnywhere, Category = "Anim")
	class UPaperFlipbook* SlowShotAnim;
	UPROPERTY(EditAnywhere, Category = "Anim")
	class UPaperFlipbook* LaughingAnim;
	UPROPERTY(EditAnywhere, Category = "Anim")
	class UPaperFlipbook* ExplodingAnim;

	class UPaperSpriteComponent* MonsterSprite;

	UPROPERTY(EditAnywhere, Category = "Anim")
	class UPaperSpriteComponent* BuildingSprite;

	UPROPERTY(EditAnywhere, Category = "Sound")
	class UAudioComponent* DamagedSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	class UAudioComponent* DangerSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	class UAudioComponent* IdleSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	class UAudioComponent* RetaliationSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	class UAudioComponent* RoarSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	class UAudioComponent* ExplosionSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	class UAudioComponent* CaughtShotSound;


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
			ShootingFast,
			ShootingSlow,
			ShootingRetaliationShot,
			Laughing,
			Exploding,
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
			VeryEasy,
			Easy,
			Normal,
			Hard
		};
	};
	EDifficulty::Type Difficulty;
	
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float ProjectileSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Projectiles")
		int32 NumberOfSpreadShots;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectiles")
		int32 NumberOfRadialShots;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float EasyFastShotMulti;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float EasySlowShotMulti;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float NormalFastShotMulti;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float NormalSlowShotMulti;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float HardFastShotMulti;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float HardSlowShotMulti;

	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float EasyShootAgainTime;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float NormalShootAgainTime;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float HardShootAgainTime;

	UPROPERTY(EditAnywhere, Category = "Projectiles")
		FRotator ProjectileRotation;
	UPROPERTY(VisibleAnywhere, Category = "Projectiles")
		int8 ShotsInUse;
	UPROPERTY(VisibleAnywhere, Category = "Projectiles")
		int8 MaxShotsUsed;

	UPROPERTY(EditAnywhere, Category = "Projectiles")
		int8 MaxStraightShots;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		int8 MaxSpreadShots;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		int8 MaxRadialShots;

	UPROPERTY(EditAnywhere, Category = "Projectiles")
	bool bCanShoot;

	bool bIsSlowMo;
	bool bIsMoving;
	bool bIsJumping;
	bool bIsFalling;

	UPROPERTY(EditAnywhere, Category = "Movement")
		bool bIsMonsterOnScreen;

	FVector MonsterMovements;

	float SlowMoMultiplier;
	void MoveMonster(float DeltaTime);
	void SetInitialProjectileSettings(UPaperSpriteComponent*& PrimComp);
	
	void UpdateProjectiles(float DeltaTime);
	bool UpdateOverlappingProjectiles(TArray<UPrimitiveComponent*>& OverlappingComponents, int color);

	void SetShot();
	void TookDamage();
	void SetIdleAnim();
	void SetIdleState();

	void CheckForRetaliate(float DeltaTime);
	void ShootRetaliationProjectile();

	void ShootFastProjectile();
	void UpdateDifficulty();
	
	void ShootSpread();
	void ShootSpreadProjectiles();

	void ShootRadial();
	void ShootRadialProjectiles();

	FVector GetShipLocation();

	FTimerHandle MonsterAnimHandle;
	FTimerHandle JumpHandle;
	FTimerHandle DeathTimer;
	
	UPROPERTY(EditAnywhere, Category = "DeathTimer")
	float CurrentTimer;
	UPROPERTY(EditAnywhere, Category = "DeathTimer")
	float MaxTimer;

	UPROPERTY(EditAnywhere, Category = "Stats")
	bool bIsShipAlive;

	void UpdateDeathTimer(float DeltaTime);
	
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		FVector ProjectileAnimOffset;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		FVector ProjectileAnimScale;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		FVector ProjectileSpriteScale;
	UPROPERTY(EditAnyWhere, Category = "Projectiles")
		float SpreadSize;

	void UpdateMonsterMovements();
	UPROPERTY(EditAnywhere, Category = "Stats")
	FVector BuildingLocation;
	FVector TopJumpLocation;

	float JumpAgainTimer;

	void StartJump();
	FVector GroundLocation;

	UPROPERTY(EditAnywhere, Category = "Referencer")
	class AReferencer* Ref;
	//void UpdateMonsterOverlaps(TArray<UPrimitiveComponent*> OverlappingComponents);
	void CheckExplosionFinished(float DeltaTime);
	UPROPERTY(EditAnywhere, Category="Death")
	float ExplosionAnimationTime;
	float ExplosionTimer;

	void CheckIfCanShoot(float DeltaTime);
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float ShootTimer;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float ShootAgainDuration;

	void UpdateShootingTimers();
	void ShootFast();

	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float DamagedDuration;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float DamagedTimer;

	void EasyShootingPattern();
	void VeryEasyShootingPattern();
	void NormalShootingPattern();
	void HardShootingPattern();

	UPROPERTY(EditAnywhere, Category = "Projectiles")
		int8 CurrentShots;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float VeryEasyShootAgainTime;

	void Scroll(FVector MoveVect);
	void ScrollProjectiles(FVector MoveVect);

	bool HasNull();
	UPROPERTY(EditAnywhere, Category = "Debug")
		FString NullString;

	bool IsProjectileTrajectoryOff(FVector& Location, class UPaperSpriteComponent*& Projectile, int index);
	void SetClosedState();
	UPROPERTY(EditAnywhere, Category = "Debug")
		TArray<FName> NameArray;

};
