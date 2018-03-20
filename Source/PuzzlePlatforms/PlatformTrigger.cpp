// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformTrigger.h"
#include "Components/BoxComponent.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "MovingPlatform.h"

// Sets default values
APlatformTrigger::APlatformTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/* box collision  */
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(TriggerVolume);
	TriggerVolume->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	/** mesh  */
	PressurePad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	PressurePad->SetupAttachment(RootComponent);
	PressurePad->SetWorldScale3D(FVector(2.f, 2.f, 0.1f));
	PressurePad->SetRelativeLocation(FVector(0.f, 0.f, -100.f));

	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (Cube.Succeeded())
	{
		PressurePad->SetStaticMesh(Cube.Object);
	}
}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapEnd);
}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlatformTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// [Server]
	if (Role == ROLE_Authority)
	{
		for (AMovingPlatform* MovingPlatform : PlatformsToTrigger)
		{
			MovingPlatform->AddActiveTrigger();
		}
	}
}

void APlatformTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// [Server]
	if (Role == ROLE_Authority)
	{
		for (AMovingPlatform* MovingPlatform : PlatformsToTrigger)
		{
			MovingPlatform->RemoveActiveTrigger();
		}
	}
}
