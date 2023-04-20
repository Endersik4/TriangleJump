// Copyright Epic Games, Inc. All Rights Reserved.


#include "TriangleJumpGameModeBase.h"

#include "Kismet/GameplayStatics.h"

#include "EnemyActor.h"
#include "RectangleActor.h"
#include "UpCameraActor.h"

#define mes(writing) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, writing); //you can just write mes("String") to add information on screen 

ATriangleJumpGameModeBase::ATriangleJumpGameModeBase()
{

}
