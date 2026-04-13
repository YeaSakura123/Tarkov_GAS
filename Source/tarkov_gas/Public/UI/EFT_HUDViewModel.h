// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "EFT_HUDViewModel.generated.h"

class UEFT_AttributeViewModel;
class UEFT_AbilitySystemComponent;
class UEFT_AttributeSet;

UCLASS()
class TARKOV_GAS_API UEFT_HUDViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	UEFT_HUDViewModel();
	UPROPERTY(BlueprintReadOnly, FieldNotify, Category="EFT|Attributes")
	TObjectPtr<UEFT_AttributeViewModel> HealthViewModel;
    
	UPROPERTY(BlueprintReadOnly, FieldNotify, Category="EFT|Attributes")
	TObjectPtr<UEFT_AttributeViewModel> StaminaViewModel;
    
	UPROPERTY(BlueprintReadOnly, FieldNotify, Category="EFT|Attributes")
	TObjectPtr<UEFT_AttributeViewModel> HandStaminaViewModel;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Category="EFT|State")
	bool bIsHUDVisible = true;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Category="EFT|Player")
	FText PlayerName;
	
	UFUNCTION(BlueprintCallable, Category="EFT|HUD")
	void Uninitialize();
    
	UFUNCTION(BlueprintCallable, Category="EFT|HUD")
	void SetHUDVisible(bool bVisible);
	
	UFUNCTION(BlueprintCallable, Category="EFT|HUD")
	void InitializeWithPlayer(AActor* PlayerActor);
private:
	void CreateAttributeViewModels();
	void BindToAttributeSet(UEFT_AbilitySystemComponent* ASC, UEFT_AttributeSet* AttributeSet);
	
	TWeakObjectPtr<AActor> CachedPlayerActor;
	
};
