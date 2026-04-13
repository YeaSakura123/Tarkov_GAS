// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EFT_AnimInstance.generated.h"

class AEFT_BaseCharacter;
class UCharacterMovementComponent;

UCLASS()
class TARKOV_GAS_API UEFT_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	
	UPROPERTY(BlueprintReadWrite, Category="EFT|Anim")
	AEFT_BaseCharacter* BaseCharacter;
	
	UPROPERTY(BlueprintReadWrite, Category="EFT|Anim")
	UCharacterMovementComponent* MoveComp;
	
	UPROPERTY(BlueprintReadWrite, Category="EFT|Anim")
	bool bIsFalling;
	
	UPROPERTY(BlueprintReadWrite, Category="EFT|Anim")
	FRotator SpineRotator;
	
	UPROPERTY(BlueprintReadWrite, Category="EFT|Anim")
	FVector Velocity;
	
	UPROPERTY(BlueprintReadWrite, Category="EFT|Anim")
	float GroundSpeed;
	
	UPROPERTY(BlueprintReadWrite, Category="EFT|Anim")
	float Direction;
	
	UPROPERTY(BlueprintReadWrite, Category="EFT|Anim")
	bool bShouldMove;
	
	UPROPERTY(BlueprintReadWrite, Category="EFT|Anim")
	float MagicNumber{0.01f};
	
private:
	void SetVelocityAndGroundSpeed();
	void SetShouldMove();
	void SetIsFalling();
	void SetSpineRotator();
};
