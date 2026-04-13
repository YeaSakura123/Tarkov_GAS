#include "GameplayTags/EFTTags.h"

namespace EFTTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(None, "EFTTags.None", "None");
	namespace SetByCaller
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Projectile, "EFTTags.SetByCaller.Projectile", "Tag for Set by Caller Magnitude for Projectiles");
	}

	namespace Status
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Sprinting, "EFTTags.Status.Sprinting", "Tag for Check is running");
	}
	namespace EFTAbilities
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActivateOnGiven, "EFTTags.EFTAbilities.ActivateOnGiven", "Tag for Abilities should be activate immediately once given")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Run, "EFTTags.EFTAbilities.Run", "Tag for Run Ability")
		
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Primary, "EFTTags.EFTAbilities.Primary", "Tag for Primary Ability");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Secondary, "EFTTags.EFTAbilities.Secondary", "Tag for Secondary Ability")
		namespace Enemy
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attack, "EFTTags.EFTAbilities.Enemy.Attack", "Tag for Enemy Attack Ability");
		}
	}
	namespace Events
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(KillScored, "EFTTags.Events.KillScored", "Tag for KillScored Event");

		namespace Player
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(HitReact, "EFTTags.Events.Player.HitReact", "Tag for Player HitReact Event");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Death, "EFTTags.Events.Player.Death", "Tag for Player Death Event");
		}
		namespace Enemy
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(HitReact, "EFTTags.Events.Enemy.HitReact", "Tag for Enemy HitReact Event");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(EndAttack, "EFTTags.Events.Enemy.EndAttack", "Tag for Enemy EndAttack Event");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(MeleeTraceHit, "EFTTags.Events.Enemy.MeleeTraceHit", "Tag for Enemy Melee Trace Hit");
		}
	}
}