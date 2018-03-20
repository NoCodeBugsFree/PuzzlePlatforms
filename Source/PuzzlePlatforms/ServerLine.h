// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerLine.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UServerLine : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/** calls to init the widget  */
	virtual bool Initialize() override;

	/** calls from main menu to setup the valid index and parent widget ref */
	void Setup(class UMainMenu* Parent, uint32 Index);

	/** server name text block */
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName;

	/** players text block */
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ConnectionFraction;

	/** Host User text block */
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HostUser;

	/** shows whether this row is selected or not  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	uint32 bSelected : 1;

	// -----------------------------------------------------------------------------------
	
private:

	/** parent widget reference  */
	UPROPERTY()
	class UMainMenu* MainMenu;

	/** the index of this particular server line  */
	UPROPERTY()
	uint32 CurrentIndex = 0;

	/** server line button reference  */
	UPROPERTY(meta = (BindWidget))
	class UButton* ServerLineButton;
	
	/** calls when server line button is clicked */
	UFUNCTION()
	void OnServerLineButtonClicked();
	
};
