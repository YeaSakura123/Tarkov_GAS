// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EFT_HUD.generated.h"

UCLASS()
class TARKOV_GAS_API UEFT_HUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="EFT|HUD")
	void InitializeHUD(UEFT_HUDViewModel* InViewModel);
    
	UFUNCTION(BlueprintPure, Category="EFT|HUD")
	UEFT_HUDViewModel* GetHUDViewModel() const { return HUDViewModel; }
	
protected:
	virtual void NativeDestruct() override;
	UPROPERTY(BlueprintReadOnly, Category="EFT|HUD")
	TObjectPtr<UEFT_HUDViewModel> HUDViewModel;
	
};
