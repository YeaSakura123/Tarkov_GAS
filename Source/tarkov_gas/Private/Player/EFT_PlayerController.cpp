// YeaSakura123 2025.11


#include "Player/EFT_PlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Characters/EFT_BaseCharacter.h"
#include "GameFramework/Character.h"
#include "GameplayTags/EFTTags.h"
#include "UI/EFT_HUD.h"
#include "UI/EFT_HUDSubsystem.h"

UEFT_HUD* AEFT_PlayerController::GetEFTHUD() const
{
	if (const ULocalPlayer* LP = GetLocalPlayer())
	{
		if (const UEFT_HUDSubsystem* HUDSubsystem = LP->GetSubsystem<UEFT_HUDSubsystem>())
		{
			return HUDSubsystem->GetHUD();
		}
	}
	return nullptr;
}

UEFT_HUDViewModel* AEFT_PlayerController::GetHUDViewModel() const
{
	if (const ULocalPlayer* LP = GetLocalPlayer())
	{
		if (const UEFT_HUDSubsystem* HUDSubsystem = LP->GetSubsystem<UEFT_HUDSubsystem>())
		{
			return HUDSubsystem->GetHUDViewModel();
		}
	}
	return nullptr;
}

void AEFT_PlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AEFT_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* InputSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!IsValid(InputSubSystem)) return;
	
	for (UInputMappingContext* Context : InputMappingContexts)
	{
		InputSubSystem->AddMappingContext(Context, 0);
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!IsValid(EnhancedInputComponent)) return;

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ThisClass::StopSprinting);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Triggered, this, &ThisClass::Primary);
	EnhancedInputComponent->BindAction(SecondaryAction, ETriggerEvent::Started, this, &ThisClass::Secondary);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &ThisClass::Run);

}

void AEFT_PlayerController::Jump()
{
	if (!IsValid(GetCharacter())) return;
	if (!IsAlive()) return;
	
	GetCharacter()->Jump();
}

void AEFT_PlayerController::StopJumping()
{
	if (!IsValid(GetCharacter())) return;
	if (!IsAlive()) return;

	GetCharacter()->StopJumping();
}

void AEFT_PlayerController::Move(const FInputActionValue& Value)
{
	if (!IsValid(GetPawn())) return;
	if (!IsAlive()) return;

	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (MovementVector.Y <= 0.1f) 
	{
		StopSprinting();
	}

	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	GetPawn()->AddMovementInput(ForwardDirection, MovementVector.Y);
	GetPawn()->AddMovementInput(RightDirection, MovementVector.X);
}

void AEFT_PlayerController::Look(const FInputActionValue& Value)
{
	if (!IsAlive()) return;
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}

void AEFT_PlayerController::Primary()
{
	ActivateAbility(EFTTags::EFTAbilities::Primary);
}

void AEFT_PlayerController::Secondary()
{
	ActivateAbility(EFTTags::EFTAbilities::Secondary);
}

void AEFT_PlayerController::StopSprinting()
{
	if (!IsAlive()) return;
	APawn* ControlledPawn = GetPawn();
	if (!IsValid(ControlledPawn)) return;

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControlledPawn);
	if (!IsValid(ASC)) return;

	const FGameplayTag SprintStatusTag = EFTTags::Status::Sprinting;
	const FGameplayTag RunAbilityTag = EFTTags::EFTAbilities::Run;

	if (ASC->HasMatchingGameplayTag(SprintStatusTag))
	{
		FGameplayTagContainer AbilityTags;
		AbilityTags.AddTag(RunAbilityTag);
		ASC->CancelAbilities(&AbilityTags);
		ASC->RemoveActiveEffectsWithGrantedTags(SprintStatusTag.GetSingleTagContainer());
	}
}

void AEFT_PlayerController::Run()
{
	if (!IsAlive()) return;

	APawn* ControlledPawn = GetPawn();
	if (!IsValid(ControlledPawn)) return;

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControlledPawn);
	if (!IsValid(ASC)) return;

	const FGameplayTag RunAbilityTag = EFTTags::EFTAbilities::Run;
	const FGameplayTag SprintStatusTag = EFTTags::Status::Sprinting;

	if (ASC->HasMatchingGameplayTag(SprintStatusTag))
	{
		StopSprinting();
		return;
	}

	const FVector LastInputVector = ControlledPawn->GetLastMovementInputVector();
	const FVector ForwardVector = ControlledPawn->GetActorForwardVector();
	const bool bIsMovingForward = (LastInputVector | ForwardVector) > 0.1f;

	if (bIsMovingForward)
	{
		ASC->TryActivateAbilitiesByTag(RunAbilityTag.GetSingleTagContainer());
	}
}

void AEFT_PlayerController::ActivateAbility(const FGameplayTag& AbilityTag) const
{
	if (!IsAlive()) return;
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
	if (!IsValid(ASC)) return;

	ASC->TryActivateAbilitiesByTag(AbilityTag.GetSingleTagContainer());
}

void AEFT_PlayerController::SwitchAbilityStateHasTag(const FGameplayTag& AbilityTag, const FGameplayTag& ConditionTag) const
{
	if (!IsAlive()) return;
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
	if (!IsValid(ASC)) return;

	if (ASC->HasMatchingGameplayTag(ConditionTag))
	{
		FGameplayTagContainer AbilityTags;
		AbilityTags.AddTag(AbilityTag);
		ASC->CancelAbilities(&AbilityTags);
	}
	else
	{
		ASC->TryActivateAbilitiesByTag(AbilityTag.GetSingleTagContainer());
	}
}

bool AEFT_PlayerController::IsAlive() const
{
	AEFT_BaseCharacter* BaseCharacter = Cast<AEFT_BaseCharacter>(GetPawn());
	if (!IsValid(BaseCharacter)) return false;
	return BaseCharacter->IsAlive();
}
