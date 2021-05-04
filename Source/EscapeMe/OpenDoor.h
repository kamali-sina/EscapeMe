// Copyright PapaSinku 2021 all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/TriggerVolume.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEME_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	void FindAudioComponent();
	void LogPressurePlateError();
	float GetTotalMassOfActors() const;
	float StartingYaw;
	bool IsOpenning=false;

	UPROPERTY(EditAnywhere)
	float TargetRotation = 90.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate=nullptr;
	AActor* OpenerActor= nullptr;

	float DoorLastOpened = 0.f;
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay=2.f;
	UPROPERTY(EditAnywhere)
	float ClosingSpeed=2.f;
	UPROPERTY(EditAnywhere)
	float OpeningSpeed=1.7f;
	UPROPERTY(EditAnywhere)
	float RequiredMass = 70.f;
	UPROPERTY()
	UAudioComponent* DoorSound = nullptr;
};
