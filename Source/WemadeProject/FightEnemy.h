// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "FightEnemy.generated.h"

UCLASS()
class WEMADEPROJECT_API AFightEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFightEnemy();

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* Defualt;

	void GetDamage(float damage);

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterValue)
	float health;
};
