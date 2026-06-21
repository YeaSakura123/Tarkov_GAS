// YeaSakura123 2025.11

#include "AI/Tasks/EFT_BTTask_SelectNearbySearchLocation.h"

#include "AI/EFT_EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EFT_EnemyCharacter.h"
#include "NavigationSystem.h"

UEFT_BTTask_SelectNearbySearchLocation::UEFT_BTTask_SelectNearbySearchLocation()
{
	NodeName = TEXT("Select Nearby Search Location");
	SearchLocationKey.SelectedKeyName = AEFT_EnemyAIController::KeySearchLocation;
	LostSightSearchCountKey.SelectedKeyName = AEFT_EnemyAIController::KeyLostSightSearchCount;
}

EBTNodeResult::Type UEFT_BTTask_SelectNearbySearchLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	AEFT_EnemyCharacter* Enemy = IsValid(AIController) ? Cast<AEFT_EnemyCharacter>(AIController->GetPawn()) : nullptr;
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!IsValid(Enemy) || !IsValid(BlackboardComp)) return EBTNodeResult::Failed;

	const int32 SearchCount = BlackboardComp->GetValueAsInt(LostSightSearchCountKey.SelectedKeyName);
	BlackboardComp->SetValueAsInt(LostSightSearchCountKey.SelectedKeyName, SearchCount + 1);

	const FVector EnemyLocation = Enemy->GetActorLocation();
	FVector SearchLocation = EnemyLocation;

	UWorld* World = Enemy->GetWorld();
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (IsValid(NavSystem) && Enemy->LostSightSearchRadius > 0.f)
	{
		FNavLocation NavLocation;
		if (NavSystem->GetRandomReachablePointInRadius(EnemyLocation, Enemy->LostSightSearchRadius, NavLocation))
		{
			SearchLocation = NavLocation.Location;
		}
	}

	BlackboardComp->SetValueAsVector(SearchLocationKey.SelectedKeyName, SearchLocation);
	return EBTNodeResult::Succeeded;
}
