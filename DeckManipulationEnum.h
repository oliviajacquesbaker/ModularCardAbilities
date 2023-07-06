// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum EManipulationType : int
{
	M_None				UMETA(DisplayName = "No Manipulation"),
	M_Skip				UMETA(DisplayName = "Skip Card"),
	M_Shuffle			UMETA(DisplayName = "Reshuffle Deck"),
	M_ChangeUse			UMETA(DisplayName = "Change Card Use Amount"),
	M_Cooldown			UMETA(DisplayName = "Put on Cooldown"),
	M_CooldownSkip		UMETA(DisplayName = "Take off Cooldown")
};
