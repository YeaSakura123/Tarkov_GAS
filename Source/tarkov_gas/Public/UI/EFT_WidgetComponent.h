// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Components/WidgetComponent.h"
#include "EFT_WidgetComponent.generated.h"

class UAbilitySystemComponent;
class UEFT_AttributeSet;
class UEFT_AbilitySystemComponent;
class AEFT_BaseCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TARKOV_GAS_API UEFT_WidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	TMap<FGameplayAttribute, FGameplayAttribute> AttributeMap;
	
private:
	TWeakObjectPtr<AEFT_BaseCharacter> EFTCharacter;
	TWeakObjectPtr<UEFT_AbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<UEFT_AttributeSet> AttributeSet;
	
	
	void InitAbilitySystemData();
	bool IsASCInitialized() const;
	void InitializedAttributeDelegate();
	void BindWidgetToAttributeChanges(UWidget* WidgetObject, const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const;
	
	UFUNCTION()
	void OnASCInitialized(UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
	UFUNCTION()
	void BindToAttributeChanges();
};
