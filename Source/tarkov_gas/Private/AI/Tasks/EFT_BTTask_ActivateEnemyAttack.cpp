// YeaSakura123 2025.11

#include "AI/Tasks/EFT_BTTask_ActivateEnemyAttack.h"

#include "AbilitySystemComponent.h"
#include "AI/EFT_EnemyAIController.h"
#include "AI/EFT_EnemyAITypes.h"
#include "Characters/EFT_BaseCharacter.h"
#include "Characters/EFT_EnemyCharacter.h"
#include "GameplayTags/EFTTags.h"
#include "BehaviorTree/BlackboardComponent.h"

UEFT_BTTask_ActivateEnemyAttack::UEFT_BTTask_ActivateEnemyAttack()
{
	NodeName = TEXT("Activate Enemy Attack");
	bNotifyTick = true;
	TargetActorKey.SelectedKeyName = AEFT_EnemyAIController::KeyTargetActor;
}

EBTNodeResult::Type UEFT_BTTask_ActivateEnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	AEFT_EnemyCharacter* Enemy = IsValid(AIController) ? Cast<AEFT_EnemyCharacter>(AIController->GetPawn()) : nullptr;
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!IsValid(Enemy) || !IsValid(BlackboardComp) || !Enemy->IsAlive() || Enemy->bIsBeingLaunched)
	{
		return EBTNodeResult::Failed;
	}

	AEFT_BaseCharacter* Target = Cast<AEFT_BaseCharacter>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!IsValid(Target) || !Target->IsAlive())
	{
		return EBTNodeResult::Failed;
	}

	UAbilitySystemComponent* ASC = Enemy->GetAbilitySystemComponent();
	if (!IsValid(ASC))
	{
		return EBTNodeResult::Failed;
	}

	UWorld* World = Enemy->GetWorld();
	if (!IsValid(World) || !Enemy->IsAttackReady(World->GetTimeSeconds()))
	{
		return EBTNodeResult::Failed;
	}

	Enemy->SetCombatTarget(Target);
	Enemy->SetAIState(EEFTEnemyAIState::Attack);
	Enemy->RotateToTarget(Target);

	const FGameplayTag AttackTag = EFTTags::Abilities::Weapon::Fire;
	if (!ASC->TryActivateAbilitiesByTag(AttackTag.GetSingleTagContainer()))
	{
		return EBTNodeResult::Failed;
	}

	Enemy->MarkAttackStarted(World->GetTimeSeconds());

	FEFTBTTaskActivateEnemyAttackMemory* Memory = reinterpret_cast<FEFTBTTaskActivateEnemyAttackMemory*>(NodeMemory);
	const float AttackDuration = FMath::Max(Enemy->AttackCooldown, Enemy->GetTimelineLength());
	Memory->FinishTime = World->GetTimeSeconds() + AttackDuration;

	return EBTNodeResult::InProgress;
}

void UEFT_BTTask_ActivateEnemyAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	const AAIController* AIController = OwnerComp.GetAIOwner();
	const AEFT_EnemyCharacter* Enemy = IsValid(AIController) ? Cast<AEFT_EnemyCharacter>(AIController->GetPawn()) : nullptr;
	if (!IsValid(Enemy) || !Enemy->IsAlive() || Enemy->bIsBeingLaunched)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	const UWorld* World = Enemy->GetWorld();
	const FEFTBTTaskActivateEnemyAttackMemory* Memory = reinterpret_cast<FEFTBTTaskActivateEnemyAttackMemory*>(NodeMemory);
	if (!IsValid(World) || World->GetTimeSeconds() >= Memory->FinishTime)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

uint16 UEFT_BTTask_ActivateEnemyAttack::GetInstanceMemorySize() const
{
	return sizeof(FEFTBTTaskActivateEnemyAttackMemory);
}
