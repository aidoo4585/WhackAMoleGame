// Fill out your copyright notice in the Description page of Project Settings.

#include "Mole.h"
#include "WhackingTheMoleCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameController.h"


#pragma optimize("", off)

// Sets default values
AMole::AMole()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComponent;

	MoleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mole Mesh"));
	//For moving the Mole Mesh with the Capsule Component 
	MoleMesh->SetupAttachment(CapsuleComponent);

}

//Called when the game starts 
void AMole::BeginPlay()
{
	Super::BeginPlay();
	MoleMesh->OnComponentHit.AddDynamic(this, &AMole::OnHit);
}


// Called every frame 
void AMole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentStateTime += DeltaTime;

	//State is idle right now 
	switch (CurrentState)
	{
		//State Switch
	case EMoleState::TranslateUp:
		if (CurrentStateTime >= TimeToTranslateUp)
		{
			SwitchState(EMoleState::Exposed);
		}
		else
		{
			const FVector MoleVelocity = FVector(0.f, 0.f, TranslateSpeed);
			// Translating Mole up	
			FVector MoleLocation = CurrentHoleLocation - FVector(0.f, 0.f, HiddenOffset);
			MoleLocation = (MoleLocation + (MoleVelocity * CurrentStateTime));
			SetActorLocation(MoleLocation);
		}
		break;

	case EMoleState::Exposed:
		if (CurrentStateTime >= TimeToStayExposed)
		{
			SwitchState(EMoleState::TranslateUnderground);
		}
		break;

	case EMoleState::TranslateUnderground:
		if (CurrentStateTime >= TimeToGoUnderground)
		{
			SwitchState(EMoleState::Hidden);
		}
		else
		{
			const FVector MoleVelocity = FVector(0.f, 0.f, TranslateSpeed);
			//Translating Mole down
			FVector MoleLocation = CurrentHoleLocation + FVector(0.f, 0.f, ExposedOffset);
			MoleLocation = (MoleLocation - (MoleVelocity * CurrentStateTime));
			SetActorLocation(MoleLocation);
		}
		break;

	case EMoleState::Hidden:
		if (CurrentStateTime >= TimeToStayHidden)
		{
			SwitchState(EMoleState::Idle);
			if (DidMoleGetHit != true)
			{
				DidTheMoleDodge = true;
			}
		}
		break;

	case EMoleState::MoleIsHit:
		SwitchState(EMoleState::Hidden);
		break;
	}
}

bool AMole::IsMoleCycleFinished() const
{
	if (CurrentState == EMoleState::Idle)
	{
		return true;
	}

	return false;
}

void AMole::TransitionToHole(const FVector& HoleLocation)
{
	CurrentHoleLocation = HoleLocation;
	SwitchState(EMoleState::TranslateUp);
	DidMoleGetHit = false;
}




void AMole::OnHit(UPrimitiveComponent* HitCompoenent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (OtherActor != nullptr)
	{
		AWhackingTheMoleCharacter* PlayerCharacter = Cast<AWhackingTheMoleCharacter>(OtherActor);

		if (PlayerCharacter != nullptr)
		{
			//Switch the current state of the Mole to go underground 
			SwitchState(EMoleState::MoleIsHit);
		}
	}
}

void AMole::SwitchState(EMoleState NewState)
{
	CurrentState = NewState;
	CurrentStateTime = 0.f;

	switch (CurrentState)
	{
	case EMoleState::TranslateUp:
		TranslateSpeed = (HiddenOffset + ExposedOffset) / TimeToStayExposed;

		break;

	case EMoleState::Exposed:
		break;

	case EMoleState::TranslateUnderground:
		TranslateSpeed = (HiddenOffset + ExposedOffset) / TimeToGoUnderground;

		break;

	case EMoleState::Hidden:
		SetActorLocation(CurrentHoleLocation - FVector(0.f, 0.f, HiddenOffset));
		break;

	case EMoleState::MoleIsHit:
		DidMoleGetHit = true;
		break;
	}

}

bool AMole::WasMoleHit()
{
	if (DidMoleGetHit == true)
	{
		return true;
	}

	return false;
}

bool AMole::DidMoleDodge()
{
	if (DidTheMoleDodge == true)
	{
		return true;
	}
	
	return false;
}



