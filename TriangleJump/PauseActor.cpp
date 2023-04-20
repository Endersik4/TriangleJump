// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseActor.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

#include "GateActor.h"
#include "TrianglePawn.h"

// Sets default values
APauseActor::APauseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Pause = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Pause"));
	RootComponent = Pause;

	BackToMainMenu = CreateDefaultSubobject<UTextRenderComponent>(TEXT("BackToMainMenu"));
	BackToMainMenu->SetupAttachment(RootComponent);

	BackToMainMenuBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BackToMainMenuBox"));
	BackToMainMenuBox->SetupAttachment(BackToMainMenu);

	Resume = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Resume"));
	Resume->SetupAttachment(RootComponent);

	ResumeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ResumeBox"));
	ResumeBox->SetupAttachment(Resume);

}

// Called when the game starts or when spawned
void APauseActor::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGateActor::StaticClass(), GateArray);
	Gate = Cast<AGateActor>(GateArray[0]);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATrianglePawn::StaticClass(), TriangleArray);
	Triangle = Cast<ATrianglePawn>(TriangleArray[0]);

}

void APauseActor::BackToMainMenuGame()
{
	FTimerHandle GameOverHandle;
	GetWorldTimerManager().SetTimer(GameOverHandle, this, &APauseActor::BackMenu, 0.1f, false, 1.15f);

	Gate->IsOpen = true;
	Gate->SoundWhenClose = false;
	Gate->SoundWhenOpen = false;
}

void APauseActor::ResumeGame()
{
	FTimerHandle GameOverHandle;
	GetWorldTimerManager().SetTimer(GameOverHandle, this, &APauseActor::ResumeTime, 0.1f, false, 1.15f);

	Gate->IsOpen = true;
	Gate->SoundWhenClose = false;
	Gate->SoundWhenOpen = false;
}

// Called every frame
void APauseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APauseActor::BackMenu()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void APauseActor::ResumeTime()
{
	Triangle->ResumeGameObjects();
	Destroy();
}

