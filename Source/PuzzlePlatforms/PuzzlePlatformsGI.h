// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"
#include "MenuInterface.h"
#include "PuzzlePlatformsGI.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGI : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	
	//~ Begin MenuInterface Interface
	
	/** calls to host a new game */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void Host(FString& ServerName) override;

	/** calls to connect to existing server */
	void Join(uint32 Index) override;

	/** calls from Lobby GM to start a session */
	void StartSession();

	/** calls to load a main menu   */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	virtual void LoadMainMenu() override;

	/** calls to refresh server list  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	virtual void RefreshServerList() override;

	//~ End MenuInterface Interface

protected: 
	
	UPuzzlePlatformsGI();

	/**  to sets up what it needs 
	*	@ sets the subsystem reference 
	*	@ binds delegates to handle creating/destroying a session 
	*/
	virtual void Init() override;

	/** calls to load main game menu */
	UFUNCTION(BlueprintCallable, Exec, Category = ExecFunctions)
	void LoadMenuWidget();

	/** calls to load in game menu */
	UFUNCTION(BlueprintCallable, Exec, Category = ExecFunctions)
	void LoadInGameMenu();
	
private:
	
	/**
	* calls by Delegate
	* which fired when the joining process for an online session has completed
	* @param SessionName the name of the session this callback is for
	* @param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	/**
	* calls by Delegate
	* which fired when a session create request has completed
	*
	* @param SessionName the name of the session this callback is for
	* @param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful);
	
	/**
	* calls by Delegate
	* which fired when a destroying an online session has completed
	* @param SessionName the name of the session this callback is for
	* @param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnDestroySessionCompleted(FName SessionName, bool bWasSuccessful);


	/**
	* calls by Delegate 
	* which fired when the search for an online session has completed
	* @param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnFindSessionsCompleted(bool bWasSuccessful);

	
	/** calls to start a new session  */
	void CreateSession();

	// ------- Online --------------------------------------------------------------------
	
	/** online session interface pointer */
	IOnlineSessionPtr SessionInterface;

	/**
	* Encapsulation of a search for sessions request.
	* Contains all the search parameters and any search results returned after
	* the OnFindSessionsCompleteDelegate has triggered
	* Check the SearchState for Done/Failed state before using the data
	*/
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	// --------- Widgets ----------------------------------------------------
	
	/** main menu widget class template  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UMainMenu> MenuWidgetClass;

	/** in game menu widget class template  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UInGameMenu> InGameMenuWidgetClass;

	/** current active in game menu widget  */
	UPROPERTY()
	class UInGameMenu* InGameMenu;
	
	/** current active main menu widget  */
	UPROPERTY()
	class UMainMenu* Menu;

	/** server name  */
	FString DesiredServerName;
	
};
