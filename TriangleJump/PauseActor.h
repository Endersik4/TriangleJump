// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PauseActor.generated.h"

UCLASS()
class TRIANGLEJUMP_API APauseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APauseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UTextRenderComponent* Pause;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UTextRenderComponent* BackToMainMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* BackToMainMenuBox;

	UFUNCTION(BlueprintCallable)
		void BackToMainMenuGame();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UTextRenderComponent* Resume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* ResumeBox;

	UFUNCTION(BlueprintCallable)
		void ResumeGame();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void BackMenu();
	void ResumeTime();

	TArray<AActor*> GateArray;
	class AGateActor* Gate;

	TArray<AActor*> TriangleArray;
	class ATrianglePawn* Triangle;
};
