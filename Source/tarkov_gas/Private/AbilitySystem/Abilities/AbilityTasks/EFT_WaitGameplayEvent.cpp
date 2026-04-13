// YeaSakura123 2025.11


#include "AbilitySystem/Abilities/AbilityTasks/EFT_WaitGameplayEvent.h"

UEFT_WaitGameplayEvent* UEFT_WaitGameplayEvent::WaitGameplayEventToActorProxy(AActor* TargetActor, FGameplayTag EventTag, bool OnlyTriggerOnce, bool OnlyMatchExact)
{
	UEFT_WaitGameplayEvent* MyObj = NewObject<UEFT_WaitGameplayEvent>();
	MyObj->SetAbilityActor(TargetActor);
	MyObj->Tag = EventTag;
	MyObj->OnlyTriggerOnce = OnlyTriggerOnce;
	MyObj->OnlyMatchExact = OnlyMatchExact;
	return MyObj;
}

void UEFT_WaitGameplayEvent::StartActivation()
{
	Activate();
}
