// Fill out your copyright notice in the Description page of Project Settings.


#include "RectangleActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

#include "TriangleJumpGameModeBase.h"
#include "TrianglePawn.h"
#include "JetEngineActor.h"
#include "JetpackActor.h"
#include "SpringActor.h"

#define mes(writing) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, writing); //you can just write mes("String") to add information on screen 

// Sets default values
ARectangleActor::ARectangleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RectangleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RectangleMesh"));
	RootComponent = RectangleMesh;

	TopBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TopBox"));
	TopBox->SetupAttachment(RectangleMesh);

	TopBox->SetGenerateOverlapEvents(true);

	TopBox->OnComponentBeginOverlap.AddDynamic(this, &ARectangleActor::TopBoxBegin);

}

// Called when the game starts or when spawned
void ARectangleActor::BeginPlay()
{
	Super::BeginPlay();

	//
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATriangleJumpGameModeBase::StaticClass(), GameModeArray);
	GameMode = Cast<ATriangleJumpGameModeBase>(GameModeArray[0]);

	SpeedOfObstacleWhileMoving = GameMode->SpeedOfObstacleWhileMoving;
	POP_ThatObstacleIsMoving = GameMode->POP_ThatObstacleIsMoving; //POP - Percent Of Possibilty

	POP_ObstacleIsNormal = GameMode->POP_ObstacleIsNormal;  //{
	POP_ObstacleIsCloud = GameMode->POP_ObstacleIsCloud;   //
	POP_ObstacleIsBroken = GameMode->POP_ObstacleIsBroken; //}

	POP_IsThingGonnaSpawn = GameMode->POP_IsThingGonnaSpawn;

	POP_IsSpringGonnaSpawn = GameMode->POP_IsSpringGonnaSpawn;    //{
	POP_IsJetengineGonnaSpawn = GameMode->POP_IsJetengineGonnaSpawn; //
	POP_IsJetpackGonnaSpawn = GameMode->POP_IsJetpackGonnaSpawn;  //}
	//

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATrianglePawn::StaticClass(), TriangleArray);
	Triangle = Cast<ATrianglePawn>(TriangleArray[0]);

	IsObstacleMoving = FMath::RandRange(1, 100);
	if (IsObstacleMoving <= POP_ThatObstacleIsMoving) MovingObstacle = true;

	WhatKindOfObstacle = FMath::RandRange(1, 100);
	if (WhatKindOfObstacle <= POP_ObstacleIsNormal) WhichTypeObstacle = 1;
	else if (WhatKindOfObstacle > POP_ObstacleIsNormal && WhatKindOfObstacle <= POP_ObstacleIsCloud) WhichTypeObstacle = 2;
	else if (WhatKindOfObstacle > POP_ObstacleIsCloud && WhatKindOfObstacle <= POP_ObstacleIsBroken) WhichTypeObstacle = 3;

	if (WhichTypeObstacle == 1)
	{
		RectangleMesh->SetMaterial(0, LightBlueMaterial);
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FVector SpawnLoc = GetActorLocation();

		IsThingGonnaSpawn = FMath::RandRange(1, 100);
		if (IsThingGonnaSpawn <= POP_IsThingGonnaSpawn)
		{
			WhatThingGonnaSpawn = FMath::RandRange(1, 100);
			if (WhatThingGonnaSpawn <= POP_IsSpringGonnaSpawn)
			{
				SpawnLoc.Z += 47.5f;
				SpawnLoc.Y -= 57.41f;
				float FirstRange = SpawnLoc.X + 69.65f;
				float SecondRange = SpawnLoc.X - 68.65f;
				SpawnLoc.X = FMath::RandRange(FirstRange, SecondRange); //This value is deciding about where obstacle is going to spawn in X
				auto SpringSpawned = GetWorld()->SpawnActor<AActor>(Spring, FTransform(SpawnLoc), SpawnParameters);
				SpringSpawnedCast = Cast<ASpringActor>(SpringSpawned);
				SpringSpawnedCast->SpeedOfSpringWhileMoving = SpeedOfObstacleWhileMoving;
			}
			else if (WhatThingGonnaSpawn > POP_IsSpringGonnaSpawn && WhatThingGonnaSpawn <= POP_IsJetengineGonnaSpawn)
			{
				SpawnLoc.Z += 52.722954f;
				SpawnLoc.Y -= 28.561432f;
				float FirstRange = SpawnLoc.X + 41.358948f;
				float SecondRange = SpawnLoc.X - 46.108399f;
				SpawnLoc.X = FMath::RandRange(FirstRange, SecondRange); //This value is deciding about where obstacle is going to spawn in X
				auto JetEngineSpawned = GetWorld()->SpawnActor<AActor>(JetEngine, FTransform(SpawnLoc), SpawnParameters);
				JetEngineSpawnedCast = Cast<AJetEngineActor>(JetEngineSpawned);
				JetEngineSpawnedCast->SpeedOfJetEngineWhileMoving = SpeedOfObstacleWhileMoving;
			}
			else if (WhatThingGonnaSpawn > POP_IsJetengineGonnaSpawn && WhatThingGonnaSpawn <= POP_IsJetpackGonnaSpawn)
			{
				SpawnLoc.Z += 67.86145f;
				SpawnLoc.Y -= 57.856384f;
				float FirstRange = SpawnLoc.X + 27.14563f;
				float SecondRange = SpawnLoc.X - 30.604737f;
				SpawnLoc.X = FMath::RandRange(FirstRange, SecondRange); //This value is deciding about where obstacle is going to spawn in X
				auto JetpackSpawned = GetWorld()->SpawnActor<AActor>(Jetpack, FTransform(SpawnLoc), SpawnParameters);
				JetpackSpawnedCast = Cast<AJetpackActor>(JetpackSpawned);
				JetpackSpawnedCast->SpeedOfJetpackWhileMoving = SpeedOfObstacleWhileMoving;
			}
		}
	}
	else if (WhichTypeObstacle == 2)
	{
		RectangleMesh->SetMaterial(0, WhiteMaterial);
	}
	else if (WhichTypeObstacle == 3)
	{
		RectangleMesh->SetMaterial(0, BrownMaterial);
	}
}

// Called every frame
void ARectangleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Triangle->IsPaused == false)
	{
		if (IsObstacleFalling == true)
		{
			FVector ChangeObstacleLoc = GetActorLocation();
			ChangeObstacleLoc.Z -= 7.f;
			SetActorLocation(ChangeObstacleLoc);
		}

		if (MovingObstacle == true)
		{
			ObstacleLocation = GetActorLocation();

			if (WhichWayObstacleIsMoving != 0)
			{
				if (ObstacleLocation.X >= 1110.f)
				{
					WhichWayObstacleIsMoving = 1; //right
				}
				else if (ObstacleLocation.X <= 170.f)
				{
					WhichWayObstacleIsMoving = 2; //left
				}

				if (WhatThingGonnaSpawn <= POP_IsSpringGonnaSpawn)
				{
					if (SpringSpawnedCast)
					{
						SpringSpawnedCast->IsMoving = true;
						SpringSpawnedCast->WhichWay = WhichWayObstacleIsMoving;
					}
				}
				else if (WhatThingGonnaSpawn > POP_IsSpringGonnaSpawn && WhatThingGonnaSpawn <= POP_IsJetengineGonnaSpawn)
				{
					if (JetEngineSpawnedCast)
					{
						JetEngineSpawnedCast->IsMoving = true;
						JetEngineSpawnedCast->WhichWay = WhichWayObstacleIsMoving;
					}
				}
				else if (WhatThingGonnaSpawn > POP_IsJetengineGonnaSpawn && WhatThingGonnaSpawn <= POP_IsJetpackGonnaSpawn)
				{
					if (JetpackSpawnedCast)
					{
						JetpackSpawnedCast->IsMoving = true;
						JetpackSpawnedCast->WhichWay = WhichWayObstacleIsMoving;
					}
				}

				if (WhichWayObstacleIsMoving == 1)
				{
					ObstacleLocation.X -= SpeedOfObstacleWhileMoving;
				}
				else if (WhichWayObstacleIsMoving == 2)
				{
					ObstacleLocation.X += SpeedOfObstacleWhileMoving;
				}
			}
			else
			{
				WhichWayObstacleIsMoving = FMath::RandRange(1, 2);
			}
			SetActorLocation(ObstacleLocation);
		}
	}

}

void ARectangleActor::TopBoxBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& Hit)
{
	if (!OtherActor->ActorHasTag("Obstacle"))
	{
		if (Triangle->IsFalling == true && Triangle->IsDead == false)
		{
			
			if (WhichTypeObstacle == 3)
			{
				UGameplayStatics::PlaySound2D(this, BrokenSound);
				IsObstacleFalling = true;
			}
			else
			{
				Triangle->ConstSpeedOfJumpingUp = Triangle->InitialConstSpeedOfJumpingUp;
				if (WhichTypeObstacle == 2)
				{
					UGameplayStatics::PlaySound2D(this, CloudSound);
					Destroy();
				}
				else
				{
					UGameplayStatics::PlaySound2D(this, JumpSound);
				}
			}
			
		}
	}
}


