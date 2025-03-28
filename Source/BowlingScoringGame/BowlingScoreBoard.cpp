// Fill out your copyright notice in the Description page of Project Settings.


#include "BowlingScoreBoard.h"
#include "BaseFrame.h"

void UBowlingScoreBoard::NativeConstruct()
{
    if (BP_GenRandomButton)
    {
        BP_GenRandomButton->OnClicked.AddDynamic(this, &UBowlingScoreBoard::OnGenRandomButtonClicked);
    }
    if(BP_BaseFrame_1)
    {
        BP_BaseFrame_1->UnLockFrame();
    }
}


void UBowlingScoreBoard::CalculateScores()
{
    if(BP_BaseFrame_1)
    {
        BP_BaseFrame_1->CalculateResult();
    }
}

void UBowlingScoreBoard::OnGenRandomButtonClicked()
{
    if(BP_BaseFrame_1)
    {
        BP_BaseFrame_1->SetRandomValues();
    }
}
