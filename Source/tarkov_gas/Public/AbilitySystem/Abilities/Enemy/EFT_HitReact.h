// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/EFT_GameplayAbility.h"
#include "EFT_HitReact.generated.h"

UCLASS()
class TARKOV_GAS_API UEFT_HitReact : public UEFT_GameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "EFT|Abilities")
	void CashHitDirectionVectors(AActor* Instigator);
	
	UPROPERTY(BlueprintReadOnly, Category = "EFT|Abilities")
	FVector AvatarForward;
	
	UPROPERTY(BlueprintReadOnly, Category = "EFT|Abilities")
	FVector ToInstigator;
};
