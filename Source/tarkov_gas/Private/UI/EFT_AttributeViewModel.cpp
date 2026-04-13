// YeaSakura123 2025.11


#include "UI/EFT_AttributeViewModel.h"
#include "AbilitySystemComponent.h"

void UEFT_AttributeViewModel::Initialize(UAbilitySystemComponent* ASC,
                                          const FGameplayAttribute& InAttribute,
                                          const FGameplayAttribute& InMaxAttribute)
{
    if (!ASC) return;

    // 允许同一个 ViewModel 重新绑定到新的 ASC，但必须先移除旧委托。
    Uninitialize();
    
    CachedASC = ASC;
    BoundAttribute = InAttribute;
    BoundMaxAttribute = InMaxAttribute;
    
    bool bFound = false;
    const float InitialValue = ASC->GetGameplayAttributeValue(InAttribute, bFound);
    if (bFound) SetCurrentValue(InitialValue);
    
    const float InitialMaxValue = ASC->GetGameplayAttributeValue(InMaxAttribute, bFound);
    if (bFound) SetMaxValue(InitialMaxValue);
    
    AttributeChangedHandle = ASC->GetGameplayAttributeValueChangeDelegate(InAttribute)
        .AddUObject(this, &ThisClass::OnAttributeChanged);
    
    MaxAttributeChangedHandle = ASC->GetGameplayAttributeValueChangeDelegate(InMaxAttribute)
        .AddUObject(this, &ThisClass::OnMaxAttributeChanged);
}

void UEFT_AttributeViewModel::Uninitialize()
{
    if (CachedASC.IsValid())
    {
        CachedASC->GetGameplayAttributeValueChangeDelegate(BoundAttribute)
            .Remove(AttributeChangedHandle);
        CachedASC->GetGameplayAttributeValueChangeDelegate(BoundMaxAttribute)
            .Remove(MaxAttributeChangedHandle);
    }

    AttributeChangedHandle.Reset();
    MaxAttributeChangedHandle.Reset();
    BoundAttribute = FGameplayAttribute();
    BoundMaxAttribute = FGameplayAttribute();
    CachedASC = nullptr;
}

void UEFT_AttributeViewModel::SetCurrentValue(float NewValue)
{
    if (UE_MVVM_SET_PROPERTY_VALUE(CurrentValue, NewValue))
    {
        UpdateDerivedValues();
    }
}

void UEFT_AttributeViewModel::SetMaxValue(float NewValue)
{
    if (UE_MVVM_SET_PROPERTY_VALUE(MaxValue, NewValue))
    {
        UpdateDerivedValues();
    }
}

void UEFT_AttributeViewModel::UpdateDerivedValues()
{
    const float NewPercent = (MaxValue > 0.f) ? FMath::Clamp(CurrentValue / MaxValue, 0.f, 1.f) : 0.f;
    UE_MVVM_SET_PROPERTY_VALUE(Percent, NewPercent);
    
    const FText NewText = FText::Format(
        NSLOCTEXT("Attributes", "ValueFormat", "{0} / {1}"),
        FText::AsNumber(FMath::RoundToInt(CurrentValue)),
        FText::AsNumber(FMath::RoundToInt(MaxValue))
    );
    UE_MVVM_SET_PROPERTY_VALUE(DisplayText, NewText);
}

void UEFT_AttributeViewModel::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
    SetCurrentValue(Data.NewValue);
}

void UEFT_AttributeViewModel::OnMaxAttributeChanged(const FOnAttributeChangeData& Data)
{
    SetMaxValue(Data.NewValue);
}
