// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EFT_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UEFT_HUD;
class UEFT_HUDViewModel;
struct FInputActionValue;
struct FGameplayTag;

UCLASS()
class TARKOV_GAS_API AEFT_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category="EFT|HUD")
	UEFT_HUD* GetEFTHUD() const;
    
	UFUNCTION(BlueprintPure, Category="EFT|HUD")
	UEFT_HUDViewModel* GetHUDViewModel() const;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	
	UPROPERTY(EditDefaultsOnly, Category = "EFT|Input")
	TArray<TObjectPtr<UInputMappingContext>> InputMappingContexts;

	UPROPERTY(EditDefaultsOnly, Category = "EFT|Input|Movement")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "EFT|Input|Movement")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "EFT|Input|Movement")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "EFT|Input|Abilities")
	TObjectPtr<UInputAction> PrimaryAction;

	UPROPERTY(EditDefaultsOnly, Category = "EFT|Input|Abilities")
	TObjectPtr<UInputAction> SecondaryAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "EFT|Input|Abilities")
	TObjectPtr<UInputAction> RunAction;

	void Jump();
	void StopJumping();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Primary();
	void Secondary();
	void StopSprinting();
	void Run();
	void ActivateAbility(const FGameplayTag& AbilityTag) const;
	void SwitchAbilityStateHasTag(const FGameplayTag& AbilityTag, const FGameplayTag& ConditionTag) const;
	void TestProjectile();
	
	bool IsAlive() const;
};
