// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "EFT_BaseCharacter.h"
#include "EFT_EnemyCharacter.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class TARKOV_GAS_API AEFT_EnemyCharacter : public AEFT_BaseCharacter
{
	GENERATED_BODY()

public:
	AEFT_EnemyCharacter();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributeSet() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EFT|AI")
	float AcceptanceRadius{500.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EFT|AI")
	float MinAttackDelay{.1f};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EFT|AI")
	float MaxAttackDelay{.5f};
	
	UFUNCTION(BlueprintImplementableEvent)
	float GetTimelineLength();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool bIsBeingLaunched{false};
	
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
};
