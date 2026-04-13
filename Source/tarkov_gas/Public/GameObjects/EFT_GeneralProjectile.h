// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EFT_GeneralProjectile.generated.h"

class USphereComponent;
class UGameplayEffect;
class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS()
class TARKOV_GAS_API AEFT_GeneralProjectile : public AActor
{
	GENERATED_BODY()

public:
	AEFT_GeneralProjectile();
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="EFT|Projectile")
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="EFT|Projectile")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="EFT|Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	
	UFUNCTION(BlueprintImplementableEvent, Category="EFT|Projectile")
	void OnProjectileImpact(AActor* OtherActor);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="EFT|Projectile", meta = (ExposeOnSpawn, ClampMin = "0.0")) 
    float InitialVelocity{600.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="EFT|Projectile", meta = (ExposeOnSpawn, ClampMin = "0.0")) 
	float MaxVelocity{3000.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="EFT|Projectile")
	float LifeSeconds{5.0f};
};
	
