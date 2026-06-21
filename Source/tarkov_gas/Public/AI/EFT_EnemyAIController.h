// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EFT_EnemyAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UBehaviorTreeComponent;
class UBlackboardComponent;

UCLASS()
class TARKOV_GAS_API AEFT_EnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEFT_EnemyAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	static const FName KeySelfActor;
	static const FName KeyTargetActor;
	static const FName KeyLastKnownTargetLocation;
	static const FName KeyHasLastKnownTargetLocation;
	static const FName KeySpawnLocation;
	static const FName KeyPatrolLocation;
	static const FName KeySearchLocation;
	static const FName KeyAIState;
	static const FName KeyDesiredDecision;
	static const FName KeyTargetDistance;
	static const FName KeyHasLineOfSight;
	static const FName KeyInAttackRange;
	static const FName KeyAttackReady;
	static const FName KeyLostSightSearchStartTime;
	static const FName KeyLostSightSearchCount;

private:
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	void ConfigureSightFromPossessedEnemy() const;
	void SetTarget(AActor* TargetActor);
	void ClearTarget(AActor* TargetActor);
	void ResetLostSightSearch(UBlackboardComponent& BlackboardComp, float SearchStartTime) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EFT|AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EFT|AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EFT|AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAISenseConfig_Sight> SightConfig;
};
