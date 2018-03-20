// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidgetBase.h"
#include "MainMenu.generated.h"

USTRUCT()
struct FServerData
{
	GENERATED_USTRUCT_BODY()

	FString Name;

	FString HostUsername;

	uint16 CurrentPlayers;

	uint16 MaxPlayers;
};


/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidgetBase
{
	GENERATED_BODY()

public:

	UMainMenu();

	/** calls to create server line widgets and add them to server's scroll box  */
	void SetServerList(TArray<FServerData>& ServerNames);

	/** calls to create server line widgets and add them to server's scroll box  */
	void SelectIndex(uint32 Index);
	
protected:

	/** calls when the widget is initialized  */
	virtual bool Initialize() override;

private:

	/** server line	widget class template */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UServerLine> ServerLineWidgetClass;
	
	// -----------------------------------------------------------------------------------

	/** calls to highlight server rows children colors according to their state  */
	void UpdateChildren();

	/** calls to load a host menu a server */
	UFUNCTION()
	void OnHostButtonClicked();

	/** calls to open a host game menu */
	UFUNCTION()
	void OpenHostMenu();

	/** calls to switch widget switcher to join menu  */
	UFUNCTION()
	void OnJoinButtonMMClicked();

	/** calls to populate server list if we have servers to join */
	UFUNCTION()
	void OnJoinButtonJMClicked();

	/** calls to switch widget to main menu */
	UFUNCTION()
	void OnCancelButtonClicked();

	/** calls to exit the game  */
	UFUNCTION()
	void OnExitButtonClicked();

	// -----------------------------------------------------------------------------------
	
	/** host button reference  */
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	/** join button in main menu  reference */
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButtonMM;

	/** join button in join menu reference */
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButtonJM;

	/** cancel button in cancel menu reference */
	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;

	/** cancel button in main menu reference */
	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	/** widget switcher reference  */
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WS;

	/** 1st widget in widget switcher reference */
	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	/** 2nd widget in widget switcher reference  */
	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	/** 3nd widget in widget switcher reference  */
	UPROPERTY(meta = (BindWidget))
	class UWidget* HostMenu;

	/** 3nd widget in widget switcher reference  */
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ServerHostName;

	/** cancel button in host menu reference */
	UPROPERTY(meta = (BindWidget))
	class UButton* CancelHostMenuButton;

	/** confirm button in host menu reference */
	UPROPERTY(meta = (BindWidget))
	class UButton* ConfirmHostMenuButton;

	/** scroll box server list widget reference  */
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ServerList;

	/** What is Optional ?
	* When we have an optional value IsSet() returns true, and GetValue() is meaningful.
	* Otherwise GetValue() is not meaningful.
	*/
	/** the index of selected server in server list  */
	TOptional<uint32> SelectedIndex;

	/// DEPRICATED
	/** ip address editable text box reference */
	// UPROPERTY(meta = (BindWidget))
	// class UEditableTextBox* IP_Text;
};
