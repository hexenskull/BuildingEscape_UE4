// Copyright Paul Buidenkov 2016

#include "BuildingEscape.h"
#include "OpenDoor.h"


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
    
    ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
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

	// Poll the TriggerVolume
    if(PressurePlate->IsOverlappingActor(ActorThatOpens))
    {
        //OpenAngle = -90.0f;
        if(OpenAngle != OpenedAngle)
        {
        OpenDoor(); //open door every frame
        }
        //LastDoorOpenTime = GetWorld()->GetTimeSeconds();
    }
    else
    {
        //CloseAngle = 0.0f;
        if(ClosedAngle != OpenAngle)
        {
            CloseDoor();
        }
    }
    
    //check if it is time to close the door
//    if(GetWorld()->GetTimeSeconds() > LastDoorOpenTime + DoorCloseDelay)
//    {
//        if(OpenAngle != CloseAngle)
//        {
//        CloseDoor();
//        }
//    }
}
    

