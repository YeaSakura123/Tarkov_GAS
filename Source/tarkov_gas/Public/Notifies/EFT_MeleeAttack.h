// YeaSakura123 2025.11

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "EFT_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class TARKOV_GAS_API UEFT_MeleeAttack : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	
private:
	UPROPERTY(EditAnywhere, Category="EFT|Debug")
	bool bDrawDebugs = true;
	
	UPROPERTY(EditAnywhere, Category="EFT|Socket")
	FName SocketName{"FX_Trail_01_R"};
	
	UPROPERTY(EditAnywhere, Category="EFT|Socket")
	float SocketExtensionOffest{40.f};
	
	UPROPERTY(EditAnywhere, Category="EFT|Socket")
	float SphereTraceRadius{60.f};
	
	TArray<FHitResult> PerformSphereTrace(USkeletalMeshComponent* MeshComp) const;
	void SendEventsToActors(USkeletalMeshComponent* MeshComp, const TArray<FHitResult>& Hits) const;
};
