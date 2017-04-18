// Fill out your copyright notice in the Description page of Project Settings.

#include "WemadeProject.h"
#include "FightEnemy.h"


// Sets default values
AFightEnemy::AFightEnemy()
{


}

void AFightEnemy::GetDamage(float damage)
{
	health -= damage;
}
