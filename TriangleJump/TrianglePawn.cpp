// Fill out your copyright notice in the Description page of Project Settings.


#include "TrianglePawn.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Components/TextRenderComponent.h"
#include "Sound/SoundCue.h"

#include "UpCameraActor.h"
#include "StatsActor.h"
#include "MySaveGame.h"
#include "GateActor.h"
#include "EnemyActor.h"
#include "RectangleActor.h"

#define mes(writing) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, writing); //you can just write mes("String") to add information on screen 

// Sets default values
ATrianglePawn::ATrianglePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Triangle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Triangle"));
	RootComponent = Triangle;
}

// Called when the game starts or when spawned
void ATrianglePawn::BeginPlay()
{
	Super::BeginPlay();
	
	InitialConstSpeedOfJumpingUp = ConstSpeedOfJumpingUp;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), CameraActors);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStatsActor::StaticClass(), StatsArray);
	Stats = Cast<AStatsActor>(StatsArray[0]);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUpCameraActor::StaticClass(), UpCameraArray);
	UpCamera = Cast<AUpCameraActor>(UpCameraArray[0]);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGateActor::StaticClass(), GateArray);
	Gate = Cast<AGateActor>(GateArray[0]);

	RotTriangle = GetActorRotation();
}

// Called every frame
void ATrianglePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GameStarted == true)
	{
		if (IsPaused == false)
		{
			if (IsDead == false)
			{
				if (IsTeleporting == false)
				{
					FVector InitialLocPawn = GetActorLocation();
					if (ConstSpeedOfJumpingUp > 0.f)
					{
						InitialLocPawn.Z += ConstSpeedOfJumpingUp; //Speed of Jumping
						ConstSpeedOfJumpingUp -= LowerSpeedOfJumping;

						ConstSpeedOfFalling = ConstSpeedOfJumpingUp;
						IsFalling = false;
					}
					else
					{
						InitialLocPawn.Z -= ConstSpeedOfFalling; //Speed of falling down
						ConstSpeedOfFalling += HigherSpeedOfFalling;

						IsFalling = true;
					}
					SetActorLocation(InitialLocPawn);

				}
			}
			if (bEnemyKilled == true)
			{
				FVector InitialLocPawn = GetActorLocation();
				InitialLocPawn.Z -= ConstSpeedOfFalling; //Speed of falling down
				ConstSpeedOfFalling += HigherSpeedOfFalling;
				
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch += 3.f;

				SetActorLocation(InitialLocPawn);
				SetActorRotation(Rotation);
			}
		}
	}
}

// Called to bind functionality to input
void ATrianglePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Left", this, &ATrianglePawn::Move);
	PlayerInputComponent->BindAxis("Right", this, &ATrianglePawn::Move);

	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &ATrianglePawn::PauseKey);

}

void ATrianglePawn::GameBegins()
{
	GameStarted = true;
}

void ATrianglePawn::GameOverFunction()
{
	if (UpCamera->Points > Stats->HighScore)
	{
		Stats->HighScore = UpCamera->Points;
	}
	Stats->LastScore = UpCamera->Points;
	Stats->Attempts += 1;
	if (WhatKilledIt == 1) Stats->KilledByEnemy += 1;

	SaveGame();

	FTimerHandle GameOverHandle;
	GetWorldTimerManager().SetTimer(GameOverHandle, this, &ATrianglePawn::FunctionGameOver, 0.1f, false, 1.f);

	IsDead = true;

	Gate->IsOpen = true;
	Gate->SoundWhenClose = false;
	Gate->SoundWhenOpen = false;

	UGameplayStatics::PlaySound2D(this, GameOverSound);
}

void ATrianglePawn::KilledByEnemy()
{
	//Dodac tutaj aby Triangle po walnieciu w Enemy spadal w dol i sie jakos obrocil (zeby bylo widac ze zostal zabity przez Enemy)
	IsDead = true;
	bEnemyKilled = true;
}

void ATrianglePawn::Move(float Axis)
{
	if (GameStarted == true)
	{
		if (IsPaused == false)
		{
			if (IsDead == false)
			{
				if (Axis >= 0.f)
				{
					if (IsLeftBox == false)
					{
						FVector InitialLocPawn = GetActorLocation();
						InitialLocPawn.X += Axis;
						SetActorLocation(InitialLocPawn);
					}
				}
				else if (Axis < 0.f)
				{
					if (IsRightBox == false)
					{
						FVector InitialLocPawn = GetActorLocation();
						InitialLocPawn.X += Axis;
						SetActorLocation(InitialLocPawn);
					}
				}
			}
		}
	}
}

void ATrianglePawn::PauseKey()
{
	if (GameStarted == true)
	{
		if (IsPaused == false)
		{
			if (IsDead == false)
			{
				FTimerHandle TimeHandle;
				GetWorldTimerManager().SetTimer(TimeHandle, this, &ATrianglePawn::PauseTime, 0.1f, false, 1.15f);

				Gate->IsOpen = true;
				Gate->SoundWhenClose = false;
				Gate->SoundWhenOpen = false;

				IsPaused = true;
			}
		}
	}
}

void ATrianglePawn::ResumeGameObjects()
{
	IsPaused = false;
}

void ATrianglePawn::PauseTime()
{
	CamLoc = CameraActors[0]->GetActorLocation();
	CamLoc.X -= 27.49414;
	CamLoc.Y += 168.967468;
	CamLoc.Z += 128.028473;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	auto PauseSpawned = GetWorld()->SpawnActor<AActor>(Pause, FTransform(CamLoc), SpawnParameters);
}

void ATrianglePawn::SaveGame()
{
	//TWORZENIE ZAPISU
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	//ZAPISYWANIE RZECZY
	SaveGameInstance->Save_HighScore = Stats->HighScore;

	SaveGameInstance->Save_Seconds = Stats->Sekundy;
	SaveGameInstance->Save_Minutes = Stats->Minuty;
	SaveGameInstance->Save_Hours = Stats->Godziny;

	SaveGameInstance->Save_LastScore = Stats->LastScore;
	SaveGameInstance->Save_Attempts = Stats->Attempts;
	SaveGameInstance->Save_KilledByEnemy = Stats->KilledByEnemy;
	SaveGameInstance->Save_UsedSprings = Stats->UsedSprings;
	SaveGameInstance->Save_UsedJetEngines = Stats->UsedJetEngines;
	SaveGameInstance->Save_UsedJetpacks = Stats->UsedJetpacks;
	//ZAPISYWANIE SLOTU
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySlot"), 0);
}

void ATrianglePawn::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist("MySlot", 0) == true)
	{
		//TWORZENIE ZAPISU
		UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
		//WCZYTANIE SLOTU
		SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot("MySlot", 0));

		//WYWOLWANIE ZAPISANEJ INFORMACJI
		Stats->HighScore = SaveGameInstance->Save_HighScore;

		Stats->Sekundy = SaveGameInstance->Save_Seconds;
		Stats->Minuty = SaveGameInstance->Save_Minutes;
		Stats->Godziny = SaveGameInstance->Save_Hours;

		Stats->LastScore = SaveGameInstance->Save_LastScore;
		Stats->Attempts = SaveGameInstance->Save_Attempts;
		Stats->KilledByEnemy = SaveGameInstance->Save_KilledByEnemy;
		Stats->UsedSprings = SaveGameInstance->Save_UsedSprings;
		Stats->UsedJetEngines = SaveGameInstance->Save_UsedJetEngines;
		Stats->UsedJetpacks = SaveGameInstance->Save_UsedJetpacks;

		UpCamera->HighPoints = SaveGameInstance->Save_HighScore;
		UpCamera->RecentPoints = SaveGameInstance->Save_LastScore;
		FString HelpString = FString::FromInt(UpCamera->RecentPoints); //I had weird bug with font and this fixed it
		UpCamera->RecentScore->SetText(HelpString);
		HelpString = FString::FromInt(UpCamera->HighPoints); //I had weird bug with font and this fixed it
		UpCamera->HighScore->SetText(HelpString);
		Stats->FromIntToString();
	}
}

void ATrianglePawn::FunctionGameOver()
{
	UpCamera->SetActorHiddenInGame(true);
	SetActorHiddenInGame(true);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), ObstacleDelete);
	
	for (int i = 0; i != ObstacleDelete.Num(); i++)
	{
		if (ObstacleDelete[i]->ActorHasTag("Obstacle"))
		{
			ObstacleDelete[i]->Destroy();
		}
	}
	
	CamLoc = CameraActors[0]->GetActorLocation();
	CamLoc.X -= 27.49414;
	CamLoc.Y += 168.967468;
	CamLoc.Z += 128.028473;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	auto GameOverSpawned = GetWorld()->SpawnActor<AActor>(GameOver, FTransform(CamLoc), SpawnParameters);
	CamLoc.X -= 4.826294;
	CamLoc.Y = GetActorLocation().Y;
	CamLoc.Z -= 754.477097;
	SetActorLocation(CamLoc);
	SetActorRotation(RotTriangle);

}



