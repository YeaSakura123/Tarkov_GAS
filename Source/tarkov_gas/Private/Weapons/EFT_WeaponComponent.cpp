// YeaSakura123 2025.11

#include "Weapons/EFT_WeaponComponent.h"

#include "DrawDebugHelpers.h"
#include "Characters/EFT_BaseCharacter.h"
#include "Characters/EFT_EnemyCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Weapons/EFT_WeaponDataAsset.h"

UEFT_WeaponComponent::UEFT_WeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UEFT_WeaponComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentWeaponData);
	DOREPLIFETIME(ThisClass, LastFireTime);
}

void UEFT_WeaponComponent::SetCurrentWeaponData(UEFT_WeaponDataAsset* NewWeaponData)
{
	CurrentWeaponData = NewWeaponData;
}

bool UEFT_WeaponComponent::CanFire(float CurrentTime) const
{
	const AEFT_BaseCharacter* Character = GetOwningCharacter();
	if (!IsValid(Character) || !Character->IsAlive() || !IsValid(CurrentWeaponData)) return false;

	return CurrentTime - LastFireTime >= CurrentWeaponData->FireCooldown;
}

void UEFT_WeaponComponent::MarkFired(float CurrentTime)
{
	LastFireTime = CurrentTime;
}

bool UEFT_WeaponComponent::GetFireStartAndDirection(FVector& OutStart, FVector& OutDirection) const
{
	const AEFT_BaseCharacter* Character = GetOwningCharacter();
	if (!IsValid(Character) || !IsValid(CurrentWeaponData)) return false;

	OutStart = Character->GetActorLocation() + FVector(0.f, 0.f, Character->BaseEyeHeight);
	OutDirection = Character->GetActorForwardVector();

	const USkeletalMeshComponent* Mesh = Character->GetMesh();
	if (IsValid(Mesh) && Mesh->DoesSocketExist(CurrentWeaponData->MuzzleSocketName))
	{
		OutStart = Mesh->GetSocketLocation(CurrentWeaponData->MuzzleSocketName);
	}

	if (const AController* Controller = Character->GetController(); IsValid(Controller))
	{
		if (const APlayerController* PlayerController = Cast<APlayerController>(Controller); IsValid(PlayerController))
		{
			FVector ViewLocation;
			FRotator ViewRotation;
			PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);
			OutStart = ViewLocation;
			OutDirection = ViewRotation.Vector();
			return true;
		}
	}

	if (AActor* Target = GetCombatTarget(); IsValid(Target))
	{
		const FVector TargetLocation = Target->GetActorLocation() + FVector(0.f, 0.f, 60.f);
		OutDirection = (TargetLocation - OutStart).GetSafeNormal();
		if (!OutDirection.IsNearlyZero()) return true;
	}

	const FRotator AimRotation = Character->GetBaseAimRotation();
	OutDirection = AimRotation.Vector();
	if (OutDirection.IsNearlyZero())
	{
		OutDirection = Character->GetActorForwardVector();
	}

	return !OutDirection.IsNearlyZero();
}

FEFTWeaponTraceResult UEFT_WeaponComponent::PerformFireTrace() const
{
	FEFTWeaponTraceResult Result;
	const AEFT_BaseCharacter* Character = GetOwningCharacter();
	if (!IsValid(Character) || !IsValid(CurrentWeaponData)) return Result;

	FVector TraceDirection;
	if (!GetFireStartAndDirection(Result.TraceStart, TraceDirection)) return Result;

	Result.TraceEnd = Result.TraceStart + TraceDirection.GetSafeNormal() * CurrentWeaponData->Range;

	UWorld* World = Character->GetWorld();
	if (!IsValid(World)) return Result;

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(EFTWeaponTrace), false);
	QueryParams.AddIgnoredActor(Character);

	if (CurrentWeaponData->TraceRadius > 0.f)
	{
		Result.bHit = World->SweepSingleByChannel(
			Result.HitResult,
			Result.TraceStart,
			Result.TraceEnd,
			FQuat::Identity,
			ECC_Visibility,
			FCollisionShape::MakeSphere(CurrentWeaponData->TraceRadius),
			QueryParams);
	}
	else
	{
		Result.bHit = World->LineTraceSingleByChannel(Result.HitResult, Result.TraceStart, Result.TraceEnd, ECC_Visibility, QueryParams);
	}

	if (CurrentWeaponData->bDrawDebugTrace)
	{
		const FColor DebugColor = Result.bHit ? FColor::Red : FColor::Green;
		DrawDebugLine(World, Result.TraceStart, Result.bHit ? Result.HitResult.ImpactPoint : Result.TraceEnd, DebugColor, false, 2.f, 0, 1.5f);
		if (Result.bHit)
		{
			DrawDebugSphere(World, Result.HitResult.ImpactPoint, 8.f, 12, DebugColor, false, 2.f);
		}
	}

	return Result;
}

AEFT_BaseCharacter* UEFT_WeaponComponent::GetOwningCharacter() const
{
	return Cast<AEFT_BaseCharacter>(GetOwner());
}

AActor* UEFT_WeaponComponent::GetCombatTarget() const
{
	const AEFT_EnemyCharacter* Enemy = Cast<AEFT_EnemyCharacter>(GetOwner());
	return IsValid(Enemy) ? Enemy->GetCombatTarget() : nullptr;
}
