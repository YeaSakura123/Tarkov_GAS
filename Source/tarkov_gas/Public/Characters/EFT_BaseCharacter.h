// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"

#include "EFT_BaseCharacter.generated.h"

namespace  EFTTags
{
	extern TARKOV_GAS_API const FName Player;
}

struct FOnAttributeChangeData;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FASCInitialized, UAbilitySystemComponent*, ASC, UAttributeSet*, AS);

UCLASS(Abstract)
class TARKOV_GAS_API AEFT_BaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AEFT_BaseCharacter();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributeSet() const {return nullptr;};
	bool IsAlive() const {return bAlive;};
	void SetAlive(bool bAliveStatus) {bAlive = bAliveStatus;};
	
	UPROPERTY(BlueprintAssignable)
	FASCInitialized OnASCInitialized;
	
	UFUNCTION(BlueprintCallable, Category = "EFT|Death")
	virtual void HandleRespawn();
	
	UFUNCTION(BlueprintCallable, Category = "EFT|Attributes")
	void ResetAttributes();
	
	UFUNCTION(BlueprintImplementableEvent)
	void RotateToTarget(AActor* RotateTarget);
	
	UPROPERTY(EditAnywhere, Category = "EFT|AI")
	float SearchRange{1000.f};
protected:
	void GiveStartupAbilities();
	void InitializeAttributes() const;
	void ApplyStaminaRegenEffect();
	
	void OnHealthChanged(const FOnAttributeChangeData& AttributeChangeData);
	virtual void HandleDeath();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EFT|Abilities")
	TSubclassOf<class UGameplayEffect> StaminaRegenEffect;

	bool bStaminaRegenEffectApplied = false;
	
private:

	UPROPERTY(EditDefaultsOnly, Category = "EFT|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
	UPROPERTY(EditDefaultsOnly, Category = "EFT|Effects")
	TSubclassOf<UGameplayEffect> InitializeAttributesEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "EFT|Effects")
	TSubclassOf<UGameplayEffect> ResetAttributesEffect;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Replicated)
	bool bAlive = true;
};
