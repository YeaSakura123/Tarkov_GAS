// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "EFT_HUDSubsystem.generated.h"

class UEFT_HUD;
class UEFT_HUDViewModel;

UCLASS()
class TARKOV_GAS_API UEFT_HUDSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
    
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
    
	UFUNCTION(BlueprintCallable, Category="EFT|HUD")
	void CreateHUD();
    
	UFUNCTION(BlueprintCallable, Category="EFT|HUD")
	void DestroyHUD();
    
	UFUNCTION(BlueprintCallable, Category="EFT|HUD")
	void ShowHUD();
    
	UFUNCTION(BlueprintCallable, Category="EFT|HUD")
	void HideHUD();
    
	UFUNCTION(BlueprintPure, Category="EFT|HUD")
	UEFT_HUD* GetHUD() const { return HUDWidget; }
    
	UFUNCTION(BlueprintPure, Category="EFT|HUD")
	UEFT_HUDViewModel* GetHUDViewModel() const { return HUDViewModel; }
    
private:
	UPROPERTY()
	TSubclassOf<UEFT_HUD> HUDWidgetClass;
    
	UPROPERTY()
	TObjectPtr<UEFT_HUD> HUDWidget;
    
	UPROPERTY()
	TObjectPtr<UEFT_HUDViewModel> HUDViewModel;
    
	FTimerHandle SetupTimerHandle;
	bool bIsSetupComplete = false;
    
	void TrySetup();
    
	UFUNCTION()
	void OnPlayerPawnChanged(APawn* OldPawn, APawn* NewPawn);
};

