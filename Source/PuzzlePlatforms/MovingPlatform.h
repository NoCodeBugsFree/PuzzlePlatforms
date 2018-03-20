// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:

	/** calls to increment the amount of active triggers   */
	void AddActiveTrigger();

	/** calls to decrement the amount of active triggers   */
	void RemoveActiveTrigger();

protected:

	AMovingPlatform();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
private:

	/** the amount of active triggers  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 ActveTriggersAmount = 0;
	
	/** the speed of this actor  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float Speed = 50.f;

	/** the destination that actor to move to  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
	FVector TargetLocation;

	/** world location of the target point (widget)  */
	FVector GlobalTargetLocation;

	/** world location of that actor when it spawns  */
	FVector GlobalStartLocation;
	
};
