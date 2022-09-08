// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mole.h"
#include "GameController.generated.h"


UCLASS()
class AGameController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGameController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<AMole> Mole = nullptr;
	
	void UpdateScore();
	
	int32 MoleScore = 0;
	
	int32 PlayerScore = 0;

protected:
	const FVector GetRandomHoleLocation() const;

	UPROPERTY(EditAnywhere)
		TArray<TObjectPtr<AActor>> Holes;

	UPROPERTY(EditAnywhere)
		int32 HoleRange = 700;

};
