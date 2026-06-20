// YeaSakura123 2025.11

#include "AI/Tasks/EFT_BTTask_SelectPatrolLocation.h"

#include "AI/EFT_EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EFT_EnemyCharacter.h"
#include "NavigationSystem.h"

UEFT_BTTask_SelectPatrolLocation::UEFT_BTTask_SelectPatrolLocation()
{
	NodeName = TEXT("Select Patrol Location");
	SpawnLocationKey.SelectedKeyName = AEFT_EnemyAIController::KeySpawnLocation;
	PatrolLocationKey.SelectedKeyName = AEFT_EnemyAIController::KeyPatrolLocation;
}

EBTNodeResult::Type UEFT_BTTask_SelectPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	AEFT_EnemyCharacter* Enemy = IsValid(AIController) ? Cast<AEFT_EnemyCharacter>(AIController->GetPawn()) : nullptr;
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!IsValid(Enemy) || !IsValid(BlackboardComp)) return EBTNodeResult::Failed;

	UWorld* World = Enemy->GetWorld();
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (!IsValid(NavSystem)) return EBTNodeResult::Failed;

	const FVector SpawnLocation = BlackboardComp->GetValueAsVector(SpawnLocationKey.SelectedKeyName);
	FNavLocation PatrolLocation;
	if (!NavSystem->GetRandomReachablePointInRadius(SpawnLocation, Enemy->PatrolRadius, PatrolLocation))
	{
		return EBTNodeResult::Failed;
	}

	BlackboardComp->SetValueAsVector(PatrolLocationKey.SelectedKeyName, PatrolLocation.Location);
	return EBTNodeResult::Succeeded;
}
