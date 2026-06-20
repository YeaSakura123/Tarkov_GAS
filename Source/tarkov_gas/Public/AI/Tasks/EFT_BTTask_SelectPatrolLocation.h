// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "EFT_BTTask_SelectPatrolLocation.generated.h"

UCLASS()
class TARKOV_GAS_API UEFT_BTTask_SelectPatrolLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UEFT_BTTask_SelectPatrolLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "EFT|Blackboard")
	FBlackboardKeySelector SpawnLocationKey;

	UPROPERTY(EditAnywhere, Category = "EFT|Blackboard")
	FBlackboardKeySelector PatrolLocationKey;
};
