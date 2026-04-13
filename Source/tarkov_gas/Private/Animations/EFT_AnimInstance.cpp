// YeaSakura123 2025.11


#include "Animations/EFT_AnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "Characters/EFT_PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UEFT_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
}

void UEFT_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	BaseCharacter = Cast<AEFT_PlayerCharacter>(TryGetPawnOwner());
	if (!IsValid(BaseCharacter)) return;
	
    MoveComp = BaseCharacter->GetCharacterMovement();
}

void UEFT_AnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	
	SetVelocityAndGroundSpeed();
	SetShouldMove();
	SetIsFalling();
	SetSpineRotator();
}

void UEFT_AnimInstance::SetVelocityAndGroundSpeed()
{
	if (!IsValid(MoveComp)) return;
	
	Velocity = MoveComp->Velocity;
	GroundSpeed = Velocity.Size2D();

	float RawDirection = UKismetAnimationLibrary::CalculateDirection(Velocity, BaseCharacter->GetActorRotation());
	
	Direction = MoveComp->bOrientRotationToMovement ? FMath::Clamp(RawDirection, -45.f, 45.f) : RawDirection;

}

void UEFT_AnimInstance::SetShouldMove()
{
	if (!IsValid(MoveComp)) return;
	
	if ((MoveComp->GetCurrentAcceleration() != FVector::ZeroVector) && (GroundSpeed > MagicNumber))
	{
		bShouldMove = true;
	}
	else
	{
		bShouldMove = false;
	}
}

void UEFT_AnimInstance::SetIsFalling()
{
	if (!IsValid(MoveComp)) return;
	
	bIsFalling = MoveComp->IsFalling();
}

void UEFT_AnimInstance::SetSpineRotator()
{
	if (!IsValid(BaseCharacter)) return;
	float RawPitch = BaseCharacter->IsLocallyControlled() ? 
					BaseCharacter->GetControlRotation().Pitch : 
					BaseCharacter->GetBaseAimRotation().Pitch;
	
	float FinalValue = (RawPitch > 180.0f ? 
					360.0f - RawPitch : 
					RawPitch * -1.0f) / 5.0f;
						
	FRotator ResultRotator = FRotator(0.0f, 0.0f, FinalValue);
	SpineRotator = ResultRotator;
}
