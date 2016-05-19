// Fill out your copyright notice in the Description page of Project Settings.

#include "GameJam2016.h"
#include "Ship.h"
#include "ShipGameState.h"
#include "ShipHUD.h"
#include "Referencer.h"
#include "GameJam2016GameMode.h"

AGameJam2016GameMode::AGameJam2016GameMode()
{
	DefaultPawnClass = AShip::StaticClass();
	HUDClass = AShipHUD::StaticClass();
}

void AGameJam2016GameMode::StartPlay()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		AShipHUD* hud = (AShipHUD*)(GetWorld()->GetFirstPlayerController()->GetHUD());
		AShip* ship = (AShip*)(World->GetFirstPlayerController()->GetPawn());

		if (hud != nullptr && hud != NULL &&
			ship != nullptr && ship != NULL)
		{
			AReferencer* Ref = ship->Ref;
			World->GetFirstPlayerController()->SetViewTarget(Ref->Camera); //causes crash
			hud->Ref = Ref;
			World->GetFirstPlayerController()->GetViewportSize(hud->XScreen, hud->YScreen);
		}
	}

	Super::StartPlay();
	Super::StartMatch();	
}


