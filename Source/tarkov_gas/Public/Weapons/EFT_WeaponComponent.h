// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EFT_WeaponComponent.generated.h"

class AEFT_BaseCharacter;
class UEFT_WeaponDataAsset;

USTRUCT(BlueprintType)
struct FEFTWeaponTraceResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool bHit{false};

	UPROPERTY(BlueprintReadOnly)
	FHitResult HitResult;

	UPROPERTY(BlueprintReadOnly)
	FVector TraceStart{FVector::ZeroVector};

	UPROPERTY(BlueprintReadOnly)
	FVector TraceEnd{FVector::ZeroVector};
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TARKOV_GAS_API UEFT_WeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEFT_WeaponComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "EFT|Weapon")
	void SetCurrentWeaponData(UEFT_WeaponDataAsset* NewWeaponData);

	UFUNCTION(BlueprintPure, Category = "EFT|Weapon")
	UEFT_WeaponDataAsset* GetCurrentWeaponData() const { return CurrentWeaponData; }

	UFUNCTION(BlueprintPure, Category = "EFT|Weapon")
	bool CanFire(float CurrentTime) const;

	UFUNCTION(BlueprintCallable, Category = "EFT|Weapon")
	void MarkFired(float CurrentTime);

	bool GetFireStartAndDirection(FVector& OutStart, FVector& OutDirection) const;
	FEFTWeaponTraceResult PerformFireTrace() const;

private:
	AEFT_BaseCharacter* GetOwningCharacter() const;
	AActor* GetCombatTarget() const;

	UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadOnly, Category = "EFT|Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UEFT_WeaponDataAsset> CurrentWeaponData;

	UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadOnly, Category = "EFT|Weapon", meta = (AllowPrivateAccess = "true"))
	float LastFireTime{-1000000.f};
};
