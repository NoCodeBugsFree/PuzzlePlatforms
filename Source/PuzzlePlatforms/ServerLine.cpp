// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerLine.h"
#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

bool UServerLine::Initialize()
{
	bool Result = Super::Initialize();
	if (Result == false)
	{
		return Result;
	}

	if (ServerLineButton)
	{
		ServerLineButton->OnClicked.AddDynamic(this, &UServerLine::OnServerLineButtonClicked);
	}

	return true;
}

void UServerLine::Setup(UMainMenu* Parent, uint32 Index)
{
	if (Parent)
	{
		MainMenu = Parent;
	}

	CurrentIndex = Index;
}

void UServerLine::OnServerLineButtonClicked()
{
	if (MainMenu)
	{
		MainMenu->SelectIndex(CurrentIndex);
	}
}
