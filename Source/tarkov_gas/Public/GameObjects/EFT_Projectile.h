// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EFT_Projectile.generated.h"

class UProjectileMovementComponent;
class UGameplayEffect;

UCLASS()
class TARKOV_GAS_API AEFT_Projectile : public AActor
{
	GENERATED_BODY()

public:
	AEFT_Projectile();
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="EFT|Damage", meta = (ExposeOnSpawn, ClampMin = "0.0")) 
	float Damage{-25.f};
	
	UFUNCTION(BlueprintImplementableEvent, Category="EFT|Projectile")
	void SpawnImpactEffects();
private: 
	UPROPERTY(VisibleAnywhere, Category="EFT|Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	
	UPROPERTY(EditDefaultsOnly, Category="EFT|Damage")
	TSubclassOf<UGameplayEffect> DamageEffect;
};
