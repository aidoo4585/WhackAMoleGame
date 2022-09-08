// Fill out your copyright notice in the Description page of Project Settings.

#include "GameController.h"
#include "Mole.h"

#pragma optimize("", off)

// Sets default values
AGameController::AGameController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void AGameController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If Mole character exists 
	if (Mole != nullptr)
	{
		//Is the Mole done its sequence of movement 
		if (Mole->IsMoleCycleFinished())
		{
			UpdateScore();
			const FVector HoleLocation = GetRandomHoleLocation();
			if (HoleLocation != FVector::ZeroVector)
			{
				Mole->TransitionToHole(HoleLocation);
			}
		}
	}
}

//Creates hole locations and stores them in a vector 
const FVector AGameController::GetRandomHoleLocation() const
{
	//Number of holes is the number of pointers to the Hole asset, created by user drag and drop 	
	const int32 NumberOfHoles = Holes.Num();

	

	if (NumberOfHoles > 0)
	{
		//Determining the location of the player actor 
		UWorld* World = GetWorld();
		check(World);
		const FVector PlayerLocation = World->GetFirstPlayerController()->GetPawn()->GetActorLocation();
		TArray <FVector> OptimalHoleLocations;
		
		for (int32 i = 0; i < NumberOfHoles; i++)
		{
			//Store distance between the Player and Holes 
			FVector HoleDistance = (PlayerLocation - (Holes[i]->GetActorLocation()));
			
			//Calculating the magnitude of the distance between Hole and Player
			double Distance = FMath::Sqrt((FMath::Pow(HoleDistance.X, 2)) +
				(FMath::Pow(HoleDistance.Y, 2)) + (FMath::Pow(HoleDistance.Z, 2)));

			//Store the holes whose distances meet the criteria 
			if (Distance >= HoleRange)
			{
				OptimalHoleLocations.Add(Holes[i]->GetActorLocation());
			}
		}
		//Determine hole index from selection of holes to randomly choose from
		const int32 IdealHoleIndex = FMath::RandRange(0, OptimalHoleLocations.Num() - 1);
		if (OptimalHoleLocations.IsValidIndex(IdealHoleIndex))
		{
			return OptimalHoleLocations[IdealHoleIndex];
		}
	}
	return FVector::ZeroVector;
}

void AGameController::UpdateScore()
{
	//The case where the IsMoleHit status is true, after the cycle is done come check here then evaluate score
	if (Mole->WasMoleHit() == true)
	{
		PlayerScore += 1;
		UE_LOG(LogTemp, Warning, TEXT("The Mole was wacked, Player Score, %d"), PlayerScore);
		Mole->DidMoleGetHit = false; //To optimize the code make a reset function for mole get hit and dodge 
	}

	//Mole was not hit but it got away so after the cycle is done, checks to see that Mole was not hit 
	else if (Mole->DidMoleDodge() == true)
	{
		MoleScore += 1;
		UE_LOG(LogTemp, Warning, TEXT("Oh no! The Mole escaped, Mole Score %d"), MoleScore);
		Mole->DidTheMoleDodge = false;
	}
}


