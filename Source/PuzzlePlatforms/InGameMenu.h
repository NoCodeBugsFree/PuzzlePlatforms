// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidgetBase.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UInGameMenu : public UMenuWidgetBase
{
	GENERATED_BODY()
	
public:

	/** calls to initialize the widget  */
	virtual bool Initialize() override;

private:

	/**  cancel button reference  */
	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;
	
	/**  quit button reference  */
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;
	
	/** calls to quit the game */
	UFUNCTION()
	void OnQuitButtonClicked();
	
	
};
