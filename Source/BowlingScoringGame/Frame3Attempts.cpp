// Fill out your copyright notice in the Description page of Project Settings.


#include "Frame3Attempts.h"
#include "Components/EditableTextBox.h"
#include "Components/EditableText.h"
#include "BowlingScoreBoard.h"
#include "Types/SlateEnums.h"

void UFrame3Attempts::NativeConstruct()
{
    Super::NativeConstruct();

    ThirdAttemptScore = 0;
    if (ThirdAttemptScoreBox)
    {
        ThirdAttemptScoreBox->OnTextCommitted.AddDynamic(this, &UFrame3Attempts::OnThirdAttemptCommited);
        
        ThirdAttemptScoreBox->SetIsReadOnly(true);
    }
}

void UFrame3Attempts::OnThirdAttemptCommited(const FText& NewText, ETextCommit::Type CommitType)
{
    if (CommitType == ETextCommit::OnEnter)
        return;
    
    ProcessThirdBoxInput();
}

void UFrame3Attempts::ProcessThirdBoxInput()
{
    FString Input = ThirdAttemptScoreBox->GetText().ToString();
    if(Input == "-" || Input == "0")
    {
        Input = "0";
        ThirdAttemptScoreBox->SetText(FText::FromString("-"));

    }
    
    if(FirstAttemptScore == 10)
    {
        if(SecondAttemptScore == 10)
        {
            if (Input == "X" || Input == "x" || Input == "10")
            {
                Input = "10";
                ThirdAttemptScoreBox->SetText(FText::FromString("X"));
                ThirdAttemptScore = FCString::Atoi(*Input);
                ScoreBoard->CalculateScores();
                ShowResult = true;
                return;
            }
            else if(ValidateInput(Input))
            {
                ThirdAttemptScore = FCString::Atoi(*Input);
                ShowResult = true;
                ScoreBoard->CalculateScores();
                return;
            }
            else
            {
                ThirdAttemptScoreBox->SetText(FText::AsNumber(ThirdAttemptScore));
                ProcessThirdBoxInput();
            }
        }
        else if (Input == "/" || 10-SecondAttemptScore == FCString::Atoi(*Input))
        {
            Input = FString::FromInt(10-SecondAttemptScore);
            ThirdAttemptScoreBox->SetText(FText::FromString("/"));
            ShowResult = true;
        }
        
        if(ValidateInput(Input) && FCString::Atoi(*Input) + SecondAttemptScore <= 10)
        {
            ThirdAttemptScore = FCString::Atoi(*Input);
            ShowResult = true;
            ScoreBoard->CalculateScores();
        }
        else
        {
            ThirdAttemptScoreBox->SetText(FText::AsNumber(ThirdAttemptScore));
            ProcessThirdBoxInput();
        }
    }
    else if(SecondAttemptScoreBox->GetText().ToString() == "/")
    {
        if(ValidateInput(Input))
        {
            ThirdAttemptScore = FCString::Atoi(*Input);
            ShowResult = true;
            ScoreBoard->CalculateScores();
        }
        else
        {
            ThirdAttemptScoreBox->SetText(FText::AsNumber(ThirdAttemptScore));
            ProcessThirdBoxInput();
        }
    }
}

void UFrame3Attempts::ProcessFirstBoxInput()
{
    FString Input = FirstAttemptScoreBox->GetText().ToString();
    if(Input == "-" || Input == "0")
    {
        Input = "0";
        FirstAttemptScoreBox->SetText(FText::FromString("-"));

    }
    
    if (Input == "X" || Input == "x" || Input == "10")
    {
        Input = "10";
        FirstAttemptScoreBox->SetText(FText::FromString("X"));
        FirstAttemptScore = FCString::Atoi(*Input);
        Strike = true;
        ShowResult = false;
        ScoreBoard->CalculateScores();
        return;
    }

    ResetInputBox(SecondAttemptScoreBox, SecondAttemptScore);
    ResetInputBox(ThirdAttemptScoreBox, ThirdAttemptScore);
    if(ValidateInput(Input))
    {
        FirstAttemptScore = FCString::Atoi(*Input);
        ScoreBoard->CalculateScores();
    }
    else
    {
        FirstAttemptScoreBox->SetText(FText::AsNumber(FirstAttemptScore));
        ProcessFirstBoxInput();
    }
}

void UFrame3Attempts::ProcessSecondBoxInput()
{
    FString Input = SecondAttemptScoreBox->GetText().ToString();
    if(Input == "-" || Input == "0")
    {
        Input = "0";
        SecondAttemptScoreBox->SetText(FText::FromString("-"));

    }

    if(FirstAttemptScore == 10)
    {
        ResetInputBox(ThirdAttemptScoreBox, ThirdAttemptScore);
        if (Input == "X" || Input == "x" || Input == "10")
        {
            Input = "10";
            SecondAttemptScoreBox->SetText(FText::FromString("X"));
            SecondAttemptScore = FCString::Atoi(*Input);
            ScoreBoard->CalculateScores();
            return;
        }
        else if(ValidateInput(Input))
        {
            SecondAttemptScore = FCString::Atoi(*Input);
            ScoreBoard->CalculateScores();
            return;
        }
    
    }else if (Input == "/" || 10-FirstAttemptScore == FCString::Atoi(*Input))
    {
        Input = FString::FromInt(10-FirstAttemptScore);
        SecondAttemptScoreBox->SetText(FText::FromString("/"));
        Spare = true;
    }
    else
    {
        Spare = false;
        ShowResult = true;
    }
    
    if(ValidateInput(Input) && ValidateScoreOverflow(Input, false))
    {
        SecondAttemptScore = FCString::Atoi(*Input);
        ScoreBoard->CalculateScores();
    }
    else
    {
        SecondAttemptScoreBox->SetText(FText::AsNumber(SecondAttemptScore));
        ProcessSecondBoxInput();
    }
}

void UFrame3Attempts::CalculateResult()
{
    if(FirstAttemptScore == 10 || FirstAttemptScore + SecondAttemptScore == 10)
    {
        EnableThirdBox = true;
    }
    else
    {
        EnableThirdBox = false;
        ResetInputBox(ThirdAttemptScoreBox, ThirdAttemptScore);
    }
    ThirdAttemptScoreBox->SetIsReadOnly(!EnableThirdBox);


    if (SecondAttemptScoreBox->GetText().ToString() == "/")
    {
        if(FirstAttemptScore == 10)
        {
            SecondAttemptScoreBox->SetText(FText::AsNumber(SecondAttemptScore));
            ResetInputBox(ThirdAttemptScoreBox, ThirdAttemptScore);
        }
        else
        {
            SecondAttemptScore = 10 - FirstAttemptScore;
        }
    }

    FrameScore = FirstAttemptScore + SecondAttemptScore + ThirdAttemptScore;

    GameTotal = 0;
    if(PastFrame != nullptr)
    {
        GameTotal = PastFrame->GetGameTotal();
    }

    GameTotal += FrameScore;

    if(ShowResult || ThirdAttemptScoreBox->GetText().ToString() != "")
    {
        FrameResultBox->SetText(FText::AsNumber(GameTotal));
    }
    else
    {
        FrameResultBox->SetText(FText::FromString(""));
    }
}

void UFrame3Attempts::SetRandomValues()
{
    
    GameTotal = 0;
    FirstAttemptScore = 0;
    SecondAttemptScore = 0;
    ThirdAttemptScore = 0;
    Spare = false;
    Strike = false;
    ShowResult = true;
    
    FirstAttemptScore = FMath::RandRange(1, 10);
    FirstAttemptScoreBox->SetText(FText::AsNumber(FirstAttemptScore));
    ProcessFirstBoxInput();
    if(FirstAttemptScore == 10)
    {
        SecondAttemptScore = FMath::RandRange(1, 10);
        SecondAttemptScoreBox->SetText(FText::AsNumber(SecondAttemptScore));
        ProcessSecondBoxInput();
        if(SecondAttemptScore == 10)
        {
            ThirdAttemptScore = FMath::RandRange(0, 10);
            ThirdAttemptScoreBox->SetText(FText::AsNumber(ThirdAttemptScore));
            ProcessThirdBoxInput();
        }else
        {
            ThirdAttemptScore = FMath::RandRange(0, 10-SecondAttemptScore-1);
            ThirdAttemptScoreBox->SetText(FText::AsNumber(ThirdAttemptScore));
            ProcessThirdBoxInput();
        }
    }
    else if(FirstAttemptScore < 10)
    {
        SecondAttemptScore = FMath::RandRange(0, 10-FirstAttemptScore-1);
        SecondAttemptScoreBox->SetText(FText::AsNumber(SecondAttemptScore));
        ProcessSecondBoxInput();

        if(FirstAttemptScore + SecondAttemptScore == 10)
        {
            ThirdAttemptScore = FMath::RandRange(0, 10-SecondAttemptScore-1);
            ThirdAttemptScoreBox->SetText(FText::AsNumber(ThirdAttemptScore));
            ProcessThirdBoxInput();
        }
    }

}