// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainMenu.generated.h"

class UTextRenderComponent;
class UBoxComponent;

UCLASS()
class TRIANGLEJUMP_API AMainMenu : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMainMenu();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UTextRenderComponent* TriangleJumpMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UTextRenderComponent* Start;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UTextRenderComponent* Stats;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UTextRenderComponent* Quit;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UBoxComponent* StartBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UBoxComponent* StatsBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
		UBoxComponent* QuitBox;

	UFUNCTION(BlueprintCallable)
		void StartFunction();
	UFUNCTION(BlueprintCallable)
		void StatsFunction();
	UFUNCTION(BlueprintCallable)
		void QuitFunction();

	//SOUNDS
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* StartSound;

	//

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsMainHidden;

private:
	TArray<AActor*> UpCameraArray;
	class AUpCameraActor* UpCamera;

	TArray<AActor*> TriangleArray;
	class ATrianglePawn* Triangle;

	TArray<AActor*> GateArray;
	class AGateActor* Gate;

	TArray<AActor*> StatsArray;
	class AStatsActor* StatsActor;

	void StartGame();
	void StatsGame();


};
