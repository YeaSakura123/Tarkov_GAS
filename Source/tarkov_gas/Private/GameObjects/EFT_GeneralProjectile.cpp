// YeaSakura123 2025.11


#include "GameObjects/EFT_GeneralProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AEFT_GeneralProjectile::AEFT_GeneralProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	SetRootComponent(CollisionComponent);
	CollisionComponent->InitSphereRadius(25.f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(CollisionComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = InitialVelocity;
	ProjectileMovement->MaxSpeed = MaxVelocity;
	ProjectileMovement->ProjectileGravityScale = 1.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.4f;
	ProjectileMovement->Friction = 0.2f;
	ProjectileMovement->BounceVelocityStopSimulatingThreshold = 5.f;
	ProjectileMovement->bIsHomingProjectile = false;;
}

void AEFT_GeneralProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(ProjectileMovement))
	{
		ProjectileMovement->InitialSpeed = InitialVelocity;
		ProjectileMovement->MaxSpeed = MaxVelocity;
		ProjectileMovement->Velocity = GetActorForwardVector() * InitialVelocity;
	}
	
	SetLifeSpan(LifeSeconds);
}

void AEFT_GeneralProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (!IsValid(OtherActor)) return;
	if (OtherActor == this) return;
	if (OtherActor == GetOwner()) return;
	if (OtherActor == GetInstigator()) return;

	OnProjectileImpact(OtherActor);
	Destroy();
}



