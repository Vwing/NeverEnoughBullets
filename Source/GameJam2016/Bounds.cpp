// Fill out your copyright notice in the Description page of Project Settings.

#include "GameJam2016.h"
#include "Bounds.h"
#include "PaperSpriteComponent.h"


// Sets default values
ABounds::ABounds()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//SetActorEnableCollision(true);

	/*
	LeftBounds = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("LeftBoundsSprite"));
	ConstructorHelpers::FObjectFinder<UPaperSprite> LeftBoundsAsset(TEXT("PaperSprite'/Game/Sprites/LeftBounds.LeftBounds'"));
	LeftBounds->SetSprite(LeftBoundsAsset.Object);
	RootComponent = LeftBounds;

	LeftBounds->GetBodyInstance()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LeftBounds->GetBodyInstance()->SetResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	LeftBounds->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	LeftBounds->GetBodyInstance()->bLockRotation = true;
	LeftBounds->GetBodyInstance()->bLockTranslation = true;
	LeftBounds->SetAbsolute(false, false, false);
	LeftBounds->SetSimulatePhysics(false);


	TopBounds = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TopBoundsSprite"));
	ConstructorHelpers::FObjectFinder<UPaperSprite> TopBoundsAsset(TEXT("PaperSprite'/Game/Sprites/TopBounds.TopBounds'"));
	TopBounds->SetSprite(TopBoundsAsset.Object);
	TopBounds->AttachTo(RootComponent);

	TopBounds->GetBodyInstance()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TopBounds->GetBodyInstance()->SetResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	TopBounds->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TopBounds->GetBodyInstance()->bLockRotation = true;
	TopBounds->GetBodyInstance()->bLockTranslation = true;
	TopBounds->SetAbsolute(false, false, false);	
	
	BottomBounds = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BottomBoundsSprite"));
	ConstructorHelpers::FObjectFinder<UPaperSprite> BottomBoundsAsset(TEXT("PaperSprite'/Game/Sprites/BottomBounds.BottomBounds'"));
	BottomBounds->SetSprite(BottomBoundsAsset.Object);
	BottomBounds->AttachTo(RootComponent);

	BottomBounds->GetBodyInstance()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BottomBounds->GetBodyInstance()->SetResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	BottomBounds->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	BottomBounds->GetBodyInstance()->bLockRotation = true;
	BottomBounds->GetBodyInstance()->bLockTranslation = true;
	BottomBounds->SetAbsolute(false, false, false);

	
	RightBounds = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("RightBoundsSprite"));
	ConstructorHelpers::FObjectFinder<UPaperSprite> RightBoundsAsset(TEXT("PaperSprite'/Game/Sprites/RightBounds.RightBounds'"));
	RightBounds->SetSprite(RightBoundsAsset.Object);
	RightBounds->AttachTo(RootComponent);

	RightBounds->GetBodyInstance()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RightBounds->GetBodyInstance()->SetResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RightBounds->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	RightBounds->GetBodyInstance()->bLockRotation = true;
	RightBounds->GetBodyInstance()->bLockTranslation = true;	
	RightBounds->SetAbsolute(false, false, false);

	SetActorLocation(FVector(0.0f, 0.0f, 0.0f));
	SetActorRotation(FRotator(90.0f, 0.0f, 0.0f));

	RightBounds->RelativeLocation = FVector(0.0f, 0.0f, .10f);
	RightBounds->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	TopBounds->RelativeLocation = FVector(0.0f, 0.0f, 0.20f);
	TopBounds->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	BottomBounds->RelativeLocation = FVector(0.0f, 0.0f, 0.30f);
	BottomBounds->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
	LeftBounds->RelativeLocation = FVector(0.0f, 0.0f, 0.40f);
	LeftBounds->RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);

	InnerBoundsRotation = FRotator(90.0f, 0.0f, 90.0f);
	*/
}

// Called when the game starts or when spawned
void ABounds::BeginPlay()
{
	Super::BeginPlay();

//	BoundsRadius = BottomInnerBounds->Bounds.BoxExtent.Z;

	//InnerBottomBounds->SetWorldRotation(InnerBoundsRotation);
}

// Called every frame
void ABounds::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

