#include "Seojy/Spawner/ResourceSpawner.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "Landscape.h"

// Sets default values
AResourceSpawner::AResourceSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnAreaBox"));
	RootComponent = SpawnArea;

	SpawnArea->SetBoxExtent(FVector(500.f, 500.f, 100.f));
	SpawnArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawnArea->SetHiddenInGame(false);

}

void AResourceSpawner::SpawnResource()
{
	if (!HasAuthority())
		return;

	UWorld* World = GetWorld();
	if (!World || ResourceTypes.Num() == 0)
		return;

	for (int32 i = 0; i < SpawnCount; ++i)
	{
		int32 Index = FMath::RandRange(0, ResourceTypes.Num() - 1);
		TSubclassOf<AResource> ResourceClass = ResourceTypes[Index];

		if (!ResourceClass)
			return;

		FVector SpawnLocation = GetRandomLocationInBox();
		FRotator SpawnRotation = FRotator::ZeroRotator;

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::
			AdjustIfPossibleButAlwaysSpawn;
		Params.Owner = this;

		GetLandScapeHeight(SpawnLocation);

		AResource* Spawned = World->SpawnActor<AResource>(ResourceClass,
			SpawnLocation,
			SpawnRotation,
			Params);
	}
}

// Called when the game starts or when spawned
void AResourceSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		SpawnResource();
	}
}

FVector AResourceSpawner::GetRandomLocationInBox() const
{
	FVector Origin = SpawnArea->GetComponentLocation();
	FVector Extent = SpawnArea->GetScaledBoxExtent();

	return UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
}

void AResourceSpawner::GetLandScapeHeight(FVector& Location) const
{
	FVector TraceStart = Location + FVector(0, 0, 10000.f);
	FVector TraceEnd = Location - FVector(0, 0, 10000.f);
	FHitResult HitResult;

	FCollisionQueryParams Params;
	Params.bTraceComplex = true;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart,TraceEnd, ECC_Visibility, Params))
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && HitActor->IsA(ALandscape::StaticClass()))
		{
			Location.Z = HitResult.Location.Z;
		}
	}


}
