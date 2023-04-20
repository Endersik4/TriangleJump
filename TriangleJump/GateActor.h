// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GateActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class TRIANGLEJUMP_API AGateActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGateActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsOpen;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* RootGate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* LeftGate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* RightGate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SpeedOfInterpToClose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SpeedOfInterpToOpen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsActive;

	//SOUNDS
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			class USoundCue* GateSound;
	//


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	bool SoundWhenClose;
	bool SoundWhenOpen;
private:
	
	FVector InitialPositionLeftGate;
	FVector InitialPositionRightGate;

	FVector FinishedPositionLeftGate;
	FVector FinishedPositionRightGate;

	float SecondsElapsed;
};
