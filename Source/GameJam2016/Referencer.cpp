// Fill out your copyright notice in the Description page of Project Settings.

#include "GameJam2016.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget.h"
#include "Referencer.h"


// Sets default values
AReferencer::AReferencer()
{
	PrimaryActorTick.bCanEverTick = false;
	
	EndWidget = LoadClass<EndMenu>()
	
}

// Called when the game starts or when spawned
void AReferencer::BeginPlay()
{
	Super::BeginPlay();

	if (EndMenu) // Check if the Asset is assigned in the blueprint.
	{
		// Create the widget and store it
		EndWidget = CreateWidget<UUserWidget>(Ship->GetController(), EndMenu);
		// now you can use the widget directly since you have a referance for it.
		// Extra check to  make sure the pointer holds the widget.
		if (EndWidget)
		{
			//let add it to the view port
			EndWidget->AddToViewport();
		}
	}
}

bool AReferencer::HasNullActors()
{
	bool HasNullActors = false;

	if (Camera == NULL ||
		Camera == nullptr)
	{
		HasNullActors = true;
		NullString = "Camera Is Null";
	}
	else if (Monster == NULL ||
		Monster == nullptr)
	{
		HasNullActors = true;
		NullString = "Monster Is Null";
	}
	else if (Ship == NULL ||
		Ship == nullptr)
	{
		HasNullActors = true;
		NullString = "Ship Is Null";
	}
	else if (Background == NULL ||
		Background == nullptr)
	{
		HasNullActors = true;
		NullString = "Background Is Null";
	}
	else if (Bounds == nullptr  || Bounds == NULL)
	{
		HasNullActors = true;
		NullString = "Bounds Is Null";
	}
	
	return HasNullActors;
}

void AReferencer::EndGame()
{

}