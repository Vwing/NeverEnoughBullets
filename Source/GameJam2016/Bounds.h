// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Bounds.generated.h"

UCLASS()
class GAMEJAM2016_API ABounds : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABounds();

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	class UPaperSpriteComponent* RootBounds;
	class UPaperSpriteComponent* TopBounds;
	class UPaperSpriteComponent* BottomBounds;
	class UPaperSpriteComponent* LeftBounds;
	class UPaperSpriteComponent* RightBounds;

	UPROPERTY(EditAnywhere, Category = "Stats")
		float BoundsRadius;
};
