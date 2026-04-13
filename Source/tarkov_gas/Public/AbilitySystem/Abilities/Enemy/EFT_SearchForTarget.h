// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/EFT_GameplayAbility.h"
#include "EFT_SearchForTarget.generated.h"

namespace EPathFollowingResult
{
	enum Type : int;
}

class UAITask_MoveTo;
class AEFT_BaseCharacter;
class AAIController;
class AEFT_EnemyCharacter;
class UEFT_WaitGameplayEvent;
class UAbilityTask_WaitDelay;

UCLASS()
class TARKOV_GAS_API UEFT_SearchForTarget : public UEFT_GameplayAbility
{
	GENERATED_BODY()
public:
	UEFT_SearchForTarget();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	TWeakObjectPtr<AEFT_EnemyCharacter> OwningEnemy;
	TWeakObjectPtr<AAIController> OwningAIController;
	TWeakObjectPtr<AEFT_BaseCharacter> TargetBaseCharacter;
	
private:
	UPROPERTY()
	TObjectPtr<UEFT_WaitGameplayEvent> WaitGameplayEventTask;
	
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitDelay> SearchDelayTask;
	
	UPROPERTY()
	TObjectPtr<UAITask_MoveTo> MoveToLocationOrActorTask;
		
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitDelay> AttackDelayTask;
	
	void StartSearch();
	
	UFUNCTION()
	void EndAttackEventReceived(FGameplayEventData Payload);
	
	UFUNCTION()
	void Search();
	
	void MoveToTargetAndAttack();
	
	UFUNCTION()
	void AttackTarget(TEnumAsByte<EPathFollowingResult::Type> Result, AAIController* AIController);
	
	UFUNCTION()
	void Attack();
};
