// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "EFT_BTTask_ClearLastKnownTargetLocation.generated.h"

UCLASS()
class TARKOV_GAS_API UEFT_BTTask_ClearLastKnownTargetLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UEFT_BTTask_ClearLastKnownTargetLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "EFT|Blackboard")
	FBlackboardKeySelector LastKnownTargetLocationKey;

	UPROPERTY(EditAnywhere, Category = "EFT|Blackboard")
	FBlackboardKeySelector HasLastKnownTargetLocationKey;
};
