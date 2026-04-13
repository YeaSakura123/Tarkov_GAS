// YeaSakura123 2025.11

#include "Tasks/EFT_AttributeChangeTask.h"
#include "AbilitySystemComponent.h"

UEFT_AttributeChangeTask* UEFT_AttributeChangeTask::ListenForAttributeChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAttribute Attribute)
{
	UEFT_AttributeChangeTask* WaitForAttributeChangeTask = NewObject<UEFT_AttributeChangeTask>();
	WaitForAttributeChangeTask->ASC = AbilitySystemComponent;
	WaitForAttributeChangeTask->AttributeToListenFor = Attribute;
	
	if (!IsValid(AbilitySystemComponent))
	{
		WaitForAttributeChangeTask->RemoveFromRoot();
		return nullptr;
	}
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(WaitForAttributeChangeTask, &UEFT_AttributeChangeTask::AttributeChanged);
	
	return WaitForAttributeChangeTask;
}

void UEFT_AttributeChangeTask::EndTask()
{
	if (ASC.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(AttributeToListenFor).RemoveAll(this);
	}
	
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UEFT_AttributeChangeTask::AttributeChanged(const FOnAttributeChangeData& Data)
{
	OnAttributeChanged.Broadcast(Data.Attribute, Data.NewValue, Data.OldValue);
}
