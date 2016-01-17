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
	void MakeMovements(float DeltaTime);
	void ShootProjectile();
	void UpdateProjectiles(float DeltaTime);
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

	UPROPERTY(EditAnywhere, Category = "Projectile")
		float ProjectileSpeed;
	UPROPERTY(EditAnywhere, Category = "Projectile")
		int8 MaxShots;
	UPROPERTY(EditAnywhere, Category = "Projectile")
		int8 ShotsInUse;
	UPROPERTY(EditAnywhere, Category = "Stats")
		TArray<class UPaperSpriteComponent*>  ProjectilesArray;
	UPROPERTY(EditAnywhere, Category = "Projectile")
		FRotator ProjectileRotation;
	UPROPERTY(EditAnywhere, Category = "Projectile")
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

	UPROPERTY(EditAnywhere, Category = "Audio")
	class UAudioComponent* ShootingSound;

	UPROPERTY(EditAnywhere,Category= "Debug")
	TArray<UPrimitiveComponent*> OverlappingComponents;

	UPROPERTY(EditAnywhere, Category = "Debug")
		FString DebugString;
	
	bool bCanMoveLeft;
};
