// Fill out your copyright notice in the Description page of Project Settings.


#include "UpCameraActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Sound/SoundCue.h"

#include "TrianglePawn.h"
#include "RectangleActor.h"
#include "TriangleJumpGameModeBase.h"
#include "EnemyActor.h"
#include "GateActor.h"
#include "StatsActor.h"

#define mes(writing) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, writing); //you can just write mes("String") to add information on screen 

// Sets default values
AUpCameraActor::AUpCameraActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UpBox = CreateDefaultSubobject<UBoxComponent>(TEXT("UpBox"));
	RootComponent = UpBox;

	DeleteBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DeleteBox"));
	DeleteBox->SetupAttachment(UpBox);

	Frame_Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame_Left"));
	Frame_Left->SetupAttachment(UpBox);

	Frame_Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame_Right"));
	Frame_Right->SetupAttachment(UpBox);

	Frame_Left_Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Frame_Left_Box"));
	Frame_Left_Box->SetupAttachment(UpBox);

	Frame_Right_Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Frame_Right_Box"));
	Frame_Right_Box->SetupAttachment(UpBox);

	Score = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Score"));
	Score->SetText(FText::AsNumber(Points));

	RecentScore = CreateDefaultSubobject<UTextRenderComponent>(TEXT("RecentScore"));
	RecentScore->SetText(FText::AsNumber(RecentPoints));

	HighScore = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HighScore"));
	HighScore->SetText(FText::AsNumber(HighPoints));

	UpBox->OnComponentBeginOverlap.AddDynamic(this, &AUpCameraActor::UpBoxBegin);
	UpBox->OnComponentEndOverlap.AddDynamic(this, &AUpCameraActor::UpBoxEnd);

	DeleteBox->OnComponentBeginOverlap.AddDynamic(this, &AUpCameraActor::DeleteBoxBegin);

	Frame_Left_Box->OnComponentBeginOverlap.AddDynamic(this, &AUpCameraActor::FrameLeftBegin);
	Frame_Right_Box->OnComponentBeginOverlap.AddDynamic(this, &AUpCameraActor::FrameRightBegin);

}

// Called when the game starts or when spawned
void AUpCameraActor::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATrianglePawn::StaticClass(), TriangleArray);
	Triangle = Cast<ATrianglePawn>(TriangleArray[0]);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), CameraActors);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATriangleJumpGameModeBase::StaticClass(), GameModeArray);
	GameMode = Cast<ATriangleJumpGameModeBase>(GameModeArray[0]);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGateActor::StaticClass(), GateArray);
	Gate = Cast<AGateActor>(GateArray[0]);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStatsActor::StaticClass(), StatsArray);
	Stats = Cast<AStatsActor>(StatsArray[0]);

	POP_IsEnemyGonnaSpawn = GameMode->POP_IsEnemyGonnaSpawn;
	POP_IsObstacleGonnaSpawn = GameMode->POP_IsObstacleGonnaSpawn;

	HighPoints = Stats->HighScore;
	RecentPoints = Stats->LastScore;

}

void AUpCameraActor::UpBoxBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (!OtherActor->ActorHasTag("Obstacle"))
	{
		InitialLocalizationOfTrianglePawn = Triangle->GetActorLocation();
		IsGoingUp = true;
	}
}

void AUpCameraActor::UpBoxEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor->ActorHasTag("Obstacle"))
	{
		IsGoingUp = false;
	}
}

void AUpCameraActor::DeleteBoxBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("Obstacle")) OtherActor->Destroy();
	else
	{
		if (Triangle->IsDead == false)
		{
			Triangle->WhatKilledIt = 2;
			Triangle->GameOverFunction();
		}
		if (Triangle->bEnemyKilled == true)
		{
			Triangle->WhatKilledIt = 1;
			Triangle->bEnemyKilled = false;
			Triangle->GameOverFunction();
		}
	}
}

void AUpCameraActor::FrameLeftBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (!OtherActor->ActorHasTag("Obstacle"))
	{
		Triangle->IsTeleporting = true;							// { This line will prevent crash 
		FVector Triangle_Loc = Triangle->GetActorLocation();	// 
		Triangle_Loc.X = 1230.f;								//		If player will go to close to the wall then he will teleport to other side
		Triangle->SetActorLocation(Triangle_Loc);				// 
		Triangle->IsTeleporting = false;						// }

		UGameplayStatics::PlaySound2D(this, TeleportSound);
	}
}

void AUpCameraActor::FrameRightBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (!OtherActor->ActorHasTag("Obstacle"))
	{
		Triangle->IsTeleporting = true;							// { This line will prevent crash 
		FVector Triangle_Loc = Triangle->GetActorLocation();	//
		Triangle_Loc.X = 50.f;									//	    If player will go to close to the wall then he will teleport to other side
		Triangle->SetActorLocation(Triangle_Loc);				//
		Triangle->IsTeleporting = false;						// } 

		UGameplayStatics::PlaySound2D(this, TeleportSound);
	}
}

// Called every frame
void AUpCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsGoingUp == true && Triangle->IsDead == false)
	{
		if (Triangle->IsFalling == false)
		{
			//float DifferenceZ = Triangle->GetActorLocation().Z - InitialLocalizationOfTrianglePawn.Z;
			FVector CameraLoc = CameraActors[0]->GetActorLocation();
			FVector ActorLoc = GetActorLocation();
			FVector GateLoc = Gate->GetActorLocation();
			
			Points += 2;

			CameraLoc.Z += Triangle->ConstSpeedOfJumpingUp;
			ActorLoc.Z += Triangle->ConstSpeedOfJumpingUp;
			GateLoc.Z += Triangle->ConstSpeedOfJumpingUp;

			CameraActors[0]->SetActorLocation(CameraLoc);
			SetActorLocation(ActorLoc);
			Gate->SetActorLocation(GateLoc);

			//if (POP_IsEnemyGonnaSpawn >= 45.f) POP_IsEnemyGonnaSpawn -= 0.0001f;
			if (POP_IsObstacleGonnaSpawn >= 15.f) { POP_IsObstacleGonnaSpawn -= 0.005f; POP_IsEnemyGonnaSpawn -= 0.0035f; }
			if (GameMode->POP_ObstacleIsNormal >= 25.f) GameMode->POP_ObstacleIsNormal -= 0.001f;
			if (GameMode->POP_ObstacleIsCloud <= 90.f) GameMode->POP_ObstacleIsCloud += 0.002f;
			if (GameMode->POP_ObstacleIsBroken > 95.f) GameMode->POP_ObstacleIsBroken -= 0.002f;
			if (GameMode->POP_ThatObstacleIsMoving <= 50.f) GameMode->POP_ThatObstacleIsMoving += 0.001f;
			if (GameMode->SpeedOfObstacleWhileMoving <= 1.5f) GameMode->SpeedOfObstacleWhileMoving += 0.0003f;
			if (GameMode->SpeedOfEnemy <= 1.5f) GameMode->SpeedOfEnemy += 0.0003f;
			if (GameMode->POP_IsThingGonnaSpawn >= 2.f) GameMode->POP_IsThingGonnaSpawn -= 0.0005f;
			if (GameMode->POP_IsSpringGonnaSpawn >= 30.f) GameMode->POP_IsSpringGonnaSpawn -= 0.001f; //80
			if (GameMode->POP_IsJetengineGonnaSpawn >= 65.f) GameMode->POP_IsJetengineGonnaSpawn -= 0.0006f; //95
			//if (GameMode->POP_IsJetpackGonnaSpawn <= 100.f) GameMode->POP_IsJetpackGonnaSpawn -= 0.0005f; //100
			

			//if (PercentOfObstacle >= 0.f) PercentOfObstacle -= 0.005f;
			FString HelpString = FString::FromInt(Points); //I had weird bug with font and this fixed it
			Score->SetText(HelpString);

			WhenSpawnObstacle += Triangle->ConstSpeedOfJumpingUp;
			//mes(FString::SanitizeFloat(WhenSpawnObstacle));
			if (WhenSpawnObstacle >= 100.f)
			{
				WhenSpawnObstacle = 0.f;
				SpawnObstacle();
			}
			
		}
	}
}

void AUpCameraActor::RestartUpCamera()
{
	SetActorHiddenInGame(false);

	StartedLocObstacle = 0.f;
	IsStarted = false;
	Points = 0.f;
	WhenSpawnObstacle = 0;
	TooFewEnemy = 3;
	TooFewObstacle = 0;
	FString HelpString = FString::FromInt(Points); //I had weird bug with font and this fixed it
	Score->SetText(HelpString);

	POP_IsObstacleGonnaSpawn = 55.f;
	POP_IsEnemyGonnaSpawn = 53.f;
	GameMode->POP_ObstacleIsNormal = 80.f;
	GameMode->POP_ObstacleIsCloud = 79.f;
	GameMode->POP_ObstacleIsBroken = 100.f;
	GameMode->POP_ThatObstacleIsMoving = 10.f;
	GameMode->SpeedOfObstacleWhileMoving = 1.1f;
	GameMode->SpeedOfEnemy = 1.2f;
	GameMode->POP_IsThingGonnaSpawn = 10.f;
	GameMode->POP_IsSpringGonnaSpawn = 50.f;
	GameMode->POP_IsJetengineGonnaSpawn = 80.f;

	RecentPoints = Stats->LastScore;
	FString ScoreString = FString::FromInt(RecentPoints); //I had weird bug with font and this fixed it
	RecentScore->SetText(ScoreString);

	HighPoints = Stats->HighScore;
	ScoreString = FString::FromInt(HighPoints); //I had weird bug with font and this fixed it
	HighScore->SetText(ScoreString);

	GameStarted();
}

void AUpCameraActor::SpawnObstacle()
{
	int IsObstacleGonnaSpawn = FMath::RandRange(1, 100); //if this Value is less ten 30 then obstacle will spawn

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector SpawnLoc = GetActorLocation();
	
	if (IsStarted == false)
	{
		SpawnLoc.Z -= 1000.f;
		StartedLocObstacle += 100.f;
		SpawnLoc.Z += StartedLocObstacle;
	}
	else
	{
		SpawnLoc.Z += 1000.f;
		
	}
	SpawnLoc.X = FMath::RandRange(170, 1020); //This value is deciding about where obstacle is going to spawn in X
	
	if (IsObstacleGonnaSpawn <= POP_IsObstacleGonnaSpawn || TooFewObstacle >= 3)
	{
		auto RectangleSpawned = GetWorld()->SpawnActor<AActor>(Rectangle, FTransform(SpawnLoc), SpawnParameters);

		auto RectangleSpawnedCast = Cast<ARectangleActor>(RectangleSpawned);
		if (RectangleSpawnedCast->WhichTypeObstacle == 1.f) TooFewObstacle = 0;
		else if (RectangleSpawnedCast->WhichTypeObstacle == 2.f) TooFewObstacle = 0;
		else TooFewObstacle++;
		if (TooFewObstacle >= 4 && RectangleSpawnedCast->WhichTypeObstacle == 3)
		{
			RectangleSpawnedCast->WhichTypeObstacle = 1;
			RectangleSpawnedCast->RectangleMesh->SetMaterial(0, RectangleSpawnedCast->LightBlueMaterial);
		}
		TooFewEnemy--;
	}
	else if (IsObstacleGonnaSpawn > POP_IsObstacleGonnaSpawn && IsObstacleGonnaSpawn <= POP_IsEnemyGonnaSpawn && TooFewEnemy <= 0)
	{
		auto EnemySpawned = GetWorld()->SpawnActor<AActor>(Enemy, FTransform(SpawnLoc), SpawnParameters);
		auto EnemySpawnedCast = Cast<AEnemyActor>(EnemySpawned);
		EnemySpawnedCast->SpeedOfEnemy = GameMode->SpeedOfEnemy;
		TooFewEnemy = 3;
	}
	else
	{
		TooFewObstacle += 1;
		TooFewEnemy--;
	}
	
}

void AUpCameraActor::GameStarted()
{
	for (int i = 1; i <= 20; i++)
	{
		SpawnObstacle();
		if (i >= 20) IsStarted = true;
	}
}

