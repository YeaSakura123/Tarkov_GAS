// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "MVVMViewModelBase.h"
#include "EFT_AttributeViewModel.generated.h"

struct FOnAttributeChangeData;
class UAbilitySystemComponent;

UCLASS(BlueprintType)
class TARKOV_GAS_API UEFT_AttributeViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Category="Attribute")
	float CurrentValue = 0.f;
    
	UPROPERTY(BlueprintReadOnly, FieldNotify, Category="Attribute")
	float MaxValue = 1.f;
    
	UPROPERTY(BlueprintReadOnly, FieldNotify, Category="Attribute")
	float Percent = 0.f;
    
	UPROPERTY(BlueprintReadOnly, FieldNotify, Category="Attribute")
	FText DisplayText;
	
	void Initialize(UAbilitySystemComponent* ASC, 
					const FGameplayAttribute& InAttribute, 
					const FGameplayAttribute& InMaxAttribute);
	
	void Uninitialize();
	
private:
	void SetCurrentValue(float NewValue);
	void SetMaxValue(float NewValue);
	void UpdateDerivedValues();
    
	void OnAttributeChanged(const FOnAttributeChangeData& Data);
	void OnMaxAttributeChanged(const FOnAttributeChangeData& Data);
	TWeakObjectPtr<UAbilitySystemComponent> CachedASC;
	FGameplayAttribute BoundAttribute;
	FGameplayAttribute BoundMaxAttribute;
    
	FDelegateHandle AttributeChangedHandle;
	FDelegateHandle MaxAttributeChangedHandle;
};
