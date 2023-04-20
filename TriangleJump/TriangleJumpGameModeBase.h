// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TriangleJumpGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TRIANGLEJUMP_API ATriangleJumpGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATriangleJumpGameModeBase();

	//szansa ze pojawi się RectangleActor (co każdy segment)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level of Difficulty")
		float POP_IsObstacleGonnaSpawn;
	
	//szansa ze pojawi się Enemy zamaist RectangleActor jak nic sie nie zrespi to dodaje +1 do TooFewObstacle co powoduje ze jezeli 3 razy z rzedu nic nie zrespi to nastepnym razem NA PEWNO cos sie zrespi (aby dalo sie grac w nieskonczonosc)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level of Difficulty")
		float POP_IsEnemyGonnaSpawn;

	//szansa czy dany rectangle ma sie poruszac
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level of Difficulty")
		float POP_ThatObstacleIsMoving;

	//szansa ze bedzie to normalny prostokat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level of Difficulty")
		float POP_ObstacleIsNormal;
	
	//szansa ze bedzie to "chumra" prostokat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level of Difficulty")
		float POP_ObstacleIsCloud;

	//szansa ze bedzie to fejkowy prostokat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level of Difficulty")
		float POP_ObstacleIsBroken;

	//czy jakis obiekt ma sie zrespic na rectangle (np. Spring, Jetpack lub Jetengine)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level of Difficulty")
		float POP_IsThingGonnaSpawn;

	//szansa ze Spring sie zrespi
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level of Difficulty")
		float POP_IsSpringGonnaSpawn;

	//szansa ze Jetengine sie zrespi
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level of Difficulty")
		float POP_IsJetengineGonnaSpawn;

	//szansa ze jetpack sie zrespi
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level of Difficulty")
		float POP_IsJetpackGonnaSpawn;

	//predkosc jaka porusza sie Enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level of Difficulty")
		float SpeedOfEnemy;

	//predkosc jetpacka, sprezyny itd w momencie gdy platforma sie porusza
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level of Difficulty")
		float SpeedOfObstacleWhileMoving;

protected:

	
};
