// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class GAMEJAM2016_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void MakeMovements(float DeltaTime);

	void UpdateOverlappingComponents(TArray<UPrimitiveComponent*>& OverlappingComponents);

	struct EProjectileStates
	{
		enum Type
		{
			PlayerShot,
			MonsterShotStraight,
			MonsterShotSin
		};

	};

	UPROPERTY(EditAnywhere, Category = "Movement")
		EProjectileStates::Type ProjectileState;

	UPROPERTY(EditAnywhere, Category = "Movement")
		FVector MovementDirection;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float Speed;

	UPROPERTY(EditAnywhere, Category = "Visual")
	class UPaperSpriteComponent* ProjectileSprite;

	UPROPERTY(EditAnywhere, Category = "Audio")
	class UAudioComponent* ShootingSound;

	UPROPERTY(EditAnywhere, Category = "Debug")
		TArray<UPrimitiveComponent*> OverlappingComponents;

	UPROPERTY(EditAnywhere, Category = "Debug")
		FString DebugString;
	
	
};
