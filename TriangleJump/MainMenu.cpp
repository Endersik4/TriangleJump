// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/TextRenderComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundCue.h"

#include "UpCameraActor.h"
#include "TrianglePawn.h"
#include "GateActor.h"
#include "StatsActor.h"

#define mes(writing) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, writing); //you can just write mes("String") to add information on scre

// Sets default values
AMainMenu::AMainMenu()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriangleJumpMenu = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TriangleJumpMenu"));
	RootComponent = TriangleJumpMenu;

	Start = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Start"));
	Start->SetupAttachment(RootComponent);

	Stats = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Stats"));
	Stats->SetupAttachment(RootComponent);

	Quit = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Quit"));
	Quit->SetupAttachment(RootComponent);

	StartBox = CreateDefaultSubobject<UBoxComponent>(TEXT("StartBox"));
	StartBox->SetupAttachment(Start);

	StatsBox = CreateDefaultSubobject<UBoxComponent>(TEXT("StatsBox"));
	StatsBox->SetupAttachment(Stats);

	QuitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("QuitBox"));
	QuitBox->SetupAttachment(Quit);
}

// Called when the game starts or when spawned
void AMainMenu::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUpCameraActor::StaticClass(), UpCameraArray);
	UpCamera = Cast<AUpCameraActor>(UpCameraArray[0]);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATrianglePawn::StaticClass(), TriangleArray);
	Triangle = Cast<ATrianglePawn>(TriangleArray[0]);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGateActor::StaticClass(), GateArray);
	Gate = Cast<AGateActor>(GateArray[0]);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStatsActor::StaticClass(), StatsArray);
	StatsActor = Cast<AStatsActor>(StatsArray[0]);

}

void AMainMenu::StartFunction()
{
	if (IsMainHidden == false)
	{
		Triangle->LoadGame();
		FTimerHandle GameOverHandle;
		GetWorldTimerManager().SetTimer(GameOverHandle, this, &AMainMenu::StartGame, 0.1f, false, 1.15f);

		Gate->IsOpen = true;
		Gate->SoundWhenClose = false;
		Gate->SoundWhenOpen = false;

		IsMainHidden = true;
	}
}

void AMainMenu::StatsFunction()
{
	if (IsMainHidden == false)
	{
		Triangle->LoadGame();
		FTimerHandle GameOverHandle;
		GetWorldTimerManager().SetTimer(GameOverHandle, this, &AMainMenu::StatsGame, 0.1f, false, 1.1f);

		Gate->IsOpen = true;
		Gate->SoundWhenClose = false;
		Gate->SoundWhenOpen = false;

		IsMainHidden = true;
	}
}

void AMainMenu::QuitFunction()
{
	if (IsMainHidden == false)
	{
		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), TEXT("quit"));
	}
}

// Called every frame
void AMainMenu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainMenu::StartGame()
{
	UpCamera->SetActorHiddenInGame(false);
	UpCamera->GameStarted();
	UpCamera->HighPoints = StatsActor->HighScore;
	UpCamera->RecentPoints = StatsActor->LastScore;

	Triangle->SetActorHiddenInGame(false);
	Triangle->GameBegins();
	SetActorHiddenInGame(true);

	UGameplayStatics::PlaySound2D(this, StartSound);
}

void AMainMenu::StatsGame()
{
	StatsActor->SetActorHiddenInGame(false);
	StatsActor->IsStatsHidden = false;
	SetActorHiddenInGame(true);
}

