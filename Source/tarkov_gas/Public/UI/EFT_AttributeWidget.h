// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystem/EFT_AttributeSet.h"

#include "EFT_AttributeWidget.generated.h"


UCLASS()
class TARKOV_GAS_API UEFT_AttributeWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="EFT|Attributes")
	FGameplayAttribute Attribute;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="EFT|Attributes")
	FGameplayAttribute MaxAttribute;
	
	void OnAttributeChange(const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair, UEFT_AttributeSet* AttributeSet, float OldValue);
	bool MatchesAttributes(const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const;
	
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="On Attribute Change"))
	void BP_OnAttributeChange(float NewValue, float NewMaxValue, float OldValue);
	
	UPROPERTY(BlueprintReadOnly, Category = "EFT|Attributes")
	TWeakObjectPtr<AActor> AvatarActor;
};
