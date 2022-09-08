// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WhackingTheMoleCharacter.h"
#include "Mole.generated.h"

UENUM()
enum class EMoleState : int8
{
	Idle,
	TranslateUp,
	Exposed,
	TranslateUnderground,
	Hidden,
	MoleIsHit
};

UCLASS()
class AMole : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMole();
	bool IsMoleCycleFinished() const;
	void TransitionToHole(const FVector& HoleLocation);
	bool WasMoleHit();
	bool DidMoleDodge();
	bool DidMoleGetHit = false;
	bool DidTheMoleDodge = false;

protected:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SwitchState(EMoleState NewState);

	// How long it takes for Mole to transition from underground to above the whole
	UPROPERTY(EditAnywhere, Category = "Mole Settings")
		float TimeToTranslateUp = 4.f;

	// How long should stay out
	UPROPERTY(EditAnywhere, Category = "Mole Settings")
		float TimeToStayExposed = 4.f;

	// How long it takes to go down under ground (transition from above the ground to underground)AboveGroundOffset
	UPROPERTY(EditAnywhere, Category = "Mole Settings")
		float TimeToGoUnderground = 4.f;

	// How long it should be under the ground before next Peek cycle should start
	UPROPERTY(EditAnywhere, Category = "Mole Settings")
		float TimeToStayHidden = 4.f;

	//Value of Z when mole is exposed
	UPROPERTY(EditAnywhere, Category = "Mole Settings")
		float ExposedOffset = 50.f;

	//Value of Z when mole is hidden
	UPROPERTY(EditAnywhere, Category = "Mole Settings")
		float HiddenOffset = 100.f;

	// What state am I in?
	EMoleState CurrentState = EMoleState::Idle;

	// For how long am I in a current state?
	float CurrentStateTime = 0.f;

	// location of current hole to peek out from
	FVector CurrentHoleLocation = FVector::ZeroVector;

	float TranslateSpeed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"));
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MoleMesh;

	//Callback function 
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitCompoenent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//Called when the game starts or when spawned 
	virtual void BeginPlay() override;

	uint32 bGenerateOverlapEvents : 1;
};
