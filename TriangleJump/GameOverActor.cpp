// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverActor.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

#include "GateActor.h"
#include "TrianglePawn.h"
#include "UpCameraActor.h"

#define mes(writing) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, writing); //you can just write mes("String") to add information on screen 

// Sets default values
AGameOverActor::AGameOverActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GameOver = CreateDefaultSubobject<UTextRenderComponent>(TEXT("GameOver"));
	RootComponent = GameOver;

	BackToMainMenu = CreateDefaultSubobject<UTextRenderComponent>(TEXT("BackToMainMenu"));
	BackToMainMenu->SetupAttachment(RootComponent);

	BackToMainMenuBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BackToMainMenuBox"));
	BackToMainMenuBox->SetupAttachment(BackToMainMenu);

	Restart = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Restart"));
	Restart->SetupAttachment(RootComponent);

	RestartBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RestartBox"));
	RestartBox->SetupAttachment(Restart);

}

// Called when the game starts or when spawned
void AGameOverActor::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGateActor::StaticClass(), GateArray);
	Gate = Cast<AGateActor>(GateArray[0]);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUpCameraActor::StaticClass(), UpCameraArray);
	UpCamera = Cast<AUpCameraActor>(UpCameraArray[0]);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATrianglePawn::StaticClass(), TriangleArray);
	Triangle = Cast<ATrianglePawn>(TriangleArray[0]);

}

void AGameOverActor::BackToMainMenuGame()
{
	FTimerHandle GameOverHandle;
	GetWorldTimerManager().SetTimer(GameOverHandle, this, &AGameOverActor::BackMenu, 0.1f, false, 1.15f);

	Gate->IsOpen = true;
	Gate->SoundWhenClose = false;
	Gate->SoundWhenOpen = false;
}

void AGameOverActor::RestartGame()
{
	FTimerHandle GameOverHandle;
	GetWorldTimerManager().SetTimer(GameOverHandle, this, &AGameOverActor::RestartTime, 0.1f, false, 1.15f);

	Gate->IsOpen = true;
	Gate->SoundWhenClose = false;
	Gate->SoundWhenOpen = false;
}

// Called every frame
void AGameOverActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameOverActor::BackMenu()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AGameOverActor::RestartTime()
{
	Triangle->SetActorHiddenInGame(false);
	Triangle->ConstSpeedOfJumpingUp = 20.f;
	Triangle->IsDead = false;
	Triangle->WhatKilledIt = 0;

	UpCamera->RestartUpCamera();

	Destroy();
}

