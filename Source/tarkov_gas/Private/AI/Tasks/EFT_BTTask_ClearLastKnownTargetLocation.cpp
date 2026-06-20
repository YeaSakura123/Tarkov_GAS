// YeaSakura123 2025.11

#include "AI/Tasks/EFT_BTTask_ClearLastKnownTargetLocation.h"

#include "AI/EFT_EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UEFT_BTTask_ClearLastKnownTargetLocation::UEFT_BTTask_ClearLastKnownTargetLocation()
{
	NodeName = TEXT("Clear Last Known Target Location");
	LastKnownTargetLocationKey.SelectedKeyName = AEFT_EnemyAIController::KeyLastKnownTargetLocation;
	HasLastKnownTargetLocationKey.SelectedKeyName = AEFT_EnemyAIController::KeyHasLastKnownTargetLocation;
}

EBTNodeResult::Type UEFT_BTTask_ClearLastKnownTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!IsValid(BlackboardComp)) return EBTNodeResult::Failed;

	BlackboardComp->ClearValue(LastKnownTargetLocationKey.SelectedKeyName);
	BlackboardComp->SetValueAsBool(HasLastKnownTargetLocationKey.SelectedKeyName, false);
	return EBTNodeResult::Succeeded;
}
