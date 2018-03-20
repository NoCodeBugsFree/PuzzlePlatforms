// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MenuWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMenuWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/** calls to set interface reference  */
	void SetMenuInterface(IMenuInterface* NewMenuInterface) { MenuInterface = NewMenuInterface; }

	/** calls to add widget to viewport and enable UI input mode  */
	UFUNCTION()
	void Setup();

	/** calls to remove widget from parent and enable Game input mode  */
	UFUNCTION()
	void TearDown();

	// -----------------------------------------------------------------------------------

protected:
	/** reference to game instance that implemented menu interface  */
	class IMenuInterface* MenuInterface;

};
