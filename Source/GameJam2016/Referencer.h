// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Referencer.generated.h"

UCLASS()
class GAMEJAM2016_API AReferencer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReferencer();
	bool HasNullActors();
	void EndGame();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player, HUD and UI")
	class UUserWidget* EndWidget;
	UPROPERTY(EditAnywhere, Category = "Debug")
		FString NullString;

	UPROPERTY(EditAnywhere, Category="References")
	class AActor* Camera;
	UPROPERTY(EditAnywhere, Category="References")
	class AShip* Ship;
	UPROPERTY(EditAnywhere, Category = "References")
	class AMonster* Monster;
	UPROPERTY(EditAnywhere, Category = "References")
	class ABackground* Background;
	UPROPERTY(EditAnywhere, Category = "References")
	class ABounds* Bounds;



	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	
};
