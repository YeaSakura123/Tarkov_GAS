// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "EFT_BaseCharacter.h"

#include "EFT_PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class TARKOV_GAS_API AEFT_PlayerCharacter : public AEFT_BaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEFT_PlayerCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual UAttributeSet* GetAttributeSet() const override;
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;
};

