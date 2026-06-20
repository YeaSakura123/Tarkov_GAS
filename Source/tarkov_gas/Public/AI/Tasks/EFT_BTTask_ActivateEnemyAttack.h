// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "EFT_BTTask_ActivateEnemyAttack.generated.h"

struct FEFTBTTaskActivateEnemyAttackMemory
{
	double FinishTime = 0.0;
};

UCLASS()
class TARKOV_GAS_API UEFT_BTTask_ActivateEnemyAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UEFT_BTTask_ActivateEnemyAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override;

	UPROPERTY(EditAnywhere, Category = "EFT|Blackboard")
	FBlackboardKeySelector TargetActorKey;
};
