// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JetEngineActor.generated.h"

UCLASS()
class TRIANGLEJUMP_API AJetEngineActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJetEngineActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* JetEngineMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* JetEngineBox;

	//Overlap box - Begin 
	UFUNCTION()
		void JetEngineBoxBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& Hit);
	////

	//SOUNDS
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* JetEngineSound;
	//

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsMoving;
	int WhichWay;
	float SpeedOfJetEngineWhileMoving = 1.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsJetEngineOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool FallingDown;

private:
	TArray<AActor*> TriangleArray;
	class ATrianglePawn* Triangle;
	FVector NewJetEngineLocation;

	TArray<AActor*> StatsArray;
	class AStatsActor* Stats;

	float DownSpeed;
	
};
