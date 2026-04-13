// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/EFT_GameplayAbility.h"
#include "EFT_Primary.generated.h"

/**
 * 
 */
UCLASS()
class TARKOV_GAS_API UEFT_Primary : public UEFT_GameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="EFT|Abilities")
	void SendHitReactEventToActors(const TArray<AActor*>& ActorsHit);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="EFT|Abilities")
	float HitBoxRadius = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="EFT|Abilities")
	float HitBoxForwardOffset = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="EFT|Abilities")
	float HitBoxElevationOffset = 20.0f;
	
};
