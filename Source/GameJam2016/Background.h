// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Background.generated.h"

UCLASS()
class GAMEJAM2016_API ABackground : public AActor
{
	GENERATED_BODY()
	
public:	
	ABackground();
	void Scroll(float HorizontalMovements);
	void KeepScroll(float HorizontalMovements);
	void Scrollz(FVector MoveVect);

	void UpdateScrollingRules();
	void UpdateScrollingBounds();
	void UpdateScrolls();

	UPROPERTY(EditAnywhere, Category = "Stats")
	class UPaperSpriteComponent* RightScrollBounds;
	UPROPERTY(EditAnywhere, Category = "Stats")
	class UPaperSpriteComponent* LeftScrollBounds;

	UPROPERTY(EditAnywhere, Category = "Referencer")
	class AReferencer* Ref;

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Art")
	class UPaperSpriteComponent* Foreground;
	UPROPERTY(EditAnywhere, Category = "Art")
	class UPaperSpriteComponent* Midground;
	UPROPERTY(EditAnywhere, Category = "Art")
	class UPaperSpriteComponent* Background;
	UPROPERTY(EditAnywhere, Category = "Art")
	class UPaperSpriteComponent* Package;

	

	UPROPERTY(EditAnywhere, Category = "Movements")
	class UPaperSpriteComponent* RightWall;
	UPROPERTY(EditAnywhere, Category = "Movements")
	class UPaperSpriteComponent* LeftWall;

	UPROPERTY(EditAnywhere, Category = "Movements")
		bool bCanScrollLeft;
	UPROPERTY(EditAnywhere, Category = "Movements")
		bool bCanScrollRight;
	UPROPERTY(EditAnywhere, Category = "Movements")
		bool bIsInLeftZone;
	UPROPERTY(EditAnywhere, Category = "Movements")
		bool bIsInRightZone;
	UPROPERTY(EditAnywhere, Category = "Movements")
		bool bIsMonsterOnScreen;

	UPROPERTY(EditAnywhere, Category = "Movements")
		float BackgroundMultiplier;
	UPROPERTY(EditAnywhere, Category = "Movements")
		float MidgroundMultiplier;
	UPROPERTY(EditAnywhere, Category = "Movements")
		float ForegroundMultiplier;

	UPROPERTY(EditAnywhere, Category = "Movements")
		TArray<UPrimitiveComponent*> OverlappingComponents;

	void ScrollUntilEnd(float ScrollFloat);

	UPROPERTY(EditAnywhere, Category = "Movements")
		bool bIsOverBossDivider;

	UPROPERTY(EditAnywhere, Category = "Movements")
		bool bIsOverStartDivider;


};


