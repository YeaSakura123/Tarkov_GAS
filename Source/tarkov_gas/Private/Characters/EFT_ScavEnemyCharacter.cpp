// YeaSakura123 2025.11

#include "Characters/EFT_ScavEnemyCharacter.h"

AEFT_ScavEnemyCharacter::AEFT_ScavEnemyCharacter()
{
	EnemyType = EEFTEnemyType::Scav;
	AcceptanceRadius = 350.f;
	AttackRange = 900.f;
	SightRadius = 1800.f;
	LoseSightSeconds = 2.5f;
	PatrolRadius = 1000.f;
	AttackCooldown = 1.4f;
	LostSightSearchRadius = 450.f;
	LostSightSearchObserveDuration = 1.2f;
	LostSightHoldAngleDuration = 1.8f;
	MaxLostSightSearches = 1;
}
