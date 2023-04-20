// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsActor.h"
#include "Components/TextRenderComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "MainMenu.h"
#include "GateActor.h"

#define mes(writing) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, writing); //you can just write mes("String") to add information on screen 

// Sets default values
AStatsActor::AStatsActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Stats = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Stats"));
	RootComponent = Stats;

	Back = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Back"));
	Back->SetupAttachment(RootComponent);

	BackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BackBox"));
	BackBox->SetupAttachment(Back);

	Val_HighScore = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Val_HighScore;"));
	Val_HighScore->SetupAttachment(RootComponent);

	Val_Time = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Val_Time"));
	Val_Time->SetupAttachment(RootComponent);

	Val_LastScore = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Val_LastScore"));
	Val_LastScore->SetupAttachment(RootComponent);

	Val_Attempts = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Val_Attempts"));
	Val_Attempts->SetupAttachment(RootComponent);

	Val_KilledByEnemy = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Val_KilledByEnemy"));
	Val_KilledByEnemy->SetupAttachment(RootComponent);

	Val_UsedSprings = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Val_UsedSprings"));
	Val_UsedSprings->SetupAttachment(RootComponent);

	Val_UsedJetEngines = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Val_UsedJetEngines"));
	Val_UsedJetEngines->SetupAttachment(RootComponent);

	Val_UsedJetpacks = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Val_UsedJetpacks"));
	Val_UsedJetpacks->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AStatsActor::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainMenu::StaticClass(), MainMenuArray);
	MainMenu = Cast<AMainMenu>(MainMenuArray[0]);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGateActor::StaticClass(), GateArray);
	Gate = Cast<AGateActor>(GateArray[0]);

	FString HelpString = FString::FromInt(HighScore); //I had weird bug with font and this fixed it
	Val_HighScore->SetText(HelpString);
	//mes("KURWA");

	HelpString = FString::FromInt(LastScore);
	Val_LastScore->SetText(HelpString);

	HelpString = FString::FromInt(Attempts);
	Val_Attempts->SetText(HelpString);

	HelpString = FString::FromInt(KilledByEnemy);
	Val_KilledByEnemy->SetText(HelpString);

	HelpString = FString::FromInt(UsedSprings);
	Val_UsedSprings->SetText(HelpString);

	HelpString = FString::FromInt(UsedJetEngines);
	Val_UsedJetEngines->SetText(HelpString);

	HelpString = FString::FromInt(UsedJetpacks);
	Val_UsedJetpacks->SetText(HelpString);
	
}

void AStatsActor::BackFunction()
{
	if (IsStatsHidden == false)
	{
		FTimerHandle GameOverHandle;
		GetWorldTimerManager().SetTimer(GameOverHandle, this, &AStatsActor::BackToMenu, 0.1f, false, 1.1f);

		Gate->IsOpen = true;
		Gate->SoundWhenClose = false;
		Gate->SoundWhenOpen = false;

		IsStatsHidden = true;
	}
}

// Called every frame
void AStatsActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Time += 1 * DeltaTime;
	if (Time >= 1.f)
	{
		Sekundy += 1;
		if (Sekundy >= 60)
		{
			Minuty += 1;
			Sekundy = 0;
		}
		if (Minuty >= 60)
		{
			Minuty = 0;
			Godziny += 1;
		}

		Time = 0;
	}
	SekundyString = FString::FromInt(Sekundy);
	MinutyString = FString::FromInt(Minuty);
	GodzinyString = FString::FromInt(Godziny);
	TimeString = GodzinyString + ":" + MinutyString + ":" + SekundyString;
	Val_Time->SetText(TimeString);

}

void AStatsActor::FromIntToString()
{
	FString HelpString = FString::FromInt(HighScore); //I had weird bug with font and this fixed it
	Val_HighScore->SetText(HelpString);

	HelpString = FString::FromInt(LastScore);
	Val_LastScore->SetText(HelpString);

	HelpString = FString::FromInt(Attempts);
	Val_Attempts->SetText(HelpString);

	HelpString = FString::FromInt(KilledByEnemy);
	Val_KilledByEnemy->SetText(HelpString);

	HelpString = FString::FromInt(UsedSprings);
	Val_UsedSprings->SetText(HelpString);

	HelpString = FString::FromInt(UsedJetEngines);
	Val_UsedJetEngines->SetText(HelpString);

	HelpString = FString::FromInt(UsedJetpacks);
	Val_UsedJetpacks->SetText(HelpString);
}

void AStatsActor::BackToMenu()
{
	MainMenu->SetActorHiddenInGame(false);
	MainMenu->IsMainHidden = false;
	SetActorHiddenInGame(true);
}

