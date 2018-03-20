// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "PuzzlePlatformsGI.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "ServerLine.h"
#include "ConstructorHelpers.h"
#include "Components/TextBlock.h"

UMainMenu::UMainMenu()
{
	/**  set the widget asset */
	static ConstructorHelpers::FClassFinder<UUserWidget> ServerLineWidgetClassBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/UI/WBP_ServerLine"));
	if (ServerLineWidgetClassBPClass.Class != NULL)
	{
		ServerLineWidgetClass = ServerLineWidgetClassBPClass.Class;
	}
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (Success == false)
	{
		return Success;
	}

	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);
	}

	if (JoinButtonMM)
	{
		JoinButtonMM->OnClicked.AddDynamic(this, &UMainMenu::OnJoinButtonMMClicked);
	}

	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UMainMenu::OnCancelButtonClicked);
	}

	if (JoinButtonJM)
	{
		JoinButtonJM->OnClicked.AddDynamic(this, &UMainMenu::OnJoinButtonJMClicked);
	}

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UMainMenu::OnExitButtonClicked);
	}

	if (CancelHostMenuButton)
	{
		CancelHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OnCancelButtonClicked);
	}

	if (ConfirmHostMenuButton)
	{
		ConfirmHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OnHostButtonClicked);
	}

	return true;
}

void UMainMenu::SetServerList(TArray<FServerData>& ServerNames)
{
	if (ServerList)
	{
		ServerList->ClearChildren();

		uint32 ServerLineIndex = 0;

		for (const FServerData& ServerData : ServerNames)
		{
			if (ServerLineWidgetClass)
			{
				UServerLine* ServerLine = CreateWidget<UServerLine>(GetWorld(), ServerLineWidgetClass);
				if (ServerLine)
				{
					/** fill the server Name data */
					ServerLine->ServerName->SetText(FText::FromString(ServerData.Name));
					
					/** fill the HostUser name */
					ServerLine->HostUser->SetText(FText::FromString(ServerData.HostUsername));
					
					/** fill the Players data */
					ServerLine->ConnectionFraction->SetText(FText::FromString(FString::Printf(TEXT("%d / %d "), ServerData.CurrentPlayers, ServerData.MaxPlayers)));
					
					ServerLine->Setup(this, ServerLineIndex++);
					ServerList->AddChild(ServerLine);
				}
			}
		}
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UMainMenu::UpdateChildren()
{
	if (ServerList)
	{	
		for (int Index = 0; Index < WS->GetChildrenCount(); Index++)
		{
			if (UServerLine* ServerLine = Cast<UServerLine>(ServerList->GetChildAt(Index)))
			{
				ServerLine->bSelected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == Index);
			}
		}
	}
}

/** call the interface function  */
void UMainMenu::OnHostButtonClicked()
{
	if (MenuInterface && ServerHostName)
	{
		FString ServerName = ServerHostName->Text.ToString();
		MenuInterface->Host(ServerName);
	}
	
	// WORKS JUST FINE!
	/*if (UGameInstance* GI = GetWorld()->GetGameInstance())
	{
		if (IMenuInterface* MI = Cast<IMenuInterface>(GI))
		{
			MI->Host();
		}
	}*/
	
	//// WORKS JUST FINE!
	//if(UPuzzlePlatformsGI* PuzzlePlatformsGI = Cast<UPuzzlePlatformsGI>(GetWorld()->GetGameInstance()))
	//{
	//	PuzzlePlatformsGI->Host();
	//}
}

void UMainMenu::OpenHostMenu()
{
	if (WS && HostMenu)
	{
		WS->SetActiveWidget(HostMenu);
	}
}

void UMainMenu::OnJoinButtonMMClicked()
{
	if (WS && JoinMenu)
	{
		WS->SetActiveWidget(JoinMenu);

		/** refresh the server list  */
		if (MenuInterface)
		{
			MenuInterface->RefreshServerList();
		}
	}
}

void UMainMenu::OnJoinButtonJMClicked() 
{
	if (SelectedIndex.IsSet() && MenuInterface)
	{
		MenuInterface->Join(SelectedIndex.GetValue());
	}

	/*if (IP_Text)
	{
		FString IP_AddressText = IP_Text->GetText().ToString();
		PuzzlePlatformsGI->Join(IP_AddressText);
	}*/
}

void UMainMenu::OnCancelButtonClicked()
{
	if (WS && MainMenu)
	{
		WS->SetActiveWidget(MainMenu);
		//WS->SetActiveWidgetIndex(0);
	}
}

void UMainMenu::OnExitButtonClicked()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("Quit");
}