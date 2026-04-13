// YeaSakura123 2025.11


#include "AbilitySystem/Abilities/EFT_Run.h"

#include "Abilities/Tasks/AbilityTask_WaitAttributeChange.h"
#include "AbilitySystem/EFT_AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayTags/EFTTags.h"


void UEFT_Run::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                               const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	float CurrentStamina = GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(UEFT_AttributeSet::GetStaminaAttribute());
	if (CurrentStamina <= 0.0f)
	{
		OnStaminaReachedZero();
		return;
	}
	
	UAbilityTask_WaitAttributeChange* WaitStaminaChangeTask = UAbilityTask_WaitAttributeChange::WaitForAttributeChange(
		this,
		UEFT_AttributeSet::GetStaminaAttribute(),
		FGameplayTag(),
		FGameplayTag(),
		false
	);

	if (IsValid(WaitStaminaChangeTask))
	{
		WaitStaminaChangeTask->OnChange.AddDynamic(this, &ThisClass::OnStaminaChanged);
		WaitStaminaChangeTask->ReadyForActivation();
	}
}

void UEFT_Run::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	CleanupSprintState(ActorInfo);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEFT_Run::OnStaminaChanged()
{
	float CurrentStamina = GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(UEFT_AttributeSet::GetStaminaAttribute());

	if (CurrentStamina <= 0.0f)
	{
		OnStaminaReachedZero();
	}
}

void UEFT_Run::OnStaminaReachedZero()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEFT_Run::CleanupSprintState(const FGameplayAbilityActorInfo* ActorInfo) const
{
	if (!ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
	{
		return;
	}

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC)
	{
		return;
	}

	// 奔跑能力结束时主动清理 Sprinting 相关效果，避免客户端继续收到扣体力复制。
	FGameplayTagContainer SprintingTags;
	SprintingTags.AddTag(EFTTags::Status::Sprinting);
	ASC->RemoveActiveEffectsWithGrantedTags(SprintingTags);
}
