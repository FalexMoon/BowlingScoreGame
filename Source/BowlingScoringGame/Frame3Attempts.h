// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseFrame.h"
#include "Frame3Attempts.generated.h"


class UBowlingScoreBoard;

UCLASS()
class BOWLINGSCORINGGAME_API UFrame3Attempts : public UBaseFrame
{
	GENERATED_BODY()

	public:
		virtual void CalculateResult() override;
		virtual void SetRandomValues() override;

	protected:
    	virtual void NativeConstruct() override;
		virtual void ProcessFirstBoxInput() override;
		virtual void ProcessSecondBoxInput() override;

	private:

    	UPROPERTY(meta = (BindWidget))
    	class UEditableTextBox* ThirdAttemptScoreBox;
		
		int ThirdAttemptScore= 0;

		UFUNCTION()
    	void OnThirdAttemptCommited(const FText& NewText, ETextCommit::Type CommitType);

		UFUNCTION()
		void ProcessThirdBoxInput();

		bool EnableThirdBox;

};
