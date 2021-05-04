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
	LogPressurePlateError();
	FindAudioComponent();
	OpenerActor = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::LogPressurePlateError() {
	if (!PressurePlate) {
		UE_LOG
		(
			LogTemp, 
			Error, 
			TEXT("Actor: %s has the OpenDoor Component but has not set the PressurePlate!"), 
			*GetOwner()->GetName()
		);
	}
	return;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetTotalMassOfActors() >= RequiredMass){
		if (!IsOpenning) {
			IsOpenning = true;
			DoorSound->Play();
		}
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}else if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay){
		if (IsOpenning) {
			IsOpenning = false;
			DoorSound->Play();
		}
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

float UOpenDoor::GetTotalMassOfActors() const {
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OverlappingActors);
	for (AActor* Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

void UOpenDoor::FindAudioComponent() {
	DoorSound = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!DoorSound) {
		UE_LOG(LogTemp, Error, TEXT("No Audio component found on %s."), *(GetOwner()->GetName()))
	}
}