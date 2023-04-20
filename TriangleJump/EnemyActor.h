// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyActor.generated.h"

UCLASS()
class TRIANGLEJUMP_API AEnemyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* EnemyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* EnemyBox;

	//Overlap box - Begin 
	UFUNCTION()
		void EnemyBoxBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& Hit);
	////

		//SOUNDS
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* DeadByEnemySound;
	//

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float SpeedOfEnemy = 1.5f;

private:
	FVector EnemyLocation;
	float Enemy_Z;
	int WhichWayObstacleIsMoving;
	bool UpDown;
	float Left_Border_Max;
	float Right_Border_Max;

	TArray<AActor*> TriangleArray;
	class ATrianglePawn* Triangle;


};
