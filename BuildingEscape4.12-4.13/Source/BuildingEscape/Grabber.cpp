// Copyright Paul Buidenkov 2016

#include "BuildingEscape.h"
#include "Grabber.h"

//my macro definition(it is for notes, it does nothing!)
#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	///Look for attached Physycs Handle
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>(); // in the braces you actually don't need anything in this case
    if (PhysicsHandle)
    {
        //Physics Handle is found
        //don't have to write here anything in this case
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Error: No Pysycs component found for %s actor. Add to PhysicsHandle in Grabber.cpp finder to PhysycsHandleComponent in <>!"), *(GetOwner()->GetName()))
    }
    
    ///Look for attached input component
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (InputComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("InputComponent for %s found!"), *GetOwner()->GetName());
        ///Bind input axis
        InputComponent->BindAction(
            "Grab", //attention here must be the same name "Grab" as in ProjectSettings->Engine->Input->ActionMappings "Grab"! Otherwise it will not react on your pressing buttons
            IE_Pressed, // action on when the button is pressed
            this, // this means this object (Grabber)
            &UGrabber::Grab // method witch performe some actions
        );
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Error: No input component found for %s actor. "), *(GetOwner()->GetName()))
    }
    
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	/// Get player view point this tick
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
        OUT PlayerViewPointLocation,
        OUT PlayerViewPointRotation
    );
    
   // UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), OUT *PlayerViewPointLocation.ToString(), OUT *PlayerViewPointRotation.ToString())
    
    /// Draw red trace in a world to visualise
    FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
    
    /// Draw a red trace in the world to visualise
    DrawDebugLine(
        GetWorld(),
        PlayerViewPointLocation,
        LineTraceEnd,
        FColor(255, 0, 0),
        false,
        0.f,
        0.f,
        2.f
    );
    
    ///Setup query parameters
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner()); // first parameter is name and it is empty, next parameter FALSE for tetermining Collision volume NOT vision vlume with all details!, third parameter is ignoring first collision(we ignore Owner cause debug red ray goes straight from the center of the sphere of the player!)
    

    /// Line trace (AKA ray-cast) out to reach distance
    FHitResult Hit;
    
    GetWorld()->LineTraceSingleByObjectType(
        OUT Hit, //mark it with OUT to remember that it is OUT parameter
        PlayerViewPointLocation,
        LineTraceEnd,
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParameters
    );
    /// See what we hit
    AActor* ActorHit = Hit.GetActor();
    if (ActorHit)
    {
    UE_LOG(LogTemp, Warning, TEXT("Hit ActorName: %s"), *(ActorHit->GetName()))
    }
}

void UGrabber::Grab()
{
    UE_LOG(LogTemp, Warning, TEXT("Grab pressed!"))
}
