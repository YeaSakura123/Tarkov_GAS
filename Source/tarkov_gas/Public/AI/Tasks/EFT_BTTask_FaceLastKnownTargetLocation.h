// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "EFT_BTTask_FaceLastKnownTargetLocation.generated.h"

class AEFT_EnemyCharacter;

struct FEFTBTTaskFaceLastKnownTargetLocationMemory
{
	float ElapsedTime{0.f};
};

UCLASS()
class TARKOV_GAS_API UEFT_BTTask_FaceLastKnownTargetLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UEFT_BTTask_FaceLastKnownTargetLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override;

	UPROPERTY(EditAnywhere, Category = "EFT|Blackboard")
	FBlackboardKeySelector LastKnownTargetLocationKey;

	UPROPERTY(EditAnywhere, Category = "EFT|Task", meta = (ClampMin = "-1.0"))
	float DurationOverride{-1.f};

	UPROPERTY(EditAnywhere, Category = "EFT|Task")
	bool bUseHoldAngleDuration{false};

private:
	float ResolveDuration(const AEFT_EnemyCharacter* Enemy) const;
	bool FaceLastKnownLocation(UBehaviorTreeComponent& OwnerComp) const;
};
