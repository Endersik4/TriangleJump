// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JetpackActor.generated.h"

UCLASS()
class TRIANGLEJUMP_API AJetpackActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AJetpackActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* JetpackMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* JetpackBox;

	//Overlap box - Begin 
	UFUNCTION()
		void JetpackBoxBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& Hit);
	////
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsJetpackOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool FallingDown;

	//SOUNDS
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* JetpackSound;
	//

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsMoving;
	int WhichWay;
	float SpeedOfJetpackWhileMoving = 1.1f;

private:
	TArray<AActor*> TriangleArray;
	class ATrianglePawn* Triangle;
	
	FVector NewJetpackLocation;

	TArray<AActor*> StatsArray;
	class AStatsActor* Stats;

	float DownSpeed;

	
};