// YeaSakura123 2025.11

#include "AI/Tasks/EFT_BTTask_FaceLastKnownTargetLocation.h"

#include "AIController.h"
#include "AI/EFT_EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EFT_EnemyCharacter.h"
#include "GameFramework/Pawn.h"

UEFT_BTTask_FaceLastKnownTargetLocation::UEFT_BTTask_FaceLastKnownTargetLocation()
{
	NodeName = TEXT("Face Last Known Target Location");
	bNotifyTick = true;
	LastKnownTargetLocationKey.SelectedKeyName = AEFT_EnemyAIController::KeyLastKnownTargetLocation;
}

EBTNodeResult::Type UEFT_BTTask_FaceLastKnownTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FEFTBTTaskFaceLastKnownTargetLocationMemory* Memory = reinterpret_cast<FEFTBTTaskFaceLastKnownTargetLocationMemory*>(NodeMemory);
	Memory->ElapsedTime = 0.f;

	AAIController* AIController = OwnerComp.GetAIOwner();
	const AEFT_EnemyCharacter* Enemy = IsValid(AIController) ? Cast<AEFT_EnemyCharacter>(AIController->GetPawn()) : nullptr;
	if (!IsValid(Enemy) || !FaceLastKnownLocation(OwnerComp)) return EBTNodeResult::Failed;

	return ResolveDuration(Enemy) <= 0.f ? EBTNodeResult::Succeeded : EBTNodeResult::InProgress;
}

void UEFT_BTTask_FaceLastKnownTargetLocation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FEFTBTTaskFaceLastKnownTargetLocationMemory* Memory = reinterpret_cast<FEFTBTTaskFaceLastKnownTargetLocationMemory*>(NodeMemory);
	Memory->ElapsedTime += DeltaSeconds;

	AAIController* AIController = OwnerComp.GetAIOwner();
	const AEFT_EnemyCharacter* Enemy = IsValid(AIController) ? Cast<AEFT_EnemyCharacter>(AIController->GetPawn()) : nullptr;
	if (!IsValid(Enemy) || !FaceLastKnownLocation(OwnerComp))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (Memory->ElapsedTime >= ResolveDuration(Enemy))
	{
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

uint16 UEFT_BTTask_FaceLastKnownTargetLocation::GetInstanceMemorySize() const
{
	return sizeof(FEFTBTTaskFaceLastKnownTargetLocationMemory);
}

float UEFT_BTTask_FaceLastKnownTargetLocation::ResolveDuration(const AEFT_EnemyCharacter* Enemy) const
{
	if (DurationOverride >= 0.f)
	{
		return DurationOverride;
	}

	if (!IsValid(Enemy))
	{
		return 0.f;
	}

	return bUseHoldAngleDuration ? Enemy->LostSightHoldAngleDuration : Enemy->LostSightSearchObserveDuration;
}

bool UEFT_BTTask_FaceLastKnownTargetLocation::FaceLastKnownLocation(UBehaviorTreeComponent& OwnerComp) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	APawn* Pawn = IsValid(AIController) ? AIController->GetPawn() : nullptr;
	if (!IsValid(AIController) || !IsValid(BlackboardComp) || !IsValid(Pawn)) return false;

	const FVector LastKnownTargetLocation = BlackboardComp->GetValueAsVector(LastKnownTargetLocationKey.SelectedKeyName);
	AIController->SetFocalPoint(LastKnownTargetLocation, EAIFocusPriority::Gameplay);

	FVector LookDirection = LastKnownTargetLocation - Pawn->GetActorLocation();
	LookDirection.Z = 0.f;
	if (!LookDirection.IsNearlyZero())
	{
		const FRotator LookRotation = LookDirection.Rotation();
		AIController->SetControlRotation(LookRotation);
		Pawn->SetActorRotation(LookRotation);
	}

	return true;
}
