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
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	void Start(float x, float y, int8 color, class AReferencer* ref);
	void UpdateOverlappingComponents(TArray<UPrimitiveComponent*>& OverlappingComponents);
	void MakeMovements(float DeltaTime);
	void Scroll(FVector MoveVect);

	UPROPERTY(EditAnywhere, Category = "Movement")
		float XSpeed;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float YSpeed;

	UPROPERTY(EditAnywhere, Category = "Stats")
		int8 Color;
	UPROPERTY(EditAnywhere, Category = "Stats")
		bool bHasStarted;
	UPROPERTY(EditAnywhere, Category = "Stats")
		class AReferencer* Ref;

	UPROPERTY(EditAnywhere, Category = "Sprite")
	class UPaperSpriteComponent* ProjectileHitBox;
	UPROPERTY(EditAnywhere, Category = "Sprite")
	class UPaperSpriteComponent* ProjectileSprite;

	UPROPERTY(EditAnywhere, Category = "Debug")
		TArray<UPrimitiveComponent*> OverlappingComponents;
	UPROPERTY(EditAnywhere, Category = "Debug")
		FString DebugString;
};

