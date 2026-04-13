// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "EFT_GameplayAbility.h"
#include "EFT_Run.generated.h"

class AEFT_BaseCharacter;
/**
 * 
 */
UCLASS()
class TARKOV_GAS_API UEFT_Run : public UEFT_GameplayAbility
{
	GENERATED_BODY()
	
public:
	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION()
	void OnStaminaChanged();
	
	UFUNCTION()
	void OnStaminaReachedZero();

private:
	void CleanupSprintState(const FGameplayAbilityActorInfo* ActorInfo) const;
};
