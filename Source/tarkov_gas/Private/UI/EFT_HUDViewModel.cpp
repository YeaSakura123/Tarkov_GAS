// YeaSakura123 2025.11


#include "UI/EFT_HUDViewModel.h"
#include "UI/EFT_AttributeViewModel.h"
#include "AbilitySystem/EFT_AbilitySystemComponent.h"
#include "AbilitySystem/EFT_AttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"

UEFT_HUDViewModel::UEFT_HUDViewModel()
{
    CreateAttributeViewModels();
}

void UEFT_HUDViewModel::CreateAttributeViewModels()
{
    HealthViewModel = NewObject<UEFT_AttributeViewModel>(this, TEXT("HealthVM"));
    StaminaViewModel = NewObject<UEFT_AttributeViewModel>(this, TEXT("StaminaVM"));
    HandStaminaViewModel = NewObject<UEFT_AttributeViewModel>(this, TEXT("HandStaminaVM"));
}

void UEFT_HUDViewModel::BindToAttributeSet(UEFT_AbilitySystemComponent* ASC, UEFT_AttributeSet* AttributeSet)
{
    if (!ASC || !AttributeSet) return;

    if (HealthViewModel) HealthViewModel->Uninitialize();
    if (StaminaViewModel) StaminaViewModel->Uninitialize();
    if (HandStaminaViewModel) HandStaminaViewModel->Uninitialize();
    
    HealthViewModel->Initialize(ASC, AttributeSet->GetHealthAttribute(), AttributeSet->GetMaxHealthAttribute());
    StaminaViewModel->Initialize(ASC, AttributeSet->GetStaminaAttribute(), AttributeSet->GetMaxStaminaAttribute());
    HandStaminaViewModel->Initialize(ASC, AttributeSet->GetHandStaminaAttribute(), AttributeSet->GetMaxHandStaminaAttribute());

    
    UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(HealthViewModel);
    UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(StaminaViewModel);
    UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(HandStaminaViewModel);
}

void UEFT_HUDViewModel::Uninitialize()
{
    if (HealthViewModel) HealthViewModel->Uninitialize();
    if (StaminaViewModel) StaminaViewModel->Uninitialize();
    if (HandStaminaViewModel) HandStaminaViewModel->Uninitialize();
    
    CachedPlayerActor = nullptr;
}

void UEFT_HUDViewModel::SetHUDVisible(bool bVisible)
{
    UE_MVVM_SET_PROPERTY_VALUE(bIsHUDVisible, bVisible);
}

void UEFT_HUDViewModel::InitializeWithPlayer(AActor* PlayerActor)
{
    if (!PlayerActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("HUDViewModel: PlayerActor is null"));
        return;
    }

    if (CachedPlayerActor.Get() == PlayerActor)
    {
        return;
    }

    Uninitialize();
    
    CachedPlayerActor = PlayerActor;
    
    UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PlayerActor);
    UEFT_AbilitySystemComponent* EFT_ASC = Cast<UEFT_AbilitySystemComponent>(ASC);
    
    if (!EFT_ASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("HUDViewModel: Failed to get EFT_AbilitySystemComponent"));
        return;
    }
    
    UEFT_AttributeSet* AttributeSet = nullptr;
    for (UAttributeSet* AS : ASC->GetSpawnedAttributes())
    {
        if (UEFT_AttributeSet* EFT_AS = Cast<UEFT_AttributeSet>(AS))
        {
            AttributeSet = EFT_AS;
            break;
        }
    }
    
    if (!AttributeSet)
    {
        return;
    }
    
    BindToAttributeSet(EFT_ASC, AttributeSet);
    
    UE_MVVM_SET_PROPERTY_VALUE(PlayerName, FText::FromString(PlayerActor->GetName()));
}
