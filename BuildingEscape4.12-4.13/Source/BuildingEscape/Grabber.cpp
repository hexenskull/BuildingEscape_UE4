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
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
    FindPhysicsHandleComponent();
    SetupInputComponent();
    
}

void UGrabber::Grab()
{
    ///LINE TRACE and see if we reach any actor with physics body collition channel set
    auto HitResult = GetFirstPhysicsBodyInReach();
    auto ComponentToGrab = HitResult.GetComponent();//what part of the chair we need to grab
    auto ActorHit = HitResult.GetActor();
    
    ///If we hit something, then attach a physics handle
    if (ActorHit)
    {
        if(!PhysicsHandle) { return; }
        PhysicsHandle->GrabComponent(
                                     ComponentToGrab,
                                     NAME_None, //no bones needed
                                     ComponentToGrab->GetOwner()->GetActorLocation(),
                                     true //allow rotation
                                     );
    }
}

void UGrabber::Release()
{
    if(!PhysicsHandle) { return; }
    PhysicsHandle->ReleaseComponent();
}

///Look for attached Physycs Handle
void UGrabber::FindPhysicsHandleComponent()
{
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>(); // in the braces you actually don't need anything in this case
    if (PhysicsHandle == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Error: No Pysycs component found for %s actor. Add to PhysicsHandle in Grabber.cpp finder to PhysycsHandleComponent in <>!"), *(GetOwner()->GetName()))
    }
}

///Look for attached input component
void UGrabber::SetupInputComponent()
{
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
        
        InputComponent->BindAction(
                                   "Grab",
                                   IE_Released,
                                   this,
                                   &UGrabber::Release
                                   );
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Error: No input component found for %s actor. "), *(GetOwner()->GetName()))
    }
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
    /// Line trace (AKA ray-cast) out to reach distance
    FHitResult HitResult;
    
    ///Setup query parameters
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner()); // first parameter is name and it is empty, next parameter FALSE for tetermining Collision volume NOT vision vlume with all details!, third parameter is ignoring first collision(we ignore Owner cause debug red ray goes straight from the center of the sphere of the player!)
    
    GetWorld()->LineTraceSingleByObjectType(
                                            OUT HitResult, //mark it with OUT to remember that it is OUT parameter
                                            GetReachLineStart(),
                                            GetReachLineEnd(),
                                            FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
                                            TraceParameters
                                            );
    
    return HitResult;
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    
    if(!PhysicsHandle) { return; }
    //if the physics handle is attached
    if (PhysicsHandle->GrabbedComponent)
    {
        //move the object we're holding
        PhysicsHandle->SetTargetLocation(GetReachLineEnd());
    }
}

FVector UGrabber::GetReachLineStart()
{
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
                                                               OUT PlayerViewPointLocation,
                                                               OUT PlayerViewPointRotation
                                                               );
    
    /// Draw red trace in a world to visualise
    return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;

    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
                                                           OUT PlayerViewPointLocation,
                                                           OUT PlayerViewPointRotation
                                                           );

    /// Draw red trace in a world to visualise
    return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}