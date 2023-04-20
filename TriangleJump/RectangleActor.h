// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RectangleActor.generated.h"

class UBoxComponent;

UCLASS()
class TRIANGLEJUMP_API ARectangleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARectangleActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* TopBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* RectangleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInstance* LightBlueMaterial;	//Original Obstacle

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInstance* BrownMaterial;   //Broken Obstacle

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInstance* WhiteMaterial;   //Cloud Obstacle

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMesh* BrokenRectangleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> Spring;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> Jetpack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> JetEngine;

	//SOUNDS
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* JumpSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* BrokenSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* CloudSound;

	//

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsObstacleFalling;

	//Overlaps boxes - Begin and End
	UFUNCTION()
		void TopBoxBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& Hit);
	///////

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	int WhichTypeObstacle; //1 - Normal, 2 - Cloud, 3 - Broken

	bool IsPaused;
private:
	TArray<AActor*> TriangleArray;
	class ATrianglePawn* Triangle;
	float WhatKindOfObstacle;
	float IsThingGonnaSpawn;

	float IsObstacleMoving;
	bool MovingObstacle;
	FVector ObstacleLocation;
	int WhichWayObstacleIsMoving; //if value is equel 0 then random way. if value is equel 1 then right. if value is equel 2 then left.
	class AJetEngineActor* JetEngineSpawnedCast;
	class AJetpackActor* JetpackSpawnedCast;
	class ASpringActor* SpringSpawnedCast;

	float WhatThingGonnaSpawn;

	TArray<AActor*> GameModeArray;
	class ATriangleJumpGameModeBase* GameMode;

	float SpeedOfObstacleWhileMoving = 1.1f;
	float POP_ThatObstacleIsMoving = 15.f; //POP - Percent Of Possibilty

	float POP_ObstacleIsNormal = 40.f;  //{
	float POP_ObstacleIsCloud = 80.f;   //
	float POP_ObstacleIsBroken = 100.f; //}

	float POP_IsThingGonnaSpawn = 30.f;

	float POP_IsSpringGonnaSpawn = 40.f;    //{
	float POP_IsJetengineGonnaSpawn = 80.f; //
	float POP_IsJetpackGonnaSpawn = 100.f;  //}
};
