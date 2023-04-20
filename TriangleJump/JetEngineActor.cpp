// Fill out your copyright notice in the Description page of Project Settings.


#include "JetEngineActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "TrianglePawn.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

#include "StatsActor.h"

#define mes(writing) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, writing); //you can just write mes("String") to add information on screen 

// Sets default values
AJetEngineActor::AJetEngineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	JetEngineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JetEngineMesh"));
	RootComponent = JetEngineMesh;

	JetEngineBox = CreateDefaultSubobject<UBoxComponent>(TEXT("JetEngineBox"));
	JetEngineBox->SetupAttachment(RootComponent);

	JetEngineBox->OnComponentBeginOverlap.AddDynamic(this, &AJetEngineActor::JetEngineBoxBegin);
}

// Called when the game starts or when spawned
void AJetEngineActor::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATrianglePawn::StaticClass(), TriangleArray);
	Triangle = Cast<ATrianglePawn>(TriangleArray[0]);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStatsActor::StaticClass(), StatsArray);
	Stats = Cast<AStatsActor>(StatsArray[0]);
}

void AJetEngineActor::JetEngineBoxBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (!OtherActor->ActorHasTag("Obstacle"))
	{
		if (FallingDown == false)
		{
			if (Triangle->IsFalling == true && IsJetEngineOn == false && Triangle->IsDead == false)
			{
				NewJetEngineLocation = Triangle->GetActorLocation();
				NewJetEngineLocation.Y -= 38.595215f;
				NewJetEngineLocation.Z -= 101.505973f;
				SetActorLocation(NewJetEngineLocation);

				FRotator NewRot = GetActorRotation();
				NewRot.Pitch += 180.f;
				SetActorRotation(NewRot);

				Triangle->ConstSpeedOfJumpingUp = 60.f;
				IsJetEngineOn = true;
				IsMoving = false;
				Triangle->IsOnJetpack = true;

				Stats->UsedJetEngines += 1;

				UGameplayStatics::PlaySound2D(this, JetEngineSound);
			}
		}
	}
}

// Called every frame
void AJetEngineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Triangle->IsPaused == false)
	{
		if (IsJetEngineOn == true)
		{
			NewJetEngineLocation.Z += Triangle->ConstSpeedOfJumpingUp;
			NewJetEngineLocation.X = Triangle->GetActorLocation().X;
			SetActorLocation(NewJetEngineLocation);
			if (Triangle->ConstSpeedOfJumpingUp <= 5.f)
			{
				Triangle->IsOnJetpack = false;
				IsJetEngineOn = false;
				FallingDown = true;
			}
		}

		if (FallingDown == true)
		{
			FVector AcLoc = GetActorLocation();
			DownSpeed += 0.3f;
			AcLoc.Z -= DownSpeed;

			SetActorLocation(AcLoc);
		}

		if (IsMoving == true)
		{
			FVector JetEngineLocation = GetActorLocation();
			if (WhichWay == 1)
			{
				JetEngineLocation.X -= SpeedOfJetEngineWhileMoving;
			}
			else if (WhichWay == 2)
			{
				JetEngineLocation.X += SpeedOfJetEngineWhileMoving;
			}
			SetActorLocation(JetEngineLocation);
		}
	}
}

