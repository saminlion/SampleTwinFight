// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "WemadeProjectGameMode.generated.h"

UCLASS(minimalapi)
class AWemadeProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AWemadeProjectGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerChooseFightMotion)
		int NumberOfFightMotion;
};



