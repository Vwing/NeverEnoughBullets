// Fill out your copyright notice in the Description page of Project Settings.

#include "GameJam2016.h"
#include "Referencer.h"
#include "Ship.h"
#include "Monster.h"
#include "PaperSpriteComponent.h"
#include "ShipHUD.h"

AShipHUD::AShipHUD()
{
	ScreenSizeX = 1920;
	ScreenSizeY = 1080;

	HUDState = EHUDStates::ErrorState;


	//Deaths HUD
	ConstructorHelpers::FObjectFinder<UTexture2D> DeathAsset(TEXT("Texture2D'/Game/Sprites/tombstone1.tombstone1'"));
	DeathTexture = DeathAsset.Object;

	DeathFracX = .90;
	DeathFracY = .025;
	DeathFracWidth = .04;
	DeathFracHeight = .06;

	//DeathNum HUD
	ConstructorHelpers::FObjectFinder<UFont> FontAsset(TEXT("Font'/Game/NewFont.NewFont'"));
	Font = FontAsset.Object;
	Deaths = 0;


	//DeathTimer HUD
	ConstructorHelpers::FObjectFinder<UTexture2D> DeathTimerAsset(TEXT("Texture2D'/Game/Sprites/red_box.red_box'"));
	DeathTimerTexture = DeathTimerAsset.Object;

	DeathTimerFracX = .05;
	DeathTimerFracY = 0.9;
	DeathTimerFracWidth = 0.85;
	DeathTimerFracHeight = .06;
	DeathTimer = 0;
	MaxDeathTimer = 1;

	//Absorb HUD
	ConstructorHelpers::FObjectFinder<UTexture2D> AbsorbTimerAsset(TEXT("Texture2D'/Game/Sprites/red_box.red_box'"));
	AbsorbTimerTexture = AbsorbTimerAsset.Object;
	ConstructorHelpers::FObjectFinder<UTexture2D> AbsorbTimerRefillAsset(TEXT("Texture2D'/Game/Sprites/blue-box.blue-box'"));
	AbsorbTimerRefillTexture = AbsorbTimerRefillAsset.Object;
	ConstructorHelpers::FObjectFinder<UTexture2D> AbsorbTimerFullAsset(TEXT("Texture2D'/Game/Sprites/blue-box.blue-box'"));
	AbsorbTimerFullTexture = AbsorbTimerFullAsset.Object;

	AbsorbFracX = .05;
	AbsorbFracY = .07;
	AbsorbFracWidth = .4;
	AbsorbFracHeight = .03;
	AbsorbTimer = 0;
	AbsorbDuration = 0;
	AbsorbAgainDuration = 1;


	//Ammo HUD
	ConstructorHelpers::FObjectFinder<UTexture2D> AmmoTextureAsset(TEXT("Texture2D'/Game/Sprites/tombstone1.tombstone1'"));
	AmmoTexture = AmmoTextureAsset.Object;
	ConstructorHelpers::FObjectFinder<UTexture2D> EmptyAmmoTextureAsset(TEXT("Texture2D'/Game/Sprites/red_box.red_box'"));
	EmptyAmmoTexture = EmptyAmmoTextureAsset.Object;
	AmmoFracX = .05;
	AmmoFracY = .01;
	AmmoFracWidth = .05;
	AmmoFracHeight = .05;
	AmmoFracOffset = .005;
	Ammo = 0;

}

void AShipHUD::BeginPlay()
{
	Super::BeginPlay();

	if (!HasNull())
	{
		HUDState = EHUDStates::Operational;
		MaxTimer = Ref->Monster->MaxTimer;
		AbsorbDuration = Ref->Ship->AbsorbDuration;
		AbsorbAgainDuration = Ref->Ship->AbsorbAgainTimer;
		AbsorbFracWidth = (AmmoFracWidth*Ref->Ship->MaxAmmo) + (AmmoFracOffset*(Ref->Ship->MaxAmmo-1)); //get the offset the thing in line with width of all the ammo
		MaxDeathTimer = Ref->Monster->MaxTimer;
	}
}

void AShipHUD::DrawHUD()
{
	if (HasNull())
	{
		return;
	}

	UpdateReferences();
	UpdateHUDState();

	Super::DrawHUD();
}

bool AShipHUD::HasNull()
{
	if (Ref == nullptr ||
		Ref == NULL)
	{
		NullString = "Ref Is Null";
		HUDState = EHUDStates::ErrorState;
		return true;
	}
	else if (Ref->HasNullActors())
	{
		NullString = "Ref has Null";
		HUDState = EHUDStates::ErrorState;
		return true;
	}
	else
	{
		return false;
	}
}

void AShipHUD::UpdateHUDState()
{
	switch (HUDState)
	{
	case EHUDStates::Operational:
		DrawDeathTexture();
		DrawNumDeaths();
		DrawAmmoTexture();
		DrawDeathTimer();
		DrawAbsorbTimer();
		break;

	case EHUDStates::ErrorState:
		break;
	}
}

void AShipHUD::DrawDeathTexture()
{
	float DeathX = ScreenSizeX*DeathFracX;
	float DeathY = ScreenSizeY*DeathFracY;
	float DeathWidth = ScreenSizeX*DeathFracWidth;
	float DeathHeight = ScreenSizeY*DeathFracHeight;

	DrawTexture(DeathTexture, DeathX, DeathY, -DeathWidth, DeathHeight, 1, 1, 1, 1);
}

void AShipHUD::DrawNumDeaths()
{
	float DeathX = ScreenSizeX*(DeathFracX - DeathFracWidth*1.5);
	float DeathY = ScreenSizeY*DeathFracY/2;
	float DeathX2 = ScreenSizeX*(DeathFracX - DeathFracWidth*2);

	DrawText("x", FColor::White, DeathX, DeathY, Font);
	DrawText(FString::FromInt(Deaths), FColor::White, DeathX2,DeathY, Font);
}

void AShipHUD::DrawDeathTimer()
{
	float DeathTimerX = ScreenSizeX*DeathTimerFracX;
	float DeathTimerY = ScreenSizeY*DeathTimerFracY;

	float DeathTimerWidth = ScreenSizeX*DeathTimerFracWidth*(DeathTimer/MaxDeathTimer > 1? 1 : DeathTimer/MaxDeathTimer);
	float DeathTimerHeight = ScreenSizeY*DeathTimerFracHeight;

	DrawTexture(DeathTimerTexture, DeathTimerX, DeathTimerY, DeathTimerWidth, DeathTimerHeight, 1, 1, 1, 1);
}

void AShipHUD::UpdateReferences()
{
	Deaths = Ref->Ship->Deaths;
	Ammo = Ref->Ship->Ammo;
	DeathTimer = Ref->Monster->CurrentTimer; // MaxDeathTimer;
	AbsorbTimer = Ref->Ship->AbsorbTimer;

	GetWorld()->GetFirstPlayerController()->GetViewportSize(ScreenSizeX, ScreenSizeY);
}

void AShipHUD::DrawAbsorbTimer()
{
	float AbsorbY = AbsorbFracY * ScreenSizeY;
	float AbsorbHeight = AbsorbFracHeight*ScreenSizeY;

	if (AbsorbTimer == 0)
	{
		float AbsorbX = AbsorbFracX * ScreenSizeX;
		float AbsorbWidth = AbsorbFracWidth*ScreenSizeX;
		DrawTexture(AbsorbTimerFullTexture, AbsorbX, AbsorbY, AbsorbWidth, AbsorbHeight, 1, 1, 1, 1);
	}
	else if (AbsorbTimer <= AbsorbDuration) // using absorb
	{
		//float AbsorbX = ScreenSizeX*(AbsorbFracX - AbsorbFracWidth);
		//float AbsorbWidth = (AbsorbTimer / AbsorbDuration)*AbsorbFracWidth*ScreenSizeX;
		float AbsorbX = AbsorbFracX * ScreenSizeX;
		float AbsorbWidth = ((AbsorbDuration - AbsorbTimer) / AbsorbDuration)*ScreenSizeX*AbsorbFracWidth;
		DrawTexture(AbsorbTimerRefillTexture, AbsorbX, AbsorbY, AbsorbWidth, AbsorbHeight, 1, 1, 1, 1);
	}
	else // if (AbsorbTimer > AbsorbDuration) // refilling absorb
	{
		float AbsorbX = AbsorbFracX * ScreenSizeX;
		// if this AbsorbAgainDuration - AbsorbDuration is zero we have a problem, but it shouldnt be
		if (AbsorbTimer > AbsorbAgainDuration)
		{ // if further along then the absorb again duration then just make it the max
			AbsorbTimer = AbsorbAgainDuration;
		}
		float CurrentRefillTime = (AbsorbTimer - AbsorbDuration) / (AbsorbAgainDuration - AbsorbDuration != 0 ? AbsorbAgainDuration - AbsorbDuration : 1); //make sure absorb duration is not 0
		float AbsorbWidth = (CurrentRefillTime)*AbsorbFracWidth*ScreenSizeX;

		DrawTexture(AbsorbTimerTexture, AbsorbX, AbsorbY, AbsorbWidth, AbsorbHeight, 1, 1, 1, 1);
	}
}

void AShipHUD::DrawAmmoTexture()
{
	float AmmoX = AmmoFracX*ScreenSizeX;
	float AmmoY = AmmoFracY*ScreenSizeY;
	float AmmoWidth = AmmoFracWidth*ScreenSizeX;
	float AmmoHeight = AmmoFracHeight*ScreenSizeY;
	float AmmoOffset = AmmoFracOffset*ScreenSizeX;

	//DrawTexture(AmmoTexture, AmmoX*AmmoOffset, AmmoY, AmmoWidth, AmmoHeight, 1, 1, 1, 1);

	for (int32 i = 0; i < Ammo ; ++i)
	{
		DrawTexture(AmmoTexture, AmmoX + (i*(AmmoOffset+AmmoWidth)), AmmoY, AmmoWidth, AmmoHeight, 1, 1, 1, 1); //ammowidth is a given plus the actual offset
	}
	for (int32 i = 4; i >= Ammo ; --i)
	{
		DrawTexture(EmptyAmmoTexture, AmmoX + (i*(AmmoOffset+AmmoWidth)), AmmoY, AmmoWidth, AmmoHeight, 1, 1, 1, 1);
	}
}