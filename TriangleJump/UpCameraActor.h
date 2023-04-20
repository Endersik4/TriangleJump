// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UpCameraActor.generated.h"

UCLASS()
class TRIANGLEJUMP_API AUpCameraActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUpCameraActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* UpBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* DeleteBox; //This box will delete obstacle if it hit this

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Frame_Left;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Frame_Right;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* Frame_Left_Box; //If this box will collide with player then it will teleport him to Frame_Right_Box

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* Frame_Right_Box; //If this box will collide with player then it will teleport him to Frame_Left_Box

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UTextRenderComponent* Score;

	//Overlaps boxes - Begin and End
	UFUNCTION()
		void UpBoxBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& Hit);
	UFUNCTION()
		void UpBoxEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void DeleteBoxBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& Hit);

	UFUNCTION()
		void FrameLeftBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& Hit);

	UFUNCTION()
		void FrameRightBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& Hit);
	///////

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> Rectangle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> Enemy;

	TArray<AActor*> GameModeArray;
	class ATriangleJumpGameModeBase* GameMode;

	//SOUNDS
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* TeleportSound;
	//

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GameStarted();

	int Points; //Number of Poins in Score
	int HighPoints; //Highscore
	int RecentPoints; //Recent Score

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UTextRenderComponent* HighScore;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UTextRenderComponent* RecentScore;

	void RestartUpCamera();

private:
	FVector InitialLocalizationOfTrianglePawn;
	FVector FinalLocalizationOfTrianglePawn;

	TArray<AActor*> TriangleArray;
	class ATrianglePawn* Triangle;

	bool IsGoingUp;

	TArray<AActor*> CameraActors;

	void SpawnObstacle();

	TArray<AActor*> GateArray;
	class AGateActor* Gate;

	float PercentOfObstacle = 80.f;

	float POP_IsObstacleGonnaSpawn = 30.f;
	float POP_IsEnemyGonnaSpawn = 32.f;

	bool IsStarted;
	float StartedLocObstacle;

	TArray<AActor*> StatsArray;
	class AStatsActor* Stats;

	float WhenSpawnObstacle;
	int TooFewEnemy = 3;
	int TooFewObstacle; //Security that Player can always jump higher
};


