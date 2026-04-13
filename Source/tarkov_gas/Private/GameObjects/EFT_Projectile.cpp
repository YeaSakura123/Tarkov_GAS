// YeaSakura123 2025.11


#include "GameObjects/EFT_Projectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Characters/EFT_PlayerCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameplayTags/EFTTags.h"
#include "Utils/EFT_BlueprintLibrary.h"


AEFT_Projectile::AEFT_Projectile()
{
	PrimaryActorTick.bCanEverTick = false;
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	
	bReplicates = true;
}

void AEFT_Projectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	AEFT_PlayerCharacter* PlayerCharacter = Cast<AEFT_PlayerCharacter>(OtherActor);
	if (!IsValid(PlayerCharacter)) return;
	if (!PlayerCharacter->IsAlive()) return;
	UAbilitySystemComponent* AbilitySystemComponent = PlayerCharacter->GetAbilitySystemComponent();
	if (!IsValid(AbilitySystemComponent) || !HasAuthority()) return;
	
	FGameplayEventData Payload;
	Payload.Instigator = GetOwner();
	Payload.Target = PlayerCharacter;
	
	UEFT_BlueprintLibrary::SendDamageEventToPlayer(PlayerCharacter, DamageEffect, Payload, EFTTags::SetByCaller::Projectile, Damage, EFTTags::None);
	
	SpawnImpactEffects();
	Destroy();
}