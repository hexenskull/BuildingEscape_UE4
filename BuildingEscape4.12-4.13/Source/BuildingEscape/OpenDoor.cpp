// Copyright Paul Buidenkov 2016

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
    
    Owner = GetOwner();
}

void UOpenDoor::OpenDoor()
{
    //Set door rotation
    Owner->SetActorRotation(FRotator(0.0f, OpenAngle-=3.f, 0.0f));
}


void UOpenDoor::CloseDoor()
{
    //Set door rotation
    Owner->SetActorRotation(FRotator(0.0f, OpenAngle+=3.f, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

    //Poll trigger volume
    if(GetTotalMassOfActorsOnPlate() > 30.f) //TODO make into parameter
    {
        if(OpenAngle != OpenedAngle)
        {
        OpenDoor(); //open door every frame
        }
    }
    else
    {
        if(ClosedAngle != OpenAngle)
        {
            CloseDoor();
        }
    }
    
}


float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
    float TotalMass = 0.f;
    
    //Find all overlapping actors
    TArray<AActor*> OverlappingActors;
    PressurePlate->GetOverlappingActors(OUT OverlappingActors); // this is an OUT parameter after passing empty array to this GetOverlappingActors function and it becomes not empy! (it fillse with overlapping actors!)
    
    //Iterate through them adding their masses
    
    for (const auto& Actor : OverlappingActors)
    {
        TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
    }
    
    
    
    return TotalMass;
}
