// YeaSakura123 2025.11


#include "Characters/EFT_EnemyCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "AbilitySystem/EFT_AbilitySystemComponent.h"
#include "AbilitySystem/EFT_AttributeSet.h"
#include "GameplayTags/EFTTags.h"
#include "Net/UnrealNetwork.h"


AEFT_EnemyCharacter::AEFT_EnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UEFT_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UEFT_AttributeSet>("AttributeSet");
}

void AEFT_EnemyCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, bIsBeingLaunched);
	DOREPLIFETIME(ThisClass, CombatTarget);
	DOREPLIFETIME(ThisClass, AIState);
}

UAbilitySystemComponent* AEFT_EnemyCharacter::GetAbilitySystemComponent() const 
{
	return AbilitySystemComponent;
}

UAttributeSet* AEFT_EnemyCharacter::GetAttributeSet() const
{
	return AttributeSet;
}

void AEFT_EnemyCharacter::SetCombatTarget(AActor* NewTarget)
{
	CombatTarget = NewTarget;
}

void AEFT_EnemyCharacter::SetAIState(EEFTEnemyAIState NewState)
{
	AIState = NewState;
}

bool AEFT_EnemyCharacter::IsAttackReady(float CurrentTime) const
{
	return CurrentTime - LastAttackTime >= AttackCooldown;
}

void AEFT_EnemyCharacter::MarkAttackStarted(float CurrentTime)
{
	LastAttackTime = CurrentTime;
}

void AEFT_EnemyCharacter::StopMovementUntilLanded()
{
	bIsBeingLaunched = true;
	SetAIState(EEFTEnemyAIState::HitReact);

	AAIController* AIController = Cast<AAIController>(GetController());
	if (!IsValid(AIController)) return;
	AIController->StopMovement();
	if (!LandedDelegate.IsAlreadyBound(this, &ThisClass::EnableMovementOnLanded))
	{
		LandedDelegate.AddDynamic(this, &ThisClass::EnableMovementOnLanded);
	}
}

void AEFT_EnemyCharacter::EnableMovementOnLanded(const FHitResult& Hit)
{
	bIsBeingLaunched = false;
	SetAIState(EEFTEnemyAIState::Idle);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EFTTags::Events::Enemy::EndAttack, FGameplayEventData());
	LandedDelegate.RemoveAll(this);
}

void AEFT_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(GetAbilitySystemComponent())) return;

	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
	ApplyStaminaRegenEffect();
	OnASCInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributeSet());

	if (!HasAuthority()) return;

	GiveStartupAbilities();
	InitializeAttributes();
	
	UEFT_AttributeSet* EFT_AttributeSet = Cast<UEFT_AttributeSet>(GetAttributeSet());
	if (!IsValid(EFT_AttributeSet)) return;
	
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(EFT_AttributeSet->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
}

void AEFT_EnemyCharacter::HandleDeath()
{
	Super::HandleDeath();
	SetAIState(EEFTEnemyAIState::Dead);
	SetCombatTarget(nullptr);
	
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!IsValid(AIController)) return;
	AIController->StopMovement();
}




