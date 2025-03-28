// Fill out your copyright notice in the Description page of Project Settings.


#include "Frame.h"
#include "Components/EditableTextBox.h"
#include "Components/EditableText.h"
#include "BowlingScoreBoard.h"
#include "Types/SlateEnums.h"




UFrame::UFrame(const FObjectInitializer& ObjectInitializer) 
: Super(ObjectInitializer)
{
    FirstAttemptScore = 0;
    SecondAttemptScore= 0;
    Spare = false;
	Strike = false;
	FrameScore = 0;
    GameTotal = 0;
    ShowResult = false;
    IsEmptyFrame = true;
}

void UFrame::NativeConstruct()
{
    Super::NativeConstruct();

    ScoreBoard = GetTypedOuter<UBowlingScoreBoard>();

    if (!ScoreBoard)
    {
        UE_LOG(LogTemp, Warning, TEXT("UFrame no pudo encontrar su ScoreBoard"));
    }

    if (FirstAttempScoreBox && SecondAttempScoreBox)
    {
        FirstAttempScoreBox->OnTextCommitted.AddDynamic(this, &UFrame::OnFirtsAttemptCommited);
        SecondAttempScoreBox->OnTextCommitted.AddDynamic(this, &UFrame::OnSecondAttemptCommited);
    }
}

void UFrame::OnFirtsAttemptCommited(const FText& NewText, ETextCommit::Type CommitType)
{
    //OnTextCommited(NewText, CommitType, FirstAttemptScore);
    if (CommitType == ETextCommit::OnEnter)
        return;
    
    IsEmptyFrame = false;
    ProcessFirstBoxInput();
}

void UFrame::OnSecondAttemptCommited(const FText& NewText, ETextCommit::Type CommitType)
{
    //OnTextCommited(NewText, CommitType, SecondAttemptScore);

    if (CommitType == ETextCommit::OnEnter)
        return;
    
    IsEmptyFrame = false;
    ProcessSecondBoxInput();
}


void UFrame::ProcessFirstBoxInput()
{
    FString Input = FirstAttempScoreBox->GetText().ToString();
    if(Input == "-" || Input == "0")
    {
        Input = "0";
        FirstAttempScoreBox->SetText(FText::FromString("-"));

    }
    
    if (Input == "X" || Input == "x" || Input == "10")
    {
        Input = "10";
        FirstAttempScoreBox->SetText(FText::FromString("X"));
        ResetInputBox(SecondAttempScoreBox);
        SecondAttempScoreBox->SetIsReadOnly(true);
        Strike = true;
        ShowResult = false;

    }
    else if(SecondAttempScoreBox->GetIsReadOnly())
    {
        SecondAttempScoreBox->SetIsReadOnly(false);
        Strike = false;
        ShowResult = true;
    }

    if(ValidateInput(Input, true))
    {
        FirstAttemptScore = FCString::Atoi(*Input);
        ScoreBoard->CalculateScores();
    }
    else
    {
        FirstAttempScoreBox->SetText(FText::AsNumber(FirstAttemptScore));
        ProcessFirstBoxInput();
    }
}

void UFrame::ProcessSecondBoxInput()
{
    FString Input = SecondAttempScoreBox->GetText().ToString();
    if(Input == "-" || Input == "0")
    {
        Input = "0";
        SecondAttempScoreBox->SetText(FText::FromString("-"));

    }

    if (Input == "/" || 10-FirstAttemptScore == FCString::Atoi(*Input))
    {
        Input = FString::FromInt(10-FirstAttemptScore);
        SecondAttempScoreBox->SetText(FText::FromString("/"));
        Spare = true;
        ShowResult = false;
    }
    else
    {
        Spare = false;
        ShowResult = true;
    }

    if(ValidateInput(Input, false))
    {
        SecondAttemptScore = FCString::Atoi(*Input);
        ScoreBoard->CalculateScores();
    }
    else
    {
        SecondAttempScoreBox->SetText(FText::AsNumber(SecondAttemptScore));
        ProcessSecondBoxInput();
    }
}


bool UFrame::ValidateInput(FString Input, bool FirstBox)
{
    if(Input.IsNumeric())
    {
        int otherBoxValue = FirstBox == true ? SecondAttemptScore : FirstAttemptScore;
        if(FCString::Atoi(*Input) + otherBoxValue > 10)
        {   
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Sum must be lower or equeal to 10"));
            return false;
        }

        if(FCString::Atoi(*Input) < 0)
        {   
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Must be a positive number"));
            return false;
        }
        return true;
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Please Enter a valid number or symbol"));
        return false;
    }
}

void UFrame::ResetInputBox(UEditableTextBox *InputBox)
{
    InputBox->SetText(FText::FromString(""));
    InputBox == FirstAttempScoreBox ? FirstAttemptScore = 0 : SecondAttemptScore = 0;
}

void UFrame::CalculateResult()
{
    GameTotal = 0;

    if (SecondAttempScoreBox->GetText().ToString() == "/")
    {
        SecondAttemptScore = 10 - FirstAttemptScore;
    }

    if(FirstAttempScoreBox->GetText().ToString() == "")
    {
        FrameResultBox->SetText(FText::FromString(""));
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%lld"), SecondAttemptScore));
        return;
    }
    else
    {
        FrameScore = FirstAttemptScore + SecondAttemptScore;
        if(FrameScore == 10 && SecondAttempScoreBox->GetText().ToString() != "/" && FirstAttempScoreBox->GetText().ToString() != "X")
        {
            ProcessSecondBoxInput();
        }
        
        if(NextFrame != nullptr)
        {
            if(Spare)
            {
                FrameScore += NextFrame->GetFirstAttemptScore();
                ShowResult = !NextFrame->IsEmpty();
            }
            else if (Strike)
            {
                if(NextFrame->GetStrike())
                {
                    FrameScore += 10;
                    if(NextFrame->NextFrame != nullptr)
                    {
                        FrameScore += NextFrame->NextFrame->GetFirstAttemptScore();
                        ShowResult = NextFrame->GetGameTotal() == 0 ? false : true;
                    }
                }
                else
                {
                    FrameScore += NextFrame->GetFirstAttemptScore() + NextFrame->GetSecondAttemptScore();
                    ShowResult = NextFrame->GetGameTotal() == 0 ? false : true;
                }
                
            }
        }
        if(PastFrame != nullptr)
        {
            GameTotal = PastFrame->GetGameTotal();
        }

        
        GameTotal += FrameScore;

        if(NextFrame != nullptr)
        {
            NextFrame->CalculateResult();
        }

        if(ShowResult)
        {
            FrameResultBox->SetText(FText::AsNumber(GameTotal));
        }
    }

}
