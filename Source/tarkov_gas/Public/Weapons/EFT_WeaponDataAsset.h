// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EFT_WeaponDataAsset.generated.h"

class UGameplayEffect;

UCLASS(BlueprintType)
class TARKOV_GAS_API UEFT_WeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EFT|Weapon")
	float Damage{25.f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EFT|Weapon")
	float Range{3000.f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EFT|Weapon")
	float FireCooldown{0.2f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EFT|Weapon")
	float TraceRadius{0.f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EFT|Weapon")
	FName MuzzleSocketName{TEXT("Muzzle")};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EFT|Weapon")
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EFT|Weapon")
	bool bDrawDebugTrace{false};
};
