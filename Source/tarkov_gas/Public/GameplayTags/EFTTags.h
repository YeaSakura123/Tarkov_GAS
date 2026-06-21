#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace EFTTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(None);
	namespace SetByCaller
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Projectile);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage);
	}
	namespace Status
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sprinting);
	}
	namespace AI
	{
		namespace Subtree
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Combat);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Search);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Patrol);
		}
	}
	namespace EFTAbilities
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActivateOnGiven);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Run);
		
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Primary);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Secondary);
		
		namespace Enemy
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attack);
		}
	}

	namespace Abilities
	{
		namespace Weapon
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Fire);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Reload);
		}
	}

	namespace Events
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(KillScored);

		namespace Player
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReact);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Death);
		}
		
		namespace Enemy
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReact);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(EndAttack);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(MeleeTraceHit);
		}
	}
}
