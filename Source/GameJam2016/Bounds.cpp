// Fill out your copyright notice in the Description page of Project Settings.

#include "GameJam2016.h"
#include "Bounds.h"
#include "PaperSpriteComponent.h"


// Sets default values
ABounds::ABounds()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VerticalBounds = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("VerticalBoundsSprite"));
	HorizontalBounds = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("HorizontalBoundsSprite"));
	ConstructorHelpers::FObjectFinder<UPaperSprite> HorizontalBounds(TEXT("PaperSprite'/Game/Sprites/HorizontalBounds.HorizontalBounds'"));
	ConstructorHelpers::FObjectFinder<UPaperSprite> VerticalBounds(TEXT("PaperSprite'/Game/Sprites/VerticalBounds.VerticalBounds'"));
}

// Called when the game starts or when spawned
void ABounds::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABounds::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

