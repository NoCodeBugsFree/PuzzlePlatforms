// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuWidgetBase.h"

void UMenuWidgetBase::Setup()
{
	AddToViewport();

	UWorld* World = GetWorld();
	if (World)
	{
		/** get player controller  */
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			/** UI Only */
			FInputModeUIOnly InputModeUIOnly;
			InputModeUIOnly.SetWidgetToFocus(TakeWidget());
			InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeUIOnly);
			PlayerController->bShowMouseCursor = true;
		}
	}
}

void UMenuWidgetBase::TearDown()
{
	RemoveFromParent();

	UWorld* World = GetWorld();
	if (World)
	{
		/** get player controller  */
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			/** Game Only */
			FInputModeGameOnly InputModeGameOnly;
			PlayerController->SetInputMode(InputModeGameOnly);
			PlayerController->bShowMouseCursor = false;
		}
	}
}
