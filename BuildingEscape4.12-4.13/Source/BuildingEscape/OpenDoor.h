// Copyright Paul Buidenkov 2016

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
    
    //my OpenDoor method declaration
    void OpenDoor();
    void CloseDoor();

    
//My addition below private:
private:
    
    float ClosedAngle = 0.0f;
    
    float OpenedAngle = -90.f;
    
    UPROPERTY(EditAnywhere)
    float OpenAngle = 0.0f;
    
    UPROPERTY(EditAnywhere)
    ATriggerVolume* PressurePlate;
    
    UPROPERTY(EditAnywhere)
    float DoorCloseDelay = 1.f; //here it simply does nothing since I made myself smooth animation of door opening and closing
    
    float LastDoorOpenTime; //this is private var WITHOUR UPROPERTY!!!
    
    AActor* ActorThatOpens; //remember pawn inherits from actor
    AActor* Owner; //the owning door

};
