// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseFrame.h"
#include "Components/EditableTextBox.h"
#include "Components/EditableText.h"
#include "BowlingScoreBoard.h"
#include "Math/UnrealMathUtility.h"
#include "Types/SlateEnums.h"




UBaseFrame::UBaseFrame(const FObjectInitializer& ObjectInitializer) 
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

void UBaseFrame::NativeConstruct()
{
    Super::NativeConstruct();

    ScoreBoard = GetTypedOuter<UBowlingScoreBoard>();

    if (!ScoreBoard)
    {
        UE_LOG(LogTemp, Warning, TEXT("UBaseFrame no pudo encontrar su ScoreBoard"));
    }

    if (FirstAttemptScoreBox && SecondAttemptScoreBox)
    {
        FirstAttemptScoreBox->OnTextCommitted.AddDynamic(this, &UBaseFrame::OnFirtsAttemptCommited);
        SecondAttemptScoreBox->OnTextCommitted.AddDynamic(this, &UBaseFrame::OnSecondAttemptCommited);
    }
    LockFrame();
}

void UBaseFrame::OnFirtsAttemptCommited(const FText& NewText, ETextCommit::Type CommitType)
{
    if (CommitType == ETextCommit::OnEnter)
        return;
    
    IsEmptyFrame = false;
    ProcessFirstBoxInput();
}

void UBaseFrame::OnSecondAttemptCommited(const FText& NewText, ETextCommit::Type CommitType)
{

    if (CommitType == ETextCommit::OnEnter)
        return;
    
    IsEmptyFrame = false;
    ProcessSecondBoxInput();
}


void UBaseFrame::ProcessFirstBoxInput()
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
        ResetInputBox(SecondAttemptScoreBox, SecondAttemptScore);
        SecondAttemptScoreBox->SetIsReadOnly(true);
        Strike = true;
        ShowResult = false;

    }
    else if(SecondAttemptScoreBox->GetIsReadOnly())
    {
        SecondAttemptScoreBox->SetIsReadOnly(false);
        Strike = false;
        ShowResult = true;
    }

    if(ValidateInput(Input) && ValidateScoreOverflow(Input, true))
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

void UBaseFrame::ProcessSecondBoxInput()
{
    FString Input = SecondAttemptScoreBox->GetText().ToString();
    if(Input == "-" || Input == "0")
    {
        Input = "0";
        SecondAttemptScoreBox->SetText(FText::FromString("-"));

    }

    if (Input == "/" || 10-FirstAttemptScore == FCString::Atoi(*Input))
    {
        Input = FString::FromInt(10-FirstAttemptScore);
        SecondAttemptScoreBox->SetText(FText::FromString("/"));
        Spare = true;
        ShowResult = false;
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


bool UBaseFrame::ValidateInput(FString Input)
{
    if(Input.IsNumeric())
    {
        int inputNumber = FCString::Atoi(*Input);
        if(inputNumber < 0 || inputNumber > 10)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Must be a number between 0 and 10"));
            return false;
        }
        else
        {
            return true;
        }
    }
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Provide a valid number or symbol"));
    return false;
}

bool UBaseFrame::ValidateScoreOverflow(FString Input, bool FirstBox)
{
    if (SecondAttemptScoreBox->GetText().ToString() == "/" && FirstBox == true)
    {
        SecondAttemptScore = 10 - FCString::Atoi(*Input);
        return true;
    }
    
    int OtherBoxValue = FirstBox == true ? SecondAttemptScore : FirstAttemptScore;
    int inputNumber = FCString::Atoi(*Input);

    if(inputNumber + OtherBoxValue > 10)
    {   
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Sum must be lower or equeal to 10"));
        return false;
    }
    return true;
}

void UBaseFrame::ResetInputBox(UEditableTextBox *InputBox, int &Value)
{
    InputBox->SetText(FText::FromString(""));
    Value = 0;
    //InputBox == FirstAttemptScoreBox ? FirstAttemptScore = 0 : SecondAttemptScore = 0;
}

void UBaseFrame::SetRandomValues()
{
    UnLockFrame();

    GameTotal = 0;
    FirstAttemptScore = 0;
    SecondAttemptScore = 0;
    Spare = false;
    Strike = false;
    ShowResult = true;
    
    FirstAttemptScore = FMath::RandRange(1, 10);
    FirstAttemptScoreBox->SetText(FText::AsNumber(FirstAttemptScore));
    ProcessFirstBoxInput();
    if(FirstAttemptScore < 10)
    {
        SecondAttemptScore = FMath::RandRange(0, 10-FirstAttemptScore-1);
        SecondAttemptScoreBox->SetText(FText::AsNumber(SecondAttemptScore));
        ProcessSecondBoxInput();
    }
    IsEmptyFrame = false;

    if(NextFrame != nullptr)
    {
        NextFrame->SetRandomValues();
    }

}

void UBaseFrame::LockFrame()
{
    GameTotal = 0;    
    ResetInputBox(FirstAttemptScoreBox, FirstAttemptScore);
    ResetInputBox(SecondAttemptScoreBox, SecondAttemptScore);
    FirstAttemptScoreBox->SetIsReadOnly(true);
    SecondAttemptScoreBox->SetIsReadOnly(true);
}

void UBaseFrame::UnLockFrame()
{
    FirstAttemptScoreBox->SetIsReadOnly(false);
    SecondAttemptScoreBox->SetIsReadOnly(false);
}

void UBaseFrame::CalculateResult()
{
    GameTotal = 0;

    if (SecondAttemptScoreBox->GetText().ToString() == "/")
    {
        SecondAttemptScore = 10 - FirstAttemptScore;
    }

    if(FirstAttemptScoreBox->GetText().ToString() == "")
    {
        FrameResultBox->SetText(FText::FromString(""));
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%lld"), SecondAttemptScore));
        return;
    }
    else
    {
        FrameScore = FirstAttemptScore + SecondAttemptScore;
        if(FrameScore == 10 && SecondAttemptScoreBox->GetText().ToString() != "/" && FirstAttemptScoreBox->GetText().ToString() != "X")
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
                    }
                    else
                    {
                        FrameScore += NextFrame->GetSecondAttemptScore();
                    }                    
                    ShowResult = NextFrame->GetGameTotal() == 0 ? false : true;
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
            NextFrame->UnLockFrame();
            NextFrame->CalculateResult();
        }

        if(ShowResult)
        {
            FrameResultBox->SetText(FText::AsNumber(GameTotal));
        }
        else
        {
            FrameResultBox->SetText(FText::FromString(""));
        }
    }

}
