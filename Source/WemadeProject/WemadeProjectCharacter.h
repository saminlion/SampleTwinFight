// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "WemadeProjectCharacter.generated.h"

UCLASS(Blueprintable)
class AWemadeProjectCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AWemadeProjectCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EnemyDetect)
		class UBoxComponent* EnemyDetectTrigger;

	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter", meta = (DisplayName = "Enemy Damage Impact"))
		void ApplyDamageToEnemy(float Damage);

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* TopDownCameraParent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

protected:
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter", meta = (DisplayName = "Enemy Detect"))
		void DetectEnemy();

	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter", meta = (DisplayName = "Enemy Lost"))
		void LostEnemy();


	/*UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Ball Speed Log Show Off"))
		void BallLogOff();*/

	//UFUNCTION(BlueprintCallable, Category = "AI", meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))



	UAnimInstance* PlayerAnimInst;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FightDamage)
		float PunchDamage;

	UPROPERTY(BlueprintReadWrite)
		AActor* Enemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		bool bFightBegin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterAnimation)
		TArray<UAnimMontage*> AnimationPreset;
};

