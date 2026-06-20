// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "AI/EFT_EnemyAITypes.h"
#include "EFT_BaseCharacter.h"
#include "EFT_EnemyCharacter.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class UBehaviorTree;

UCLASS()
class TARKOV_GAS_API AEFT_EnemyCharacter : public AEFT_BaseCharacter
{
	GENERATED_BODY()

public:
	AEFT_EnemyCharacter();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributeSet() const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EFT|AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EFT|AI")
	float AcceptanceRadius{500.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EFT|AI")
	float AttackRange{250.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EFT|AI")
	float SightRadius{1500.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EFT|AI")
	float LoseSightSeconds{3.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EFT|AI")
	float PatrolRadius{1200.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EFT|AI")
	float AttackCooldown{1.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EFT|AI")
	float MinAttackDelay{.1f};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EFT|AI")
	float MaxAttackDelay{.5f};
	
	UFUNCTION(BlueprintImplementableEvent)
	float GetTimelineLength();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool bIsBeingLaunched{false};

	UFUNCTION(BlueprintCallable, Category = "EFT|AI")
	void SetCombatTarget(AActor* NewTarget);

	UFUNCTION(BlueprintPure, Category = "EFT|AI")
	AActor* GetCombatTarget() const { return CombatTarget.Get(); }

	UFUNCTION(BlueprintCallable, Category = "EFT|AI")
	void SetAIState(EEFTEnemyAIState NewState);

	UFUNCTION(BlueprintPure, Category = "EFT|AI")
	EEFTEnemyAIState GetAIState() const { return AIState; }

	bool IsAttackReady(float CurrentTime) const;
	void MarkAttackStarted(float CurrentTime);
	
	void StopMovementUntilLanded();
protected:
	virtual void BeginPlay() override;
	virtual void HandleDeath() override;
private:
	
	UFUNCTION()
	void EnableMovementOnLanded(const FHitResult& Hit);
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category = "EFT|AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category = "EFT|AI", meta = (AllowPrivateAccess = "true"))
	EEFTEnemyAIState AIState{EEFTEnemyAIState::Idle};

	float LastAttackTime{-1000000.f};
};
