// YeaSakura123 2025.11


#include "Notifies/EFT_MeleeAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "KismetTraceUtils.h"
#include "ShaderPrintParameters.h"
#include "Characters/EFT_PlayerCharacter.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "GameplayTags/EFTTags.h"
#include "Kismet/KismetMathLibrary.h"

void UEFT_MeleeAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	if (!IsValid(MeshComp)) return;
	if (!IsValid(MeshComp->GetOwner())) return;
	
	TArray<FHitResult> Hits = PerformSphereTrace(MeshComp);
	
	SendEventsToActors(MeshComp, Hits);
}

TArray<FHitResult> UEFT_MeleeAttack::PerformSphereTrace(USkeletalMeshComponent* MeshComp) const
{
	TArray<FHitResult> OutHits;
	
	const FTransform SocketTransform = MeshComp->GetSocketTransform(SocketName);
	const FVector Start = SocketTransform.GetLocation();
	const FVector ExtendedSocketDirection = UKismetMathLibrary::GetForwardVector(SocketTransform.GetRotation().Rotator()) * SocketExtensionOffest;
	const FVector End = Start - ExtendedSocketDirection;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(MeshComp->GetOwner());
	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);
	UWorld* World = GEngine->GetWorldFromContextObject(MeshComp, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World)) return OutHits;
	
	bool const bHit = World->SweepMultiByChannel(
		OutHits, 
		Start, 
		End, 
		FQuat::Identity, 
		ECC_Visibility, 
		FCollisionShape::MakeSphere(SphereTraceRadius), 
		Params, 
		ResponseParams);
	
	if (bDrawDebugs)
	{
		DrawDebugSphereTraceMulti(
			World, 
			Start, 
			End, 
			SphereTraceRadius, 
			EDrawDebugTrace::ForDuration, 
			bHit, 
			OutHits, 
			FColor::Red, 
			FColor::Green, 
			5.0f);
	}
	
	return OutHits;
}

void UEFT_MeleeAttack::SendEventsToActors(USkeletalMeshComponent* MeshComp, const TArray<FHitResult>& Hits) const
{
	for (const FHitResult Hit : Hits)
	{
		AEFT_PlayerCharacter* PlayerCharacter = Cast<AEFT_PlayerCharacter>(Hit.GetActor());
		if (!IsValid(PlayerCharacter)) continue;
		if (!PlayerCharacter->IsAlive()) continue;
		UAbilitySystemComponent* ASC = PlayerCharacter->GetAbilitySystemComponent();
		if (!IsValid(ASC)) continue;
		
		FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
		ContextHandle.AddHitResult(Hit);
		
		FGameplayEventData Payload;
		Payload.Target = PlayerCharacter;
		Payload.ContextHandle = ContextHandle;
		Payload.Instigator = MeshComp->GetOwner();
		
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EFTTags::Events::Enemy::MeleeTraceHit, Payload);
	}
}
