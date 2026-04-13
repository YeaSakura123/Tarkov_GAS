// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "EFT_AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttributesInitialized);

UCLASS()
class TARKOV_GAS_API UEFT_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	UPROPERTY(BlueprintAssignable)
	FAttributesInitialized OnAttributesInitialized;
	
	UPROPERTY(ReplicatedUsing=OnRep_AttributesInitialized)
	bool bAttributesInitialized = false;
	
	UFUNCTION()
	void OnRep_AttributesInitialized();
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HandStamina)
	FGameplayAttributeData HandStamina;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHandStamina)
	FGameplayAttributeData MaxHandStamina;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MoveSpeed)
	FGameplayAttributeData MoveSpeed;
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_HandStamina(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_MaxHandStamina(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	void OnRep_MoveSpeed(const FGameplayAttributeData& OldValue);
	
	ATTRIBUTE_ACCESSORS(ThisClass, Health)
	ATTRIBUTE_ACCESSORS(ThisClass, MaxHealth)
	ATTRIBUTE_ACCESSORS(ThisClass, Mana)
	ATTRIBUTE_ACCESSORS(ThisClass, MaxMana)
	ATTRIBUTE_ACCESSORS(ThisClass, Stamina)
	ATTRIBUTE_ACCESSORS(ThisClass, MaxStamina)
	ATTRIBUTE_ACCESSORS(ThisClass, HandStamina)
	ATTRIBUTE_ACCESSORS(ThisClass, MaxHandStamina)
	ATTRIBUTE_ACCESSORS(ThisClass, MoveSpeed)

private:
	void ClampAttributeValue(const FGameplayAttribute& Attribute, float& NewValue) const;
};
