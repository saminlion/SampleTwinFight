// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "WemadeProjectPlayerController.generated.h"

UCLASS()
class AWemadeProjectPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AWemadeProjectPlayerController();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	void Possess(APawn * InPawn);

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location and when it hits enemy attack sequence begin */
	void SetNewMoveDestination(const FVector DestLocation, AActor* HitActor);

	/** Attack enemy until it get futher away and when enemy killed character stop*/
	void AttackKillEnemy();

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
};


