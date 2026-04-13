// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EFT_GameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class TARKOV_GAS_API UEFT_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EFT|Debug")
	bool bDrawDebugs = false;
};
