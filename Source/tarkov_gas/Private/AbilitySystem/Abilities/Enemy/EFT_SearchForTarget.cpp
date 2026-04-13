// YeaSakura123 2025.11


#include "AbilitySystem/Abilities/Enemy/EFT_SearchForTarget.h"

#include "AIController.h"
#include "Characters/EFT_EnemyCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Async/AbilityAsync_WaitGameplayEvent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/AbilityTasks/EFT_WaitGameplayEvent.h"
#include "GameplayTags/EFTTags.h"
#include "Tasks/AITask_MoveTo.h"
#include "Utils/EFT_BlueprintLibrary.h"

UEFT_SearchForTarget::UEFT_SearchForTarget()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnly;
}

void UEFT_SearchForTarget::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	OwningEnemy = Cast<AEFT_EnemyCharacter>(GetAvatarActorFromActorInfo());
	check(OwningEnemy.IsValid());
	OwningAIController = Cast<AAIController>(OwningEnemy->GetController());
	check(OwningAIController.IsValid());
	
	StartSearch();
	
	WaitGameplayEventTask = UEFT_WaitGameplayEvent::WaitGameplayEventToActorProxy(GetAvatarActorFromActorInfo(), EFTTags::Events::Enemy::EndAttack);
	WaitGameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::EndAttackEventReceived);
	WaitGameplayEventTask->StartActivation();
}

void UEFT_SearchForTarget::StartSearch()
{
	if (bDrawDebugs) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("UEFT_SearchForTarget::StartSearch"));
	if (!OwningEnemy.IsValid()) return;
	
	const float SearchDelay = FMath::FRandRange(OwningEnemy->MinAttackDelay, OwningEnemy->MaxAttackDelay);
	SearchDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, SearchDelay);
	SearchDelayTask->OnFinish.AddDynamic(this, &ThisClass::Search);
	SearchDelayTask->Activate();
}

void UEFT_SearchForTarget::EndAttackEventReceived(FGameplayEventData Payload)
{
	if (OwningEnemy.IsValid() && !OwningEnemy->bIsBeingLaunched)
	{
		StartSearch();
	}
	
}

void UEFT_SearchForTarget::Search()
{
	const FVector SearchOrigin = GetAvatarActorFromActorInfo()->GetActorLocation();
	if (!OwningEnemy.IsValid()) return;
	FClosestActorWithTagResult ClosestActorResult = UEFT_BlueprintLibrary::FindClosestActorWithTag(GetAvatarActorFromActorInfo(), SearchOrigin, EFTTags::Player, OwningEnemy->SearchRange);
	
	TargetBaseCharacter = Cast<AEFT_BaseCharacter>(ClosestActorResult.Actor);
	
	if (!TargetBaseCharacter.IsValid())
	{
		StartSearch();
		return;
	}
	if (TargetBaseCharacter->IsAlive())
	{
		MoveToTargetAndAttack();
	}
	else
	{
		StartSearch();
	}
}

void UEFT_SearchForTarget::MoveToTargetAndAttack()
{
	if (!OwningEnemy.IsValid() || !OwningAIController.IsValid() || !TargetBaseCharacter.IsValid()) return;
	if (!OwningEnemy->IsAlive())
	{
		StartSearch();
		return;
	}
	
	MoveToLocationOrActorTask = UAITask_MoveTo::AIMoveTo(
		OwningAIController.Get(), 
		FVector(), 
		TargetBaseCharacter.Get(), 
		OwningEnemy->AcceptanceRadius);
	
	MoveToLocationOrActorTask->OnMoveTaskFinished.AddUObject(this, &ThisClass::AttackTarget);
	MoveToLocationOrActorTask->ConditionalPerformMove();
}

void UEFT_SearchForTarget::AttackTarget(TEnumAsByte<EPathFollowingResult::Type> Result, AAIController* AIController)
{
	if (Result != EPathFollowingResult::Success)
	{
		StartSearch();
		return;
	}
	OwningEnemy->RotateToTarget(TargetBaseCharacter.Get());
	
	AttackDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, OwningEnemy->GetTimelineLength());
	AttackDelayTask->OnFinish.AddDynamic(this, &ThisClass::Attack);
	AttackDelayTask->Activate();
}

void UEFT_SearchForTarget::Attack()
{
	const FGameplayTag AttackTag = EFTTags::EFTAbilities::Enemy::Attack;
	GetAbilitySystemComponentFromActorInfo()->TryActivateAbilitiesByTag(AttackTag.GetSingleTagContainer());
}
