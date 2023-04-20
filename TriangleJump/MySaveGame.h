// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TRIANGLEJUMP_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UMySaveGame();

	UPROPERTY()
		int Save_HighScore;
	UPROPERTY()
		int Save_Seconds;
	UPROPERTY()
		int Save_Minutes;
	UPROPERTY()
		int Save_Hours;
	UPROPERTY()
		int Save_LastScore;
	UPROPERTY()
		int Save_Attempts;
	UPROPERTY()
		int Save_KilledByEnemy;
	UPROPERTY()
		int Save_UsedSprings;
	UPROPERTY()
		int Save_UsedJetEngines;
	UPROPERTY()
		int Save_UsedJetpacks;
	
};
