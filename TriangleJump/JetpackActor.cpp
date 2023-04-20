// Fill out your copyright notice in the Description page of Project Settings.


#include "JetpackActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "TrianglePawn.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

#include "StatsActor.h"

// Sets default values
AJetpackActor::AJetpackActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	JetpackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JetpackMesh"));
	RootComponent = JetpackMesh;

	JetpackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("JetpackBox"));
	JetpackBox->SetupAttachment(RootComponent);

	JetpackBox->OnComponentBeginOverlap.AddDynamic(this, &AJetpackActor::JetpackBoxBegin);

}

// Called when the game starts or when spawned
void AJetpackActor::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATrianglePawn::StaticClass(), TriangleArray);
	Triangle = Cast<ATrianglePawn>(TriangleArray[0]);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStatsActor::StaticClass(), StatsArray);
	Stats = Cast<AStatsActor>(StatsArray[0]);

}

void AJetpackActor::JetpackBoxBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (!OtherActor->ActorHasTag("Obstacle"))
	{
		if (FallingDown == false)
		{
			if (Triangle->IsFalling == true && IsJetpackOn == false && Triangle->IsDead == false)
			{
				NewJetpackLocation = Triangle->GetActorLocation();
				NewJetpackLocation.Y -= 5.785278f;
				NewJetpackLocation.Z -= 61.84777f;
				SetActorLocation(NewJetpackLocation);
				Triangle->ConstSpeedOfJumpingUp = 80.f;
				IsJetpackOn = true;
				IsMoving = false;
				Triangle->IsOnJetpack = true;

				Stats->UsedJetpacks += 1;

				UGameplayStatics::PlaySound2D(this, JetpackSound);
			}
		}
	}
}

// Called every frame
void AJetpackActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Triangle->IsPaused == false)
	{
		if (IsJetpackOn == true)
		{
			NewJetpackLocation.Z += Triangle->ConstSpeedOfJumpingUp;
			NewJetpackLocation.X = Triangle->GetActorLocation().X + 0.049072f;
			SetActorLocation(NewJetpackLocation);
			if (Triangle->ConstSpeedOfJumpingUp <= 5.f)
			{
				Triangle->IsOnJetpack = false;
				//Destroy();
				FallingDown = true;
				IsJetpackOn = false;
			}
		}

		if (FallingDown == true)
		{
			FVector AcLoc = GetActorLocation();
			DownSpeed += 0.2f;
			AcLoc.Z -= DownSpeed;

			SetActorLocation(AcLoc);
		}

		if (IsMoving == true)
		{
			FVector JetpackLocation = GetActorLocation();
			if (WhichWay == 1) JetpackLocation.X -= SpeedOfJetpackWhileMoving;
			else if (WhichWay == 2) JetpackLocation.X += SpeedOfJetpackWhileMoving;
			SetActorLocation(JetpackLocation);
		}
	}
}

