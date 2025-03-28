// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Frame.generated.h"

class UBowlingScoreBoard;

UCLASS()
class BOWLINGSCORINGGAME_API UFrame : public UUserWidget
{
	GENERATED_BODY()

	public:
    	UFrame(const FObjectInitializer& ObjectInitializer);
		
		
		UPROPERTY(EditAnywhere, Category = "BowlingScore")
    	class UFrame* PastFrame;

		
		UPROPERTY(EditAnywhere, Category = "BowlingScore")
    	class UFrame* NextFrame;

		int32 GetFirstAttemptScore() const { return FirstAttemptScore; }
		int32 GetSecondAttemptScore() const { return SecondAttemptScore; }
		int32 GetFrameScore() const { return FrameScore; }
		int32 GetGameTotal() const { return GameTotal; }
		bool GetStrike() const {return Strike; }
		bool GetSpare() const {return Spare; }
		bool IsEmpty() const {return IsEmptyFrame; }

		UFUNCTION()
    	void CalculateResult();

	protected:
    	virtual void NativeConstruct() override;

	private:

		UPROPERTY()
    	UBowlingScoreBoard* ScoreBoard;

    	UPROPERTY(meta = (BindWidget))
    	class UEditableTextBox* FirstAttempScoreBox;

		UPROPERTY(meta = (BindWidget))
    	class UEditableTextBox* SecondAttempScoreBox;

		UPROPERTY(meta = (BindWidget))
    	class UTextBlock* FrameResultBox;


		int FirstAttemptScore = 0;
		int SecondAttemptScore= 0;
		bool Spare;
		bool Strike;
		int FrameScore;
		int GameTotal;
		bool ShowResult;
		bool IsEmptyFrame;

		UFUNCTION()
    	void OnFirtsAttemptCommited(const FText& NewText, ETextCommit::Type CommitType);

		UFUNCTION()
    	void OnSecondAttemptCommited(const FText& NewText, ETextCommit::Type CommitType);

		UFUNCTION()
		void ProcessFirstBoxInput();

		UFUNCTION()
		void ProcessSecondBoxInput();

		UFUNCTION()
    	bool ValidateInput(FString Input, bool FirstBox);

		UFUNCTION()
    	void ResetInputBox(UEditableTextBox* InputBox);

		
};
