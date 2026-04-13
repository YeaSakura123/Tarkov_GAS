// YeaSakura123 2025.11


#include "Characters/EFT_PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/EFT_AttributeSet.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/EFT_PlayerState.h"


// Sets default values
AEFT_PlayerCharacter::AEFT_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	bUseControllerRotationPitch = false;
	//bUseControllerRotationYaw = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	//GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.0f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	/*
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	*/
	CameraBoom->SetupAttachment(GetMesh(), FName("head"));
	CameraBoom->TargetArmLength = 0.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	
	Tags.Add(EFTTags::Player);
}

UAbilitySystemComponent* AEFT_PlayerCharacter::GetAbilitySystemComponent() const
{
	AEFT_PlayerState* EFTPlayerState = Cast<AEFT_PlayerState>(GetPlayerState());
	if (!IsValid(EFTPlayerState)) return nullptr;

	return EFTPlayerState->GetAbilitySystemComponent();
}

UAttributeSet* AEFT_PlayerCharacter::GetAttributeSet() const
{
	AEFT_PlayerState* EFTPlayerState = Cast<AEFT_PlayerState>(GetPlayerState());
	if (!IsValid(EFTPlayerState)) return nullptr;
	
	return EFTPlayerState->GetAttributeSet();
}

void AEFT_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!IsValid(GetAbilitySystemComponent()) || !HasAuthority()) return;

	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);
	ApplyStaminaRegenEffect();
	OnASCInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributeSet());
	GiveStartupAbilities();
	InitializeAttributes();
	
	UEFT_AttributeSet* EFT_AttributeSet = Cast<UEFT_AttributeSet>(GetAttributeSet());
	if (!IsValid(EFT_AttributeSet)) return;
	
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(EFT_AttributeSet->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
}

void AEFT_PlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (!IsValid(GetAbilitySystemComponent())) return;

	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);
	ApplyStaminaRegenEffect();
	OnASCInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributeSet());
	
	UEFT_AttributeSet* EFT_AttributeSet = Cast<UEFT_AttributeSet>(GetAttributeSet());
	if (!IsValid(EFT_AttributeSet)) return;
	
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(EFT_AttributeSet->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
}


