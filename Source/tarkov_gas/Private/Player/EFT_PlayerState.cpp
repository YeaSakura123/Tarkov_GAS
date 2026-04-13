// YeaSakura123 2025.11

#include "Player/EFT_PlayerState.h"
#include "AbilitySystem/EFT_AbilitySystemComponent.h"
#include "AbilitySystem/EFT_AttributeSet.h"

AEFT_PlayerState::AEFT_PlayerState()
{
	SetNetUpdateFrequency(100.f);

	AbilitySystemComponent = CreateDefaultSubobject<UEFT_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UEFT_AttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AEFT_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}