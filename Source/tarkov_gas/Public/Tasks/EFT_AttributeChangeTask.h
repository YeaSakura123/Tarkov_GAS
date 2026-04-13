// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "EFT_AttributeChangeTask.generated.h"

struct FOnAttributeChangeData;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, FGameplayAttribute, Attribute, float, NewValue, float, OldValue);

UCLASS(BlueprintType, meta = (ExposedAsyncProxy = AsyncTask))
class TARKOV_GAS_API UEFT_AttributeChangeTask : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnAttributeChanged;
	
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true"))
	static UEFT_AttributeChangeTask* ListenForAttributeChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAttribute Attribute);
	
	UFUNCTION(BlueprintCallable)
	void EndTask();
	
	TWeakObjectPtr<UAbilitySystemComponent> ASC;
	FGameplayAttribute AttributeToListenFor;
	
	void AttributeChanged(const FOnAttributeChangeData& Data);
};
