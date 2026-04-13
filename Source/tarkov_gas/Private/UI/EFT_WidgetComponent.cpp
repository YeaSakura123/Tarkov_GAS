// YeaSakura123 2025.11


#include "UI/EFT_WidgetComponent.h"

#include "AbilitySystem/EFT_AbilitySystemComponent.h"
#include "AbilitySystem/EFT_AttributeSet.h"
#include "Blueprint/WidgetTree.h"
#include "Characters/EFT_BaseCharacter.h"
#include "UI/EFT_AttributeWidget.h"

void UEFT_WidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	InitAbilitySystemData();
	
	if (!IsASCInitialized())
	{
		EFTCharacter->OnASCInitialized.AddDynamic(this, &ThisClass::OnASCInitialized);
		return;
	}
	
	InitializedAttributeDelegate();
}


void UEFT_WidgetComponent::InitAbilitySystemData()
{
	EFTCharacter = Cast<AEFT_BaseCharacter>(GetOwner());
	AttributeSet = Cast<UEFT_AttributeSet>(EFTCharacter->GetAttributeSet());
	AbilitySystemComponent = Cast<UEFT_AbilitySystemComponent>( EFTCharacter->GetAbilitySystemComponent());	
}

bool UEFT_WidgetComponent::IsASCInitialized() const
{
	return AbilitySystemComponent.IsValid() && AttributeSet.IsValid();
}

void UEFT_WidgetComponent::InitializedAttributeDelegate()
{
	if (!AttributeSet->bAttributesInitialized)
	{
		AttributeSet->OnAttributesInitialized.AddDynamic(this, &ThisClass::BindToAttributeChanges);
	}
	else
	{
		BindToAttributeChanges();
	}
}

void UEFT_WidgetComponent::OnASCInitialized(UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	
	AbilitySystemComponent = Cast<UEFT_AbilitySystemComponent>(ASC);
	AttributeSet = Cast<UEFT_AttributeSet>(AS);
	
	if (!IsASCInitialized()) return;
	InitializedAttributeDelegate();
}

void UEFT_WidgetComponent::BindToAttributeChanges()
{
	for (const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair : AttributeMap)
	{
		BindWidgetToAttributeChanges(GetUserWidgetObject(), Pair);
		GetUserWidgetObject()->WidgetTree->ForEachWidget([this, &Pair](UWidget* ChildWidget)
		{
			BindWidgetToAttributeChanges(ChildWidget, Pair);
		});
	}
}

void UEFT_WidgetComponent::BindWidgetToAttributeChanges(UWidget* WidgetObject, const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const
{
	UEFT_AttributeWidget* AttributeWidget = Cast<UEFT_AttributeWidget>(WidgetObject);
	if (!IsValid(AttributeWidget)) return;
	if (!AttributeWidget->MatchesAttributes(Pair)) return;
	AttributeWidget->AvatarActor = EFTCharacter;	
	
	AttributeWidget->OnAttributeChange(Pair, AttributeSet.Get(), 0.f);
		
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Key).AddLambda([this, AttributeWidget, &Pair](const FOnAttributeChangeData& AttributeChangeData)
	{
		AttributeWidget->OnAttributeChange(Pair, AttributeSet.Get(), AttributeChangeData.OldValue);
	});
}