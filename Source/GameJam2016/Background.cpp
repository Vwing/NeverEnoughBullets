// Fill out your copyright notice in the Description page of Project Settings.

#include "GameJam2016.h"
#include "Background.h"
#include "Referencer.h"
#include "Monster.h"
#include "Divider.h"
#include "Ship.h"
#include "PaperSpriteComponent.h"


ABackground::ABackground()
{
	PrimaryActorTick.bCanEverTick = false;
	SetActorEnableCollision(true);

	Package = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Package"));
	Background = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Backgroundz"));
	Foreground = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Foreground"));
	Midground = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Midground"));

	ConstructorHelpers::FObjectFinder<UPaperSprite> WallArt(TEXT("PaperSprite'/Game/Sprites/red_box_Sprite.red_box_Sprite'"));
	LeftWall = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("LeftWall"));
	RightWall = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("RightWall"));

	RootComponent = Package;
	Background->AttachTo(Package);
	Foreground->AttachTo(Package);
	Midground->AttachTo(Package);
	LeftWall->AttachTo(Package);
	RightWall->AttachTo(Package);

	LeftWall->SetSprite(WallArt.Object);
	RightWall->SetSprite(WallArt.Object);
	LeftWall->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RightWall->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LeftWall->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RightWall->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	LeftWall->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	RightWall->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	LeftWall->bGenerateOverlapEvents = true;
	RightWall->bGenerateOverlapEvents = true;
	
	LeftWall->bMultiBodyOverlap = true;
	LeftWall->GetBodyInstance()->bUseCCD = true;
	RightWall->bMultiBodyOverlap = true;
	RightWall->GetBodyInstance()->bUseCCD = true;

	Foreground->bMultiBodyOverlap = true;
	Foreground->GetBodyInstance()->bUseCCD = true;
	Foreground->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Foreground->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	Foreground->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Foreground->bGenerateOverlapEvents = true;

	LeftScrollBounds = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("LeftScrollBounds"));
	LeftScrollBounds->SetSprite(WallArt.Object);
	LeftScrollBounds->AttachTo(RootComponent);
	LeftScrollBounds->SetAbsolute(true, true, true);
	LeftScrollBounds->bGenerateOverlapEvents = true;
	LeftScrollBounds->bMultiBodyOverlap = true;
	LeftScrollBounds->GetBodyInstance()->bUseCCD = true;
	LeftScrollBounds->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LeftScrollBounds->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	LeftScrollBounds->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);

	RightScrollBounds = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("RightScrollBounds"));
	RightScrollBounds->SetSprite(WallArt.Object);
	RightScrollBounds->AttachTo(RootComponent);
	RightScrollBounds->SetAbsolute(true, true, true);
	RightScrollBounds->bGenerateOverlapEvents = true;
	RightScrollBounds->bMultiBodyOverlap = true;
	RightScrollBounds->GetBodyInstance()->bUseCCD = true;
	RightScrollBounds->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RightScrollBounds->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RightScrollBounds->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);

	LeftWall->SetAbsolute(false, false, true);
	RightWall->SetAbsolute(false, false, true);
	Background->SetAbsolute(false, false, true);
	Foreground->SetAbsolute(false, false, true);
	Midground->SetAbsolute(false, false, true);
	bCanScrollRight = true;
	bCanScrollLeft = true;
	bIsInLeftZone = true;
	bIsInRightZone = true;

	BackgroundMultiplier = 0.1f;
	ForegroundMultiplier = 1.0f;
	MidgroundMultiplier = .5f;
}

void ABackground::BeginPlay()
{
	Background->SetAbsolute(false, false, true);
	Foreground->SetAbsolute(false, false, true);
	Midground->SetAbsolute(false, false, true);
	bCanScrollRight = true;
	bCanScrollLeft = true;
	RightWall->bGenerateOverlapEvents = true;
	LeftWall->bGenerateOverlapEvents = true;

	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, -25.0f)); //set away from all other ostacles to make it faster to get overlaps without obstruction

	LeftScrollBounds->bMultiBodyOverlap = true;
	LeftScrollBounds->GetBodyInstance()->bUseCCD = true;
	LeftScrollBounds->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LeftScrollBounds->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	LeftScrollBounds->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);

	RightScrollBounds->bMultiBodyOverlap = true;
	RightScrollBounds->GetBodyInstance()->bUseCCD = true;
	RightScrollBounds->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RightScrollBounds->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RightScrollBounds->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);

	Super::BeginPlay();
}

void ABackground::Scroll(float HorizontalMovements)
{
	FVector MoveVect = FVector(HorizontalMovements*ForegroundMultiplier, 0.0f, 0.0f)*-1;
	Background->AddRelativeLocation(FVector(HorizontalMovements*BackgroundMultiplier, 0.0f, 0.0f)*-1); //must multiply by -1 because that simulates movement
	Midground->AddRelativeLocation(FVector(HorizontalMovements*MidgroundMultiplier, 0.0f, 0.0f)*-1); // when we talk relative to the player input to background
	//Foreground->AddRelativeLocation(FVector(HorizontalMovements*ForegroundMultiplier, 0.0f, 0.0f)*-1);	
	
	//Ref->Monster->Scroll(MoveVect);
//	Ref->BossDivider->Scroll(MoveVect);
	//Ref->StartDivider->Scroll(MoveVect);
	//UpdateScrolls();

	if (bIsOverBossDivider && HorizontalMovements > 0)
	{
		//Ref->Ship->bIsScrollingRight = true;
		//ScrollUntilEnd(HorizontalMovements);
	}
	else if (bIsOverStartDivider && HorizontalMovements < 0)
	{
		Ref->Ship->bIsScrollingLeft = true;
		ScrollUntilEnd(HorizontalMovements);
	}
}

void ABackground::UpdateScrolls()
{
	UpdateScrollingRules();
	UpdateScrollingBounds();
}

void ABackground::UpdateScrollingRules()
{
	Foreground->GetOverlappingComponents(OverlappingComponents); // check all the overlapping components on this 
	bCanScrollLeft = bCanScrollRight = false;

	for (int i = 0; i < OverlappingComponents.Num(); ++i)
	{
		if (OverlappingComponents[i]->GetName().Contains("LeftWall"))
		{
			bCanScrollLeft = true;
		}
		else if (OverlappingComponents[i]->GetName().Contains("RightWall"))
		{
			bCanScrollRight = true;
		}
	}

	if (bCanScrollRight == false)
	{
		Ref->Ship->bIsScrollingRight = false;
	}
	else if (bCanScrollLeft == false)
	{
		Ref->Ship->bIsScrollingLeft = false;
	}
}


void ABackground::UpdateScrollingBounds()
{
	bIsInLeftZone = bIsInRightZone = bIsMonsterOnScreen = false;
	
	LeftScrollBounds->GetOverlappingComponents(OverlappingComponents);
	
	for (int i = 0; i < OverlappingComponents.Num(); ++i)
	{
		if (OverlappingComponents[i]->GetName().Contains("Ship"))
		{
			bIsInLeftZone = true;
			bIsInRightZone = false;
		}
	}
	
	RightScrollBounds->GetOverlappingComponents(OverlappingComponents);

	for (int i = 0; i < OverlappingComponents.Num(); ++i)
	{
		if (OverlappingComponents[i]->GetName().Contains("Ship"))
		{
			bIsInRightZone = true;
			bIsInLeftZone = false;
		}
		else if (OverlappingComponents[i]->GetName().Contains("Monster"))
		{
			bIsMonsterOnScreen = true;
		}
	}
	Ref->Monster->bIsMonsterOnScreen = bIsMonsterOnScreen;
}

void ABackground::ScrollUntilEnd(float ScrollFloat)
{
	//Ref->Ship->SetClosedState();
	//Ref->Monster->SetClosedState();

	KeepScroll(ScrollFloat);
	UpdateScrolls();
}

void ABackground::KeepScroll(float HorizontalMovements)
{ //same as Scroll() but witout the check for start and end divider

	FVector MoveVect = FVector(HorizontalMovements*ForegroundMultiplier, 0.0f, 0.0f)*-1;

	Background->AddRelativeLocation(FVector(HorizontalMovements*BackgroundMultiplier, 0.0f, 0.0f)*-1); //must multiply by -1 because that simulates movement
	Midground->AddRelativeLocation(FVector(HorizontalMovements*MidgroundMultiplier, 0.0f, 0.0f)*-1); // when we talk relative to the player input to background
	Foreground->AddRelativeLocation(FVector(HorizontalMovements*ForegroundMultiplier, 0.0f, 0.0f)*-1);

	Ref->Monster->Scroll(MoveVect);
}

void ABackground::Scrollz(FVector MoveVect)
{
	LeftWall->AddRelativeLocation(MoveVect);
	RightWall->AddRelativeLocation(MoveVect);
	LeftScrollBounds->AddRelativeLocation(MoveVect);
	RightScrollBounds->AddRelativeLocation(MoveVect);
}