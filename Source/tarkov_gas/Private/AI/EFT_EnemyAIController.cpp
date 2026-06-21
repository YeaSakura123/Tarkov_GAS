// YeaSakura123 2025.11

#include "AI/EFT_EnemyAIController.h"

#include "AI/EFT_EnemyAIConfigDataAsset.h"
#include "AI/EFT_EnemyAITypes.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EFT_BaseCharacter.h"
#include "Characters/EFT_EnemyCharacter.h"
#include "GameplayTags/EFTTags.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

namespace
{
	void SetDynamicSubtreeIfValid(UBehaviorTreeComponent* BehaviorTreeComponent, const FGameplayTag& InjectTag, UBehaviorTree* Subtree)
	{
		if (!IsValid(BehaviorTreeComponent) || !InjectTag.IsValid() || !IsValid(Subtree)) return;
		BehaviorTreeComponent->SetDynamicSubtree(InjectTag, Subtree);
	}
}

const FName AEFT_EnemyAIController::KeySelfActor(TEXT("SelfActor"));
const FName AEFT_EnemyAIController::KeyTargetActor(TEXT("TargetActor"));
const FName AEFT_EnemyAIController::KeyLastKnownTargetLocation(TEXT("LastKnownTargetLocation"));
const FName AEFT_EnemyAIController::KeyHasLastKnownTargetLocation(TEXT("bHasLastKnownTargetLocation"));
const FName AEFT_EnemyAIController::KeySpawnLocation(TEXT("SpawnLocation"));
const FName AEFT_EnemyAIController::KeyPatrolLocation(TEXT("PatrolLocation"));
const FName AEFT_EnemyAIController::KeySearchLocation(TEXT("SearchLocation"));
const FName AEFT_EnemyAIController::KeyAIState(TEXT("AIState"));
const FName AEFT_EnemyAIController::KeyDesiredDecision(TEXT("DesiredDecision"));
const FName AEFT_EnemyAIController::KeyTargetDistance(TEXT("TargetDistance"));
const FName AEFT_EnemyAIController::KeyHasLineOfSight(TEXT("bHasLineOfSight"));
const FName AEFT_EnemyAIController::KeyInAttackRange(TEXT("bInAttackRange"));
const FName AEFT_EnemyAIController::KeyAttackReady(TEXT("bAttackReady"));
const FName AEFT_EnemyAIController::KeyLostSightSearchStartTime(TEXT("LostSightSearchStartTime"));
const FName AEFT_EnemyAIController::KeyLostSightSearchCount(TEXT("LostSightSearchCount"));

AEFT_EnemyAIController::AEFT_EnemyAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BrainComponent = BehaviorTreeComponent;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SetPerceptionComponent(*AIPerceptionComponent);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.f;
	SightConfig->LoseSightRadius = 2000.f;
	SightConfig->PeripheralVisionAngleDegrees = 75.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::OnTargetPerceptionUpdated);
}

void AEFT_EnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AEFT_EnemyCharacter* Enemy = Cast<AEFT_EnemyCharacter>(InPawn);
	if (IsValid(Enemy))
	{
		Enemy->ApplyAIConfig();
	}

	if (!IsValid(Enemy) || !IsValid(Enemy->BehaviorTreeAsset)) return;
	if (!IsValid(Enemy->BehaviorTreeAsset->BlackboardAsset)) return;

	ConfigureSightFromPossessedEnemy();

	UBlackboardComponent* BlackboardComp = nullptr;
	if (!UseBlackboard(Enemy->BehaviorTreeAsset->BlackboardAsset, BlackboardComp)) return;

	BlackboardComp->SetValueAsObject(KeySelfActor, Enemy);
	BlackboardComp->SetValueAsVector(KeySpawnLocation, Enemy->GetActorLocation());
	BlackboardComp->SetValueAsEnum(KeyAIState, static_cast<uint8>(EEFTEnemyAIState::Idle));
	BlackboardComp->SetValueAsEnum(KeyDesiredDecision, static_cast<uint8>(EEFTEnemyDecision::None));
	BlackboardComp->SetValueAsBool(KeyHasLastKnownTargetLocation, false);
	BlackboardComp->SetValueAsVector(KeySearchLocation, Enemy->GetActorLocation());
	ResetLostSightSearch(*BlackboardComp, -1.f);

	RunBehaviorTree(Enemy->BehaviorTreeAsset);

	const UEFT_EnemyAIConfigDataAsset* AIConfig = Enemy->AIConfig;
	if (IsValid(AIConfig))
	{
		SetDynamicSubtreeIfValid(BehaviorTreeComponent, EFTTags::AI::Subtree::Combat, AIConfig->CombatSubtree.Get());
		SetDynamicSubtreeIfValid(BehaviorTreeComponent, EFTTags::AI::Subtree::Search, AIConfig->SearchSubtree.Get());
		SetDynamicSubtreeIfValid(BehaviorTreeComponent, EFTTags::AI::Subtree::Patrol, AIConfig->PatrolSubtree.Get());
	}
}

void AEFT_EnemyAIController::OnUnPossess()
{
	if (IsValid(BehaviorTreeComponent))
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}

	Super::OnUnPossess();
}

void AEFT_EnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!IsValid(Actor) || !Actor->ActorHasTag(EFTTags::Player)) return;

	if (Stimulus.WasSuccessfullySensed())
	{
		const AEFT_BaseCharacter* BaseCharacter = Cast<AEFT_BaseCharacter>(Actor);
		if (IsValid(BaseCharacter) && BaseCharacter->IsAlive())
		{
			SetTarget(Actor);
		}
	}
	else
	{
		ClearTarget(Actor);
	}
}

void AEFT_EnemyAIController::ConfigureSightFromPossessedEnemy() const
{
	const AEFT_EnemyCharacter* Enemy = Cast<AEFT_EnemyCharacter>(GetPawn());
	if (!IsValid(Enemy) || !IsValid(SightConfig) || !IsValid(AIPerceptionComponent)) return;

	SightConfig->SightRadius = Enemy->SightRadius;
	SightConfig->LoseSightRadius = Enemy->SightRadius + 500.f;
	SightConfig->SetMaxAge(Enemy->LoseSightSeconds);

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->RequestStimuliListenerUpdate();
}

void AEFT_EnemyAIController::SetTarget(AActor* TargetActor)
{
	AEFT_EnemyCharacter* Enemy = Cast<AEFT_EnemyCharacter>(GetPawn());
	UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
	if (!IsValid(Enemy) || !IsValid(BlackboardComp) || !IsValid(TargetActor)) return;

	Enemy->SetCombatTarget(TargetActor);
	BlackboardComp->SetValueAsObject(KeyTargetActor, TargetActor);
	BlackboardComp->SetValueAsVector(KeyLastKnownTargetLocation, TargetActor->GetActorLocation());
	BlackboardComp->SetValueAsBool(KeyHasLastKnownTargetLocation, true);
	ResetLostSightSearch(*BlackboardComp, -1.f);
}

void AEFT_EnemyAIController::ClearTarget(AActor* TargetActor)
{
	AEFT_EnemyCharacter* Enemy = Cast<AEFT_EnemyCharacter>(GetPawn());
	UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
	if (!IsValid(Enemy) || !IsValid(BlackboardComp)) return;

	if (Enemy->GetCombatTarget() == TargetActor)
	{
		Enemy->SetCombatTarget(nullptr);
		BlackboardComp->ClearValue(KeyTargetActor);
	}

	if (IsValid(TargetActor))
	{
		BlackboardComp->SetValueAsVector(KeyLastKnownTargetLocation, TargetActor->GetActorLocation());
		BlackboardComp->SetValueAsBool(KeyHasLastKnownTargetLocation, true);
		const UWorld* World = GetWorld();
		ResetLostSightSearch(*BlackboardComp, IsValid(World) ? World->GetTimeSeconds() : 0.f);
	}
}

void AEFT_EnemyAIController::ResetLostSightSearch(UBlackboardComponent& BlackboardComp, float SearchStartTime) const
{
	BlackboardComp.SetValueAsFloat(KeyLostSightSearchStartTime, SearchStartTime);
	BlackboardComp.SetValueAsInt(KeyLostSightSearchCount, 0);
}
