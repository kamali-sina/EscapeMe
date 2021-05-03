// Copyright PapaSinku 2021 all rights reserved.


#include "OpenDoor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	// ...
	StartingYaw = GetOwner()->GetActorRotation().Yaw; 
	if (!PressurePlate){
		UE_LOG(LogTemp, Error, TEXT("Actor: %s has the OpenDoor Component but has not set the PressurePlate!"), *GetOwner()->GetName());
	}
	OpenerActor = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (PressurePlate && PressurePlate->IsOverlappingActor(OpenerActor)){
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}else if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay){
		CloseDoor(DeltaTime);
	}
}


void UOpenDoor::OpenDoor(float DeltaTime){
	FRotator ActorRotation = GetOwner()->GetActorRotation(); 
	ActorRotation.Yaw = FMath::FInterpTo(ActorRotation.Yaw, StartingYaw + TargetRotation, DeltaTime, OpeningSpeed);
	GetOwner()->SetActorRotation(ActorRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime){
	FRotator ActorRotation = GetOwner()->GetActorRotation(); 
	ActorRotation.Yaw = FMath::FInterpTo(ActorRotation.Yaw, StartingYaw, DeltaTime, ClosingSpeed);
	GetOwner()->SetActorRotation(ActorRotation);
}