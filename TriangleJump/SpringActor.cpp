// Fill out your copyright notice in the Description page of Project Settings.


#include "SpringActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "TrianglePawn.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

#include "StatsActor.h"

#define mes(writing) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, writing); //you can just write mes("String") to add information on screen 

// Sets default values
ASpringActor::ASpringActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpringMesh"));
	RootComponent = SpringMesh;

	SpringBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpringBox"));
	SpringBox->SetupAttachment(RootComponent);

	SpringBox->OnComponentBeginOverlap.AddDynamic(this, &ASpringActor::SpringBoxBegin);

}

// Called when the game starts or when spawned
void ASpringActor::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATrianglePawn::StaticClass(), TriangleArray);
	Triangle = Cast<ATrianglePawn>(TriangleArray[0]);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStatsActor::StaticClass(), StatsArray);
	Stats = Cast<AStatsActor>(StatsArray[0]);

}

void ASpringActor::SpringBoxBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (!OtherActor->ActorHasTag("Obstacle"))
	{
		if (Triangle->IsFalling == true && Triangle->IsDead == false)
		{
			IsAnimSpring = true;
			Triangle->ConstSpeedOfJumpingUp = 36.f;
			Stats->UsedSprings += 1;
			IsMoving = false;

			UGameplayStatics::PlaySound2D(this, SpringSound);
		}
	}
}

// Called every frame
void ASpringActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Triangle->IsPaused == false)
	{
		if (IsMoving == true)
		{
			FVector SpringLocation = GetActorLocation();
			if (WhichWay == 1) SpringLocation.X -= SpeedOfSpringWhileMoving;
			else if (WhichWay == 2) SpringLocation.X += SpeedOfSpringWhileMoving;
			SetActorLocation(SpringLocation);
		}
	}
}

