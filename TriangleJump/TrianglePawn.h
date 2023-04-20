// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TrianglePawn.generated.h"

UCLASS()
class TRIANGLEJUMP_API ATrianglePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATrianglePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//SOUNDS
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* GameOverSound;
	//

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Triangle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> GameOver;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> Pause;

	bool IsLeftBox;
	bool IsRightBox;

	//Constant value of Triangle speed Up 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics of Triangle")
		float ConstSpeedOfJumpingUp; 
	//Constant value of Triangle falling down
	float ConstSpeedOfFalling; 
	//This value will smoothly decreasing Const Speed Of Jumping Up
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics of Triangle")
		float LowerSpeedOfJumping; 
	//This value will smoothly rising Const Speed Of Falling Down
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics of Triangle")
		float HigherSpeedOfFalling; 

	float InitialConstSpeedOfJumpingUp;
	bool IsFalling;
	bool IsTeleporting;
	bool IsSpring;

	bool IsOnJetpack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsDead;

	void GameOverFunction();

	bool GameStarted;
	void GameBegins();

	int WhatKilledIt; //1 - Enemy, 2 - DeleteBox

	void SaveGame();
	void LoadGame();

	void ResumeGameObjects();
	bool IsPaused;

	void KilledByEnemy();
	bool bEnemyKilled;
	float SpeedOfRotatingKill = 50.f;
	FRotator RotTriangle;

private:
	void Move(float Axis);

	void PauseKey();
	void PauseTime();

	TArray<AActor*> StatsArray;
	class AStatsActor* Stats;
	TArray<AActor*> UpCameraArray;
	class AUpCameraActor* UpCamera;

	TArray<AActor*> CameraActors;
	TArray<AActor*> ObstacleDelete;

	TArray<AActor*> GateArray;
	class AGateActor* Gate;

	void FunctionGameOver();

	FVector CamLoc;
};
