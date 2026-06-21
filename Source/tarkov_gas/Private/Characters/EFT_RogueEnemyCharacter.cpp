// YeaSakura123 2025.11

#include "Characters/EFT_RogueEnemyCharacter.h"

AEFT_RogueEnemyCharacter::AEFT_RogueEnemyCharacter()
{
	EnemyType = EEFTEnemyType::Rogue;
	AcceptanceRadius = 450.f;
	AttackRange = 1400.f;
	SightRadius = 2600.f;
	LoseSightSeconds = 4.f;
	PatrolRadius = 1400.f;
	AttackCooldown = 0.75f;
	LostSightSearchRadius = 700.f;
	LostSightSearchObserveDuration = 1.5f;
	LostSightHoldAngleDuration = 3.f;
	MaxLostSightSearches = 2;
}
