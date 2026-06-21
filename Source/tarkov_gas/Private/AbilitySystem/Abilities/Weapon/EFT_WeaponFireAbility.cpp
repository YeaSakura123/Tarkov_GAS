// YeaSakura123 2025.11

#include "AbilitySystem/Abilities/Weapon/EFT_WeaponFireAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Characters/EFT_BaseCharacter.h"
#include "GameplayTags/EFTTags.h"
#include "Weapons/EFT_WeaponComponent.h"
#include "Weapons/EFT_WeaponDataAsset.h"

UEFT_WeaponFireAbility::UEFT_WeaponFireAbility()
{
	FGameplayTagContainer AssetTags;
	AssetTags.AddTag(EFTTags::Abilities::Weapon::Fire);
	SetAssetTags(AssetTags);
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UEFT_WeaponFireAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AEFT_BaseCharacter* AvatarCharacter = Cast<AEFT_BaseCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(AvatarCharacter) || !AvatarCharacter->IsAlive() || !AvatarCharacter->HasAuthority())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UEFT_WeaponComponent* WeaponComponent = AvatarCharacter->GetWeaponComponent();
	if (!IsValid(WeaponComponent))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UEFT_WeaponDataAsset* WeaponData = WeaponComponent->GetCurrentWeaponData();
	const UWorld* World = AvatarCharacter->GetWorld();
	if (!IsValid(WeaponData) || !IsValid(World) || !WeaponComponent->CanFire(World->GetTimeSeconds()))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const FEFTWeaponTraceResult TraceResult = WeaponComponent->PerformFireTrace();
	WeaponComponent->MarkFired(World->GetTimeSeconds());

	AEFT_BaseCharacter* HitCharacter = TraceResult.bHit ? Cast<AEFT_BaseCharacter>(TraceResult.HitResult.GetActor()) : nullptr;
	if (IsValid(HitCharacter) && HitCharacter != AvatarCharacter && HitCharacter->IsAlive() && IsValid(WeaponData->DamageEffect))
	{
		UAbilitySystemComponent* TargetASC = HitCharacter->GetAbilitySystemComponent();
		if (IsValid(TargetASC))
		{
			FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();
			ContextHandle.AddInstigator(AvatarCharacter, AvatarCharacter);
			ContextHandle.AddHitResult(TraceResult.HitResult);

			FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(WeaponData->DamageEffect, 1.f, ContextHandle);
			if (SpecHandle.IsValid())
			{
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, EFTTags::SetByCaller::Damage, -WeaponData->Damage);
				TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
