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
	virtual void Tick( float DeltaSeconds ) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	
	void Fire();
	void StopFire();
	void MoveUp(float AxisValue);
	void MoveRight(float AxisValue);
	void Absorb();
	void StopAbsorb();
	void InitiateAbsorb();
	
	void MakeMovements(float DeltaTime);
	void ShootProjectile();
	void UpdateProjectiles(float DeltaTime);
	void UpdateMonster();
	void UpdateOverlappingComponents(TArray<UPrimitiveComponent*>& OverlappingComponents);
	bool UpdateOverlappingProjectiles(TArray<UPrimitiveComponent*>& OverlappingComponents);
	
	UPROPERTY(EditAnywhere, Category = "Stats")
		float MaxVerticalSpeed;
	UPROPERTY(EditAnywhere, Category = "Stats")
		float MaxHorizontalSpeed;
	UPROPERTY(EditAnywhere, Category = "Stats")
	float CurrentHorizontalSpeed;
	UPROPERTY(EditAnywhere,Category = "Stats")
	float CurrentVerticalSpeed;

	UPROPERTY(EditAnywhere, Category = "Projectiles")
		float ProjectileSpeed;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		int8 MaxShotsUsed;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		int8 ShotsInUse;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		int8 Ammo;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		TArray<class UPaperSpriteComponent*>  ProjectilesArray;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		FRotator ProjectileRotation;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
		bool bCanShoot;

	struct EShipStates
	{
		enum Type
		{
			Static,
			MovingHorizontal,
			MovingVertical,
			MovingVerticalShooting,
			MovingHorizontalShooting,
			Shooting,
			Damaged,
			ErrorState,
			Closed
		};
	};

	EShipStates::Type ShipState;

	UPROPERTY(EditAnywhere, Category = "Stats")
	class UPaperSpriteComponent* ShipSprite;
	UPROPERTY(EditAnywhere, Category = "Stats")
	class UPaperSpriteComponent* AbsorbSprite; 

	UPROPERTY(EditAnywhere, Category = "Audio")
	class UAudioComponent* AbsorbSound;
	UPROPERTY(EditAnywhere, Category = "Audio")
	class UAudioComponent* ShootingSound;
	UPROPERTY(EditAnywhere, Category = "Stats")
	class UPaperSpriteComponent* TopInnerBounds;
	UPROPERTY(EditAnywhere, Category = "Stats")
	class UPaperSpriteComponent* BottomInnerBounds;
	UPROPERTY(EditAnywhere,Category= "Debug")
	TArray<UPrimitiveComponent*> OverlappingComponents;

	UPROPERTY(EditAnywhere, Category = "Debug")
		FString DebugString;
	UPROPERTY(EditAnywhere, Category = "Debug")
		FVector DebugLocation;

	UPROPERTY(EditAnywhere, Category = "Stats")
	class AMonster* MonsterReference;

	bool bCanMoveLeft;
	bool bCanMoveRight;
	bool bIsDead;
	bool bCanAbsorb;

	FTimerHandle ShipHandle;
	
};
