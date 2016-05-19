// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Ship.generated.h"

UCLASS()
class GAMEJAM2016_API AShip : public APawn
{
	GENERATED_BODY()

public:
	AShip();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void Fire();
	void StopFire();
	void MoveUp(float AxisValue);

	void MoveRight(float AxisValue);
	void Absorb();

	void MakeMovements(float DeltaTime);
	void Move();
	void Scroll();
	
	void AddAmmo();

	void UpdateScrollingRules();
	void ShootProjectile();
	void UpdateProjectiles(float DeltaTime);
	void UpdateMonster(float DeltaTime);
	void UpdateOverlappingComponents(TArray<UPrimitiveComponent*>& OverlappingComponents);
	bool UpdateOverlappingProjectiles(TArray<UPrimitiveComponent*>& OverlappingComponents);

	UPROPERTY(EditAnywhere, Category = "Stats")
	class AReferencer* Ref;

	UPROPERTY(EditAnywhere, Category = "Stats")
		float MaxVerticalSpeed;
	UPROPERTY(EditAnywhere, Category = "Stats")
		float MaxHorizontalSpeed;
	UPROPERTY(EditAnywhere, Category = "Stats")
		float CurrentHorizontalSpeed;
	UPROPERTY(EditAnywhere, Category = "Stats")
		float CurrentVerticalSpeed;

	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float ProjectileSpeed;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		int8 MaxShotsUsed;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		int8 ShotsInUse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectiles")
		int32 Ammo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectiles")
		int32 MaxAmmo;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		TArray<class UPaperSpriteComponent*>  ProjectilesArray;

	UPROPERTY(EditAnywhere, Category = "Projectiles")
		FRotator ProjectileRotation;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		bool bCanShoot;

	int32 Deaths;
	UPROPERTY(EditAnywhere, Category = "DeathTimer")
		float DeathTime;
	UPROPERTY(EditAnywhere, Category = "DeathTimer")
		float MaxDeathTime;

	struct EShipStates
	{
		enum Type
		{
			Idle,
			MoveUp,
			MoveDown,
			MoveForward,
			Absorb,
			Shooting,
			Damaged,
			Exploding,
			Dead,
			Invincible,
			Sliced,
			ErrorState,
			Closed
		};
	};

	EShipStates::Type ShipState;

	UPROPERTY(EditAnywhere, Category = "Anim")
	class UPaperSpriteComponent* ShipHitBox;

	// flipbook components
	UPROPERTY(EditAnywhere, Category = "Anim")
	class UPaperFlipbookComponent* ShipFlipbook;
	UPROPERTY(EditAnywhere, Category = "Anim")
	class UPaperFlipbook* ShipIdleAnim;
	UPROPERTY(EditAnywhere, Category = "Anim")
	class UPaperFlipbook* ShipMoveForwardAnim;
	UPROPERTY(EditAnywhere, Category = "Anim")
	class UPaperFlipbook* ShipMoveDownAnim;
	UPROPERTY(EditAnywhere, Category = "Anim")
	class UPaperFlipbook* ShipMoveUpAnim;
	UPROPERTY(EditAnywhere, Category = "Anim")
	class UPaperFlipbook* ShipBlueAnim;
	UPROPERTY(EditAnywhere, Category = "Anim")
	class UPaperFlipbook* ExplosionAnim;
	UPROPERTY(EditAnywhere, Category = "Anim")
	class UPaperFlipbook* InvincibleAnim;

	// absorb components
	UPROPERTY(EditAnywhere, Category = "Anim")
	class UPaperSpriteComponent* AbsorbSprite;
	
	// Arrow Components
	UPROPERTY(EditAnywhere, Category = "Arrow")
	class UPaperFlipbookComponent* TopArrowAnim;
	UPROPERTY(EditAnywhere, Category = "Arrow")
	class UPaperFlipbookComponent* BottomArrowAnim;
	UPROPERTY(EditAnywhere, Category = "Arrow")
		FVector ArrowAnimScale;
	


	// audio components
	UPROPERTY(EditAnywhere, Category = "Audio")
	class UAudioComponent* AbsorbSound;
	UPROPERTY(EditAnywhere, Category = "Audio")
	class UAudioComponent* ShootingSound;
	UPROPERTY(EditAnywhere, Category = "Audio")
	class UAudioComponent* SliceSound;
	UPROPERTY(EditAnywhere, Category = "Audio")
	class UAudioComponent* BlasterSound;
	

	UPROPERTY(EditAnywhere, Category = "Stats")
	class UPaperSpriteComponent* TopInnerBounds;
	UPROPERTY(EditAnywhere, Category = "Stats")
	class UPaperSpriteComponent* BottomInnerBounds;


	UPROPERTY(EditAnywhere, Category = "Debug")
		TArray<UPrimitiveComponent*> OverlappingComponents;

	UPROPERTY(EditAnywhere, Category = "Debug")
		FString DebugString;
	UPROPERTY(EditAnywhere, Category = "Debug")
		FVector DebugLocation;

	bool bCanMoveLeft;
	bool bCanMoveRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		bool bIsDead;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		bool bIsExploding;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		bool bIsInvincible;

	FTimerHandle ShipHandle;
	FTimerHandle UpdateMonsterHandle;
	FTimerHandle ArrowHandle;
	FTimerHandle SlowMoHandle;

	void TurnSlowMoOn();
	void TurnSlowMoOff();
	UPROPERTY(EditAnywhere, Category="Stats")
	float SlowMoDuration;
	void SlowMo();
	void StopSlowMo();

	UPROPERTY(EditAnywhere, Category = "Projectiles")
		FVector ProjectileAnimOffset;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		FVector ProjectileAnimScale;

	
	UPROPERTY(EditAnywhere, Category = "Stats")
		FVector AbsorbSpriteScale;
	UPROPERTY(EditAnywhere, Category = "Stats")
		FRotator InnerBoundsRotation;
	UPROPERTY(EditAnywhere, Category = "Stats")
		FVector ShipSpriteScale;
	UPROPERTY(EditAnywhere, Category = "Stats")
		FVector ShipFlipbookScale;
	UPROPERTY(EditAnywhere, Category = "Stats")
		FVector ShipFlipbookOffset;
	UPROPERTY(EditAnywhere, Category = "Stats")
		FVector InnerBoundsScale;

	bool bCanSlice;
	bool bIsSlowMo;
	bool bCanSlowMo;
	float SlowMoMultiplier;
	bool bGameHasStarted;
	bool bAreArrowsOn;
	void SetAnim(UPaperFlipbook* AnimAsset);
	void UpdateArrows();
	void Die();
	
	void TurnArrowsOff();
	UPROPERTY(EditAnywhere,Category = "Absorb")
	float AbsorbAnimXOffset;

	void CheckForRevive(float DeltaTime);
	void ReviveShip();
	void CheckFinishedExploding(float DeltaTime);
	void ShotDead();
	void UpdateInvincibility(float DeltaTime);
	UPROPERTY(EditAnywhere, Category = "Stats")
		float MaxInvincibleTime;

	void UpdateOtherStates(float DeltaTime);
	UPROPERTY(EditAnywhere, Category = "Absorb")
		float AbsorbAgainTimer;
	UPROPERTY(EditAnywhere, Category = "Absorb")
		float AbsorbDuration;
	UPROPERTY(EditAnywhere, Category = "Absorb")
		float AbsorbTimer;
	UPROPERTY(EditAnywhere, Category = "Absorb")
		bool bIsAbsorbing;
	void UpdateSlice();
	void UpdateAbsorb(float DeltaTime);
	
	UPROPERTY(EditAnywhere, Category = "Movements")
		bool bCanScrollLeft;
	UPROPERTY(EditAnywhere, Category = "Movements")
		bool bCanScrollRight;
	UPROPERTY(EditAnywhere, Category = "Movements")
		bool bIsInRightZone;
	UPROPERTY(EditAnywhere, Category = "Movements")
		bool bIsInLeftZone;
	

	UPROPERTY(EditAnywhere, Category = "Debug")
		FString DebugString2;

	bool HasNull();
	UPROPERTY(EditAnywhere, Category = "Debug")
		FString NullString;

	void SetClosedState();
	void SetIdleState();
	void KeepScroll(float HorizontalMovement);
	void SlicedScroll(float HorizontalMovement);
	void CameraMove(FVector MoveVect);
	void ScrollLeft(float DeltaTime);

	UPROPERTY(EditAnywhere, Category = "Movements")
		bool bIsScrollingRight;
	UPROPERTY(EditAnywhere, Category = "Movements")
		bool bIsScrollingLeft;

	void DisableAllShots();

	UPROPERTY(EditAnywhere, Category = "Stats")
		float MonsterTimer;
	UPROPERTY(EditAnywhere, Category = "Stats")
		float MonsterDuration;
	UPROPERTY(EditAnywhere, Category = "Stats")
		bool bIsScrolling;
};
