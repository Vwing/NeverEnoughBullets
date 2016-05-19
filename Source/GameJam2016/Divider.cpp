// Fill out your copyright notice in the Description page of Project Settings.

#include "GameJam2016.h"
#include "PaperSpriteComponent.h"
#include "Divider.h"


// Sets default values
ADivider::ADivider()
{
	PrimaryActorTick.bCanEverTick = false;

	Divider = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("DividerSprite"));
	RootComponent = Divider;

	ConstructorHelpers::FObjectFinder<UPaperSprite> DividerSpriteAsset(TEXT("PaperSprite'/Game/Sprites/Slash_sprite_Sprite_4.Slash_sprite_Sprite_4'"));
	Divider->SetSprite(DividerSpriteAsset.Object);

	Divider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	Divider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	Divider->bGenerateOverlapEvents = true;
	
	DivideName = "Divider";
}

// Called when the game starts or when spawned
void ADivider::BeginPlay()
{
	Divider->Rename( *DivideName );
	
	Super::BeginPlay();
}

void ADivider::Scroll(FVector MoveVect)
{
	AddActorWorldOffset(MoveVect);
}