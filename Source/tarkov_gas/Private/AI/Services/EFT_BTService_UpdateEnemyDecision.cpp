// YeaSakura123 2025.11

#include "AI/Services/EFT_BTService_UpdateEnemyDecision.h"

#include "AI/EFT_EnemyAIController.h"
#include "AI/EFT_EnemyAITypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EFT_BaseCharacter.h"
#include "Characters/EFT_EnemyCharacter.h"

UEFT_BTService_UpdateEnemyDecision::UEFT_BTService_UpdateEnemyDecision()
{
	NodeName = TEXT("Update Enemy Decision");
	Interval = 0.2f;
	RandomDeviation = 0.05f;

	TargetActorKey.SelectedKeyName = AEFT_EnemyAIController::KeyTargetActor;
	HasLastKnownTargetLocationKey.SelectedKeyName = AEFT_EnemyAIController::KeyHasLastKnownTargetLocation;
	LastKnownTargetLocationKey.SelectedKeyName = AEFT_EnemyAIController::KeyLastKnownTargetLocation;
	AIStateKey.SelectedKeyName = AEFT_EnemyAIController::KeyAIState;
	DesiredDecisionKey.SelectedKeyName = AEFT_EnemyAIController::KeyDesiredDecision;
	TargetDistanceKey.SelectedKeyName = AEFT_EnemyAIController::KeyTargetDistance;
	HasLineOfSightKey.SelectedKeyName = AEFT_EnemyAIController::KeyHasLineOfSight;
	InAttackRangeKey.SelectedKeyName = AEFT_EnemyAIController::KeyInAttackRange;
	AttackReadyKey.SelectedKeyName = AEFT_EnemyAIController::KeyAttackReady;
	LostSightSearchStartTimeKey.SelectedKeyName = AEFT_EnemyAIController::KeyLostSightSearchStartTime;
	LostSightSearchCountKey.SelectedKeyName = AEFT_EnemyAIController::KeyLostSightSearchCount;
}

void UEFT_BTService_UpdateEnemyDecision::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AEFT_EnemyAIController* AIController = Cast<AEFT_EnemyAIController>(OwnerComp.GetAIOwner());
	AEFT_EnemyCharacter* Enemy = IsValid(AIController) ? Cast<AEFT_EnemyCharacter>(AIController->GetPawn()) : nullptr;
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!IsValid(AIController) || !IsValid(Enemy) || !IsValid(BlackboardComp)) return;

	EEFTEnemyAIState NewState = EEFTEnemyAIState::Idle;
	EEFTEnemyDecision NewDecision = EEFTEnemyDecision::None;
	float TargetDistance = 0.f;
	bool bHasLineOfSight = false;
	bool bInAttackRange = false;
	bool bAttackReady = false;

	if (!Enemy->IsAlive())
	{
		NewState = EEFTEnemyAIState::Dead;
		NewDecision = EEFTEnemyDecision::Wait;
	}
	else if (Enemy->bIsBeingLaunched)
	{
		NewState = EEFTEnemyAIState::HitReact;
		NewDecision = EEFTEnemyDecision::Wait;
	}
	else
	{
		AEFT_BaseCharacter* Target = Cast<AEFT_BaseCharacter>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
		if (IsValid(Target) && Target->IsAlive())
		{
			Enemy->SetCombatTarget(Target);
			TargetDistance = FVector::Dist(Enemy->GetActorLocation(), Target->GetActorLocation());
			bHasLineOfSight = AIController->LineOfSightTo(Target);
			bInAttackRange = TargetDistance <= Enemy->AttackRange;

			if (bHasLineOfSight)
			{
				BlackboardComp->SetValueAsVector(LastKnownTargetLocationKey.SelectedKeyName, Target->GetActorLocation());
				BlackboardComp->SetValueAsBool(HasLastKnownTargetLocationKey.SelectedKeyName, true);
				BlackboardComp->SetValueAsFloat(LostSightSearchStartTimeKey.SelectedKeyName, -1.f);
				BlackboardComp->SetValueAsInt(LostSightSearchCountKey.SelectedKeyName, 0);
			}

			const UWorld* World = Enemy->GetWorld();
			bAttackReady = IsValid(World) && Enemy->IsAttackReady(World->GetTimeSeconds());

			if (bHasLineOfSight && bInAttackRange && bAttackReady)
			{
				NewState = EEFTEnemyAIState::Attack;
				NewDecision = EEFTEnemyDecision::Attack;
			}
			else
			{
				NewState = EEFTEnemyAIState::Chase;
				NewDecision = EEFTEnemyDecision::Chase;
			}
		}
		else
		{
			Enemy->SetCombatTarget(nullptr);
			BlackboardComp->ClearValue(TargetActorKey.SelectedKeyName);

			if (IsValid(Target) && !Target->IsAlive())
			{
				BlackboardComp->ClearValue(LastKnownTargetLocationKey.SelectedKeyName);
				BlackboardComp->SetValueAsBool(HasLastKnownTargetLocationKey.SelectedKeyName, false);
				BlackboardComp->SetValueAsFloat(LostSightSearchStartTimeKey.SelectedKeyName, -1.f);
				BlackboardComp->SetValueAsInt(LostSightSearchCountKey.SelectedKeyName, 0);
			}

			if (BlackboardComp->GetValueAsBool(HasLastKnownTargetLocationKey.SelectedKeyName))
			{
				const UWorld* World = Enemy->GetWorld();
				const float CurrentTime = IsValid(World) ? World->GetTimeSeconds() : 0.f;
				const float SearchStartTime = BlackboardComp->GetValueAsFloat(LostSightSearchStartTimeKey.SelectedKeyName);
				if (SearchStartTime < 0.f)
				{
					BlackboardComp->SetValueAsFloat(LostSightSearchStartTimeKey.SelectedKeyName, CurrentTime);
					BlackboardComp->SetValueAsInt(LostSightSearchCountKey.SelectedKeyName, 0);
				}

				const FVector LastKnownTargetLocation = BlackboardComp->GetValueAsVector(LastKnownTargetLocationKey.SelectedKeyName);
				TargetDistance = FVector::Dist(Enemy->GetActorLocation(), LastKnownTargetLocation);

				const int32 SearchCount = BlackboardComp->GetValueAsInt(LostSightSearchCountKey.SelectedKeyName);
				if (SearchCount < FMath::Max(0, Enemy->MaxLostSightSearches))
				{
					NewState = EEFTEnemyAIState::Search;
					NewDecision = EEFTEnemyDecision::SearchNearby;
				}
				else
				{
					NewState = EEFTEnemyAIState::HoldAngle;
					NewDecision = EEFTEnemyDecision::HoldAngle;
				}
			}
			else
			{
				NewState = EEFTEnemyAIState::Patrol;
				NewDecision = EEFTEnemyDecision::Patrol;
			}
		}
	}

	Enemy->SetAIState(NewState);
	BlackboardComp->SetValueAsEnum(AIStateKey.SelectedKeyName, static_cast<uint8>(NewState));
	BlackboardComp->SetValueAsEnum(DesiredDecisionKey.SelectedKeyName, static_cast<uint8>(NewDecision));
	BlackboardComp->SetValueAsFloat(TargetDistanceKey.SelectedKeyName, TargetDistance);
	BlackboardComp->SetValueAsBool(HasLineOfSightKey.SelectedKeyName, bHasLineOfSight);
	BlackboardComp->SetValueAsBool(InAttackRangeKey.SelectedKeyName, bInAttackRange);
	BlackboardComp->SetValueAsBool(AttackReadyKey.SelectedKeyName, bAttackReady);
}
