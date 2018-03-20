// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

bool UInGameMenu::Initialize()
{
	bool Result = Super::Initialize();
	if (Result == false)
	{
		return false;
	}

	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::TearDown);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::OnQuitButtonClicked);
	}

	return true;
}

void UInGameMenu::OnQuitButtonClicked()
{
	TearDown();
	if (MenuInterface)
	{
		MenuInterface->LoadMainMenu();
	}
}
