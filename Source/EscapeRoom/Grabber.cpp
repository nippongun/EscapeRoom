// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"

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
	FindPhysicsHandle();
	FindInputComponent();
	// Check for the Physcis Handle Component
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysiscsHandle->GrabbedComponent)
	{
		PhysiscsHandle->SetTargetLocation(GetLineTraceEnd());
	}
	// If the physics handle is attached
	// Move the object we are handling
}

void UGrabber::Grab()
{
	auto HitResult = GetFirstPhysicsBodyInReach();

	UPrimitiveComponent *ComponentToGrab = HitResult.GetComponent();
	// TODO: Try and reach any actors with physiscs body collision channel set.
	// if we hit something then attach the physics handle
	if (HitResult.GetActor())
	{
		PhysiscsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, GetLineTraceEnd());
	}
}

void UGrabber::Release()
{
	PhysiscsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsHandle()
{
	PhysiscsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysiscsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle Component for %s found! Is the component attached?"), *GetOwner()->GetName());
	}
}

void UGrabber::FindInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

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

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{

	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	// ray cast out to a certain distance
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerPositionInWorld(),
		GetLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams);

	// See what it hits
	AActor *ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Error, TEXT("Line trace has hit: %s"), *(ActorHit->GetName()));
	}

	return Hit;
}

FVector UGrabber::GetLineTraceEnd() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);
	return PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * reach);
}

FVector UGrabber::GetPlayerPositionInWorld() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);
	return PlayerViewPointLocation;
}