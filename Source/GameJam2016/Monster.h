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

	UPROPERTY(EditAnywhere, Category = "Anim")
	class UPaperFlipbookComponent* MonsterFlipbook;
	UPROPERTY(EditAnywhere,Category="Anim")
	class UPaperFlipbook* IdleAnim;
	UPROPERTY(EditAnywhere, Category = "Anim")
	class UPaperFlipbook* DamagedAnim;

	class UPaperSpriteComponent* MonsterSprite;

	UPROPERTY(EditAnywhere, Category = "Sound")
	class UAudioComponent* DamagedSound;

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
	
	void SetDamagedState();
	void SetDamagedAnim();
	void SetIdleAnim();
	void SetIdleState();
	FTimerHandle MonsterHandle;
};
