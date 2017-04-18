// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "WemadeProject.h"
#include "WemadeProjectPlayerController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "WemadeProjectCharacter.h"
#include "Engine.h"

AWemadeProjectPlayerController::AWemadeProjectPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AWemadeProjectPlayerController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	InPawn = GetPawn();
}

void AWemadeProjectPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void AWemadeProjectPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AWemadeProjectPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AWemadeProjectPlayerController::OnSetDestinationReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AWemadeProjectPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AWemadeProjectPlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &AWemadeProjectPlayerController::OnResetVR);
}

void AWemadeProjectPlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AWemadeProjectPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (AWemadeProjectCharacter* MyPawn = Cast<AWemadeProjectCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UNavigationSystem::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint, Hit.GetActor());
		}
	}
}

void AWemadeProjectPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint, HitResult.GetActor());
	}
}

void AWemadeProjectPlayerController::SetNewMoveDestination(const FVector DestLocation, AActor* HitActor)
{
	APawn* const MyPawn = GetPawn();
	
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();

		// We need to issue move command only if far enough in order for walk animation to play correctly
		
		if (NavSys && (Distance > 120.0f) && HitActor->ActorHasTag("Enemy"))
		{
			NavSys->SimpleMoveToLocation(this, HitActor->GetActorLocation());
		}

		else if (NavSys && (Distance > 120.0f))
		{
			NavSys->SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void AWemadeProjectPlayerController::AttackKillEnemy()
{

}

void AWemadeProjectPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void AWemadeProjectPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}