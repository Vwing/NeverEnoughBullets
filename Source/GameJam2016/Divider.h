// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Divider.generated.h"

UCLASS()
class GAMEJAM2016_API ADivider : public AActor
{
	GENERATED_BODY()
	
public:	
	ADivider();
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Sprites")
	class UPaperSpriteComponent* Divider;

	void Scroll(FVector MoveVect);

	UPROPERTY(EditAnywhere, Category = "Sprites")
		FString DivideName;
};
