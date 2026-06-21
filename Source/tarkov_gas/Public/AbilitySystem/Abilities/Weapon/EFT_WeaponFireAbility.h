// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/EFT_GameplayAbility.h"
#include "EFT_WeaponFireAbility.generated.h"

UCLASS()
class TARKOV_GAS_API UEFT_WeaponFireAbility : public UEFT_GameplayAbility
{
	GENERATED_BODY()

public:
	UEFT_WeaponFireAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
