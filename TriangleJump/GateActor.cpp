// Fill out your copyright notice in the Description page of Project Settings.


#include "GateActor.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

#include "Components/StaticMeshComponent.h"


#define mes(writing) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, writing); //you can just write mes("String") to add information on screen 

// Sets default values
AGateActor::AGateActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootGate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootGate"));
	RootComponent = RootGate;

	LeftGate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftGate"));
	LeftGate->SetupAttachment(RootComponent);

	RightGate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightGate"));
	RightGate->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGateActor::BeginPlay()
{
	Super::BeginPlay();
	
	InitialPositionLeftGate = LeftGate->GetRelativeLocation();
	InitialPositionRightGate = RightGate->GetRelativeLocation();

	FinishedPositionLeftGate = InitialPositionLeftGate;
	FinishedPositionLeftGate.Y -= 2039.f;

	FinishedPositionRightGate = InitialPositionRightGate;
	FinishedPositionRightGate.Y += 2039.f;

}

// Called every frame
void AGateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (SecondsElapsed <= 0.6f) SecondsElapsed += 1 * DeltaTime;

	if (SecondsElapsed >= 0.6f)
	{
		if (IsActive == true)
		{
			if (IsOpen == false)
			{
				LeftGate->SetRelativeLocation(FMath::VInterpTo(LeftGate->GetRelativeLocation(), FinishedPositionLeftGate, DeltaTime, SpeedOfInterpToOpen));
				RightGate->SetRelativeLocation(FMath::VInterpTo(RightGate->GetRelativeLocation(), FinishedPositionRightGate, DeltaTime, SpeedOfInterpToOpen));
				if (SoundWhenClose == false)
				{
					UGameplayStatics::PlaySound2D(this, GateSound);
					SoundWhenClose = true;
				}
			}
			else if (IsOpen == true)
			{
				LeftGate->SetRelativeLocation(FMath::VInterpTo(LeftGate->GetRelativeLocation(), InitialPositionLeftGate, DeltaTime, SpeedOfInterpToClose));
				RightGate->SetRelativeLocation(FMath::VInterpTo(RightGate->GetRelativeLocation(), InitialPositionRightGate, DeltaTime, SpeedOfInterpToClose));
				if (LeftGate->GetRelativeLocation().Y >= -5.f) IsOpen = false;
				if (SoundWhenOpen == false)
				{
					UGameplayStatics::PlaySound2D(this, GateSound);
					SoundWhenOpen = true;
				}
			}
		}
	}
	
}

