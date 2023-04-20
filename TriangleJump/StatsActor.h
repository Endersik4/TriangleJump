// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StatsActor.generated.h"

class UTextRenderComponent;

UCLASS()
class TRIANGLEJUMP_API AStatsActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStatsActor();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UTextRenderComponent* Stats;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UTextRenderComponent* Back;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		class UBoxComponent* BackBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UTextRenderComponent* Val_HighScore;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UTextRenderComponent* Val_Time;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UTextRenderComponent* Val_LastScore;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UTextRenderComponent* Val_Attempts;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UTextRenderComponent* Val_KilledByEnemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UTextRenderComponent* Val_UsedSprings;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UTextRenderComponent* Val_UsedJetEngines;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UTextRenderComponent* Val_UsedJetpacks;

	UFUNCTION(BlueprintCallable)
		void BackFunction();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int HighScore;
	
	int Sekundy;
	int Minuty;
	int Godziny;

	int LastScore;
	int Attempts;
	int KilledByEnemy;
	int UsedSprings;
	int UsedJetEngines;
	int UsedJetpacks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsStatsHidden = true;

	void FromIntToString();
private:
	TArray<AActor*> GateArray;
	class AGateActor* Gate;

	TArray<AActor*> MainMenuArray;
	class AMainMenu* MainMenu;

	void BackToMenu();

	float Time;
	FString SekundyString;
	FString MinutyString;
	FString GodzinyString;
	FString TimeString;
	

};
