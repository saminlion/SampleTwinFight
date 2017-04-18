// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "WemadeProject.h"
#include "WemadeProjectCharacter.h"
#include "FightEnemy.h"
#include "WemadeProjectGameMode.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "Engine.h"

AWemadeProjectCharacter::AWemadeProjectCharacter()
{
	TopDownCameraParent = CreateDefaultSubobject<USceneComponent>(TEXT("TopDownCameraParentParent"));
	TopDownCameraParent->SetupAttachment(GetCapsuleComponent());

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	EnemyDetectTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("EnemyDetectTriggger"));
	EnemyDetectTrigger->InitBoxExtent(FVector(5, 5, 5));
	EnemyDetectTrigger->SetupAttachment(TopDownCameraParent);

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(TopDownCameraParent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AWemadeProjectCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld* World = GetWorld())
			{
				FHitResult HitResult;
				FCollisionQueryParams Params;
				FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
				FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
}

void AWemadeProjectCharacter::DetectEnemy()
{
	/**Play Fight Animation choosen by Player Before Fight Start and Damage Impact*/
	auto FightGameMode = Cast<AWemadeProjectGameMode>(GetWorld()->GetAuthGameMode());

	if (FightGameMode)
	{
		bFightBegin = true;

		//this->PlayAnimMontage(AnimationPreset[FightGameMode->NumberOfFightMotion], 1.0f, "Defualt");

		//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Cyan, "Enemy Detect");
	}
}

void AWemadeProjectCharacter::LostEnemy()
{
	/**Play Fight Animation choosen by Player Before Fight Start and Damage Impact*/
	auto FightGameMode = Cast<AWemadeProjectGameMode>(GetWorld()->GetAuthGameMode());

	if (FightGameMode)
	{
		bFightBegin = false;

		//this->PlayAnimMontage(AnimationPreset[FightGameMode->NumberOfFightMotion], 1.0f, "Defualt");

		//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Cyan, "Enemy Detect");
	}
}

void AWemadeProjectCharacter::ApplyDamageToEnemy(float Damage)
{
	auto EnemyFunction = Cast<AFightEnemy>(Enemy);

	if (EnemyFunction)
	{
		EnemyFunction->GetDamage(PunchDamage);
	}
}
