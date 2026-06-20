// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "EFT_EnemyAITypes.generated.h"

UENUM(BlueprintType)
enum class EEFTEnemyAIState : uint8
{
	Idle,
	Patrol,
	Investigate,
	Chase,
	Attack,
	HitReact,
	Dead
};

UENUM(BlueprintType)
enum class EEFTEnemyDecision : uint8
{
	None,
	Patrol,
	Investigate,
	Chase,
	Attack,
	Wait
};
