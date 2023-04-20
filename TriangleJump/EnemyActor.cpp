// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "TrianglePawn.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

#define mes(writing) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, writing); //you can just write mes("String") to add information on screen 

// Sets default values
AEnemyActor::AEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	RootComponent = EnemyMesh;

	EnemyBox = CreateDefaultSubobject<UBoxComponent>(TEXT("EnemyBox"));
	EnemyBox->SetupAttachment(RootComponent);

	EnemyBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::EnemyBoxBegin);

}

// Called when the game starts or when spawned
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATrianglePawn::StaticClass(), TriangleArray);
	Triangle = Cast<ATrianglePawn>(TriangleArray[0]);

	float NewScale3D = FMath::RandRange(0.2f, 0.9f);
	SetActorScale3D(FVector(NewScale3D, NewScale3D, NewScale3D));
	Enemy_Z = GetActorLocation().Z;

	Left_Border_Max = FMath::RandRange(470.f, 1110.f);
	Right_Border_Max = FMath::RandRange(170.f, 470.f);
	
}

void AEnemyActor::EnemyBoxBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (!OtherActor->ActorHasTag("Obstacle") && !OtherActor->ActorHasTag("UpCamera"))
	{
		if (Triangle->IsOnJetpack == false && Triangle->IsDead == false)
		{
			UGameplayStatics::PlaySound2D(this, DeadByEnemySound);
			Triangle->KilledByEnemy();
		}
	}

	
}

// Called every frame
void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EnemyLocation = GetActorLocation();

	if (Triangle->IsPaused == false)
	{
		if (WhichWayObstacleIsMoving != 0)
		{
			if (EnemyLocation.X >= Left_Border_Max)
			{
				WhichWayObstacleIsMoving = 1; //right
			}
			else if (EnemyLocation.X <= Right_Border_Max)
			{
				WhichWayObstacleIsMoving = 2; //left
			}

			if (WhichWayObstacleIsMoving == 1)
			{
				EnemyLocation.X -= SpeedOfEnemy;
			}
			else if (WhichWayObstacleIsMoving == 2)
			{
				EnemyLocation.X += SpeedOfEnemy;
			}

			if (EnemyLocation.Z > Enemy_Z + 30.f) UpDown = true;
			else if (EnemyLocation.Z < Enemy_Z - 30.f) UpDown = false;

			if (UpDown == true) EnemyLocation.Z -= 1.2f;
			else if (UpDown == false) EnemyLocation.Z += 1.2f;
		}
		else
		{
			WhichWayObstacleIsMoving = FMath::RandRange(1, 2);
		}
		SetActorLocation(EnemyLocation);
	}
}

