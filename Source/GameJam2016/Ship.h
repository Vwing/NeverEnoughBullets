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
	void MoveUp(float AxisValue);
	void MoveRight(float AxisValue);
	

	UPROPERTY(EditAnywhere, Category = "Stats")
		float MaxVerticalSpeed;
	UPROPERTY(EditAnywhere, Category = "Stats")
		float MaxHorizontalSpeed;
	UPROPERTY(EditAnywhere, Category = "Stats")
	float CurrentHorizontalSpeed;
	UPROPERTY(EditAnywhere,Category = "Stats")
	float CurrentVerticalSpeed;

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
};
