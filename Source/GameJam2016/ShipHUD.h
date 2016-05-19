// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "ShipHUD.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAM2016_API AShipHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AShipHUD();
	virtual void DrawHUD() override;
	void BeginPlay();

	UPROPERTY(EditAnywhere, Category = "HUD")
		class AReferencer* Ref;

	UPROPERTY(EditAnywhere, Category = "Text")
		int32 XScreen;
	UPROPERTY(EditAnywhere, Category = "Text")
		int32 YScreen;
	UPROPERTY(EditAnywhere, Category = "Text")
		float XOffset;
	UPROPERTY(EditAnywhere, Category = "Text")
		float YOffset;

	bool HasNull();
	UPROPERTY(EditAnywhere, Category = "Debug")
		FString NullString;
	UPROPERTY(EditAnywhere, Category = "Debug")
		FString NullString2;
private:	

	struct EHUDStates
	{
		enum Type
		{
			Operational,
			ErrorState
		};
	};
	EHUDStates::Type HUDState;

	
	UPROPERTY(EditAnywhere, Category = "HUD")
	float CurrentHealth;
	UPROPERTY(EditAnywhere, Category = "HUD")
	float MaxHealth;
	UPROPERTY(EditAnywhere, Category = "HUD")
	float MaxTimer;
	UPROPERTY(EditAnywhere, Category = "HUD")
	float CurrentTimer;


	void DrawAbsorbTimer();
	void UpdateHUDState();
	void DrawNumDeaths();
	void DrawDeathTexture();
	void DrawAmmoTexture();
	void UpdateReferences();
	void DrawDeathTimer();


	UPROPERTY(EditAnywhere, Category = "HUD")
		int32 ScreenSizeX;
	UPROPERTY(EditAnywhere, Category = "HUD")
		int32 ScreenSizeY;

	//Death HUD
	UPROPERTY(EditAnywhere, Category = "Death")
		UTexture2D* DeathTexture;
	UPROPERTY(EditAnywhere, Category = "Death")
		int32 Deaths;
	UPROPERTY(EditAnywhere, Category = "Death")
		float DeathFracX;
	UPROPERTY(EditAnywhere, Category = "Death")
		float DeathFracY;
	UPROPERTY(EditAnywhere, Category = "Death")
		float DeathFracWidth;
	UPROPERTY(EditAnywhere, Category = "Death")
		float DeathFracHeight;

	//DeathNum HUD
	UPROPERTY(EditAnywhere, Category = "Death")
	class UFont* Font;

	//DeathTimer HUD
	UPROPERTY(EditAnywhere, Category = "DeathTimer")
		UTexture2D* DeathTimerTexture;
	UPROPERTY(EditAnywhere, Category = "DeathTimer")
		float DeathTimerFracX;
	UPROPERTY(EditAnywhere, Category = "DeathTimer")
		float DeathTimerFracY;
	UPROPERTY(EditAnywhere, Category = "DeathTimer")
		float DeathTimerFracWidth;
	UPROPERTY(EditAnywhere, Category = "DeathTimer")
		float DeathTimerFracHeight;
	UPROPERTY(EditAnywhere, Category = "DeathTimer")
		float DeathTimer;
	UPROPERTY(EditAnywhere, Category = "DeathTimer")
		float MaxDeathTimer;

	//Absorb HUD
	UPROPERTY(EditAnywhere, Category = "Absorb")
		UTexture2D* AbsorbTimerTexture;
	UPROPERTY(EditAnywhere, Category = "Absorb")
		UTexture2D* AbsorbTimerFullTexture;
	UPROPERTY(EditAnywhere, Category = "Absorb")
		UTexture2D* AbsorbTimerRefillTexture;
	UPROPERTY(EditAnywhere, Category = "Absorb")
		float AbsorbFracX;
	UPROPERTY(EditAnywhere, Category = "Absorb")
		float AbsorbFracY;
	UPROPERTY(EditAnywhere, Category = "Absorb")
		float AbsorbFracWidth;
	UPROPERTY(EditAnywhere, Category = "Absorb")
		float AbsorbFracHeight;
	UPROPERTY(EditAnywhere, Category = "Absorb")
		float AbsorbTimer;
	UPROPERTY(EditAnywhere, Category = "Absorb")
		float AbsorbDuration;
	UPROPERTY(EditAnywhere, Category = "Absorb")
		float AbsorbAgainDuration;

	//Ammo HUD
	UPROPERTY(EditAnywhere, Category = "Ammo")
		UTexture2D* AmmoTexture;
	UPROPERTY(EditAnywhere, Category = "Ammo")
		UTexture2D* EmptyAmmoTexture;
	UPROPERTY(EditAnywhere, Category = "Ammo")
		float AmmoFracX;
	UPROPERTY(EditAnywhere, Category = "Ammo")
		float AmmoFracY;
	UPROPERTY(EditAnywhere, Category = "Ammo")
		float AmmoFracWidth;
	UPROPERTY(EditAnywhere, Category = "Ammo")
		float AmmoFracHeight;
	UPROPERTY(EditAnywhere, Category = "Ammo")
		float AmmoFracOffset;
	UPROPERTY(EditAnywhere, Category = "Ammo")
		float Ammo;
	
};
