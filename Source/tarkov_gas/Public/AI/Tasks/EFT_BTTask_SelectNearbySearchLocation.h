// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "EFT_BTTask_SelectNearbySearchLocation.generated.h"

UCLASS()
class TARKOV_GAS_API UEFT_BTTask_SelectNearbySearchLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UEFT_BTTask_SelectNearbySearchLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "EFT|Blackboard")
	FBlackboardKeySelector SearchLocationKey;

	UPROPERTY(EditAnywhere, Category = "EFT|Blackboard")
	FBlackboardKeySelector LostSightSearchCountKey;
};
