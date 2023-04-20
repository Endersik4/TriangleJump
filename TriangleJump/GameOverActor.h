// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameOverActor.generated.h"

UCLASS()
class TRIANGLEJUMP_API AGameOverActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameOverActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UTextRenderComponent* GameOver;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UTextRenderComponent* BackToMainMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* BackToMainMenuBox;

	UFUNCTION(BlueprintCallable)
		void BackToMainMenuGame();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UTextRenderComponent* Restart;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* RestartBox;

	UFUNCTION(BlueprintCallable)
		void RestartGame();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void BackMenu();
	void RestartTime();

	TArray<AActor*> GateArray;
	class AGateActor* Gate;

	TArray<AActor*> TriangleArray;
	class ATrianglePawn* Triangle;
	TArray<AActor*> UpCameraArray;
	class AUpCameraActor* UpCamera;
};
