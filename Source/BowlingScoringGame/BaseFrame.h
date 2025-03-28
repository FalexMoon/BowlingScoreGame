// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "BaseFrame.generated.h"

class UBowlingScoreBoard;

UCLASS()
class BOWLINGSCORINGGAME_API UBaseFrame : public UUserWidget
{
	GENERATED_BODY()
	
	public:
		UBaseFrame(const FObjectInitializer& ObjectInitializer);
		
		
		UPROPERTY(EditAnywhere, Category = "BowlingScore")
    	class UBaseFrame* PastFrame;

		
		UPROPERTY(EditAnywhere, Category = "BowlingScore")
    	class UBaseFrame* NextFrame;

		int32 GetFirstAttemptScore() const { return FirstAttemptScore; }
		int32 GetSecondAttemptScore() const { return SecondAttemptScore; }
		int32 GetFrameScore() const { return FrameScore; }
		int32 GetGameTotal() const { return GameTotal; }
		bool GetStrike() const {return Strike; }
		bool GetSpare() const {return Spare; }
		bool IsEmpty() const {return IsEmptyFrame; }

		UFUNCTION()
    	virtual void CalculateResult();

		UFUNCTION()
		virtual void SetRandomValues();

		UFUNCTION()
		virtual void LockFrame();
		
		UFUNCTION()
		virtual void UnLockFrame();

	protected:
    	virtual void NativeConstruct() override;
		
		UPROPERTY()
    	UBowlingScoreBoard* ScoreBoard;

		UFUNCTION()
		virtual void ProcessFirstBoxInput();

		UFUNCTION()
		virtual void ProcessSecondBoxInput();

		UFUNCTION()
    	virtual bool ValidateInput(FString Input);

		UFUNCTION()
		virtual bool ValidateScoreOverflow(FString Input, bool FirstBox);
		
		int FirstAttemptScore = 0;
		int SecondAttemptScore= 0;
		int FrameScore;
		int GameTotal;
		bool ShowResult;
		bool IsEmptyFrame;
		bool Spare;
		bool Strike;
		
    	UPROPERTY(meta = (BindWidget))
    	class UEditableTextBox* FirstAttemptScoreBox;

		UPROPERTY(meta = (BindWidget))
    	class UEditableTextBox* SecondAttemptScoreBox;

		UPROPERTY(meta = (BindWidget))
    	class UTextBlock* FrameResultBox;

		UFUNCTION()
    	void ResetInputBox(UEditableTextBox* InputBox, int &Value);

		

	private:


		UFUNCTION()
    	void OnFirtsAttemptCommited(const FText& NewText, ETextCommit::Type CommitType);

		UFUNCTION()
    	void OnSecondAttemptCommited(const FText& NewText, ETextCommit::Type CommitType);


};
