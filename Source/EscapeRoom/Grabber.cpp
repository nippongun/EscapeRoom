// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "Grabber.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...

	UE_LOG(LogTemp, Warning, TEXT("Grabber"));

	// Check for the Physcis Handle Component
	PhysiscsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!PhysiscsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle Component for %s found! Is the component attached?"), *GetOwner()->GetName());
	}

	if (!InputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("No Input Component for %s found! Is the component attached?"), *GetOwner()->GetName());
	}
	else
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	// get players viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	// UE_LOG(LogTemp, Warning, TEXT("Location:%s, Rotation:%s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());
	// ray-cast out to a certain distance

	// Draw a line from player showung the reach.

	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * reach);

	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(0, 255, 0),
		false,
		0.f,
		0,
		5);

	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	// ray cast out to a certain distance
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams);

	// See what it hits
	AActor *ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Error, TEXT("Line trace has hit: %s"), *(ActorHit->GetName()));
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber pressed"));
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber released"));
}