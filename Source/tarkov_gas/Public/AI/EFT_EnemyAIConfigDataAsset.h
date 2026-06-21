// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "AI/EFT_EnemyAITypes.h"
#include "Engine/DataAsset.h"
#include "EFT_EnemyAIConfigDataAsset.generated.h"

class UBehaviorTree;

UCLASS(BlueprintType)
class TARKOV_GAS_API UEFT_EnemyAIConfigDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EFT|AI")
	EEFTEnemyType EnemyType{EEFTEnemyType::Generic};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EFT|AI|Behavior")
	TObjectPtr<UBehaviorTree> MasterBehaviorTreeAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EFT|AI|Behavior")
	TObjectPtr<UBehaviorTree> CombatSubtree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EFT|AI|Behavior")
	TObjectPtr<UBehaviorTree> SearchSubtree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EFT|AI|Behavior")
	TObjectPtr<UBehaviorTree> PatrolSubtree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EFT|AI|Perception")
	float SightRadius{1500.f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EFT|AI|Perception")
	float LoseSightSeconds{3.f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EFT|AI|Combat")
	float AttackRange{250.f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EFT|AI|Combat")
	float AcceptanceRadius{500.f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EFT|AI|Combat")
	float AttackCooldown{1.f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EFT|AI|Patrol")
	float PatrolRadius{1200.f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EFT|AI|Lost Sight")
	float LostSightSearchRadius{600.f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EFT|AI|Lost Sight")
	float LostSightSearchObserveDuration{1.25f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EFT|AI|Lost Sight")
	float LostSightHoldAngleDuration{2.f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EFT|AI|Lost Sight", meta = (ClampMin = "0"))
	int32 MaxLostSightSearches{1};
};
