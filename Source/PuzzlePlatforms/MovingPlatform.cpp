// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"
#include "ConstructorHelpers.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	
	/**  set the mesh asset */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (Cube.Succeeded())
	{
		GetStaticMeshComponent()->SetStaticMesh(Cube.Object);
	}

	GetStaticMeshComponent()->SetWorldScale3D(FVector(1.f, 1.f, 0.1f));
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	
	// Replication
	SetReplicates(true);
	SetReplicateMovement(true);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	/** GetTransform() -  Get the actor-to-world transform. Get the world location of the TargetLocation */
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);

	GlobalStartLocation = GetActorLocation();
}

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	/** move only if we are a server and active  */
	if (Role == ROLE_Authority && ActveTriggersAmount > 0)
	{
		FVector Location = GetActorLocation();
		float JourneyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
		float JourneyTravelled = (Location - GlobalStartLocation).Size();

		if (JourneyTravelled >= JourneyLength)
		{
			FVector Swap = GlobalStartLocation;
			GlobalStartLocation = GlobalTargetLocation;
			GlobalTargetLocation = Swap;
		}

		FVector Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
		
		Location += Speed * DeltaSeconds * Direction;

		SetActorLocation(Location);
	}
}

void AMovingPlatform::AddActiveTrigger()
{
	ActveTriggersAmount++;
}

void AMovingPlatform::RemoveActiveTrigger()
{
	if (ActveTriggersAmount > 0)
	{
		ActveTriggersAmount--;
	}
}