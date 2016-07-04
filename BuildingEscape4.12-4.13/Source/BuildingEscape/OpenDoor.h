// Copyright Ben Tristem 2016.

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

//below is macro for blueprint acces to cpp code
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent); //it is like creating a class inside class in braces

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
    
    //below is uproperty for blueprint
    UPROPERTY(BlueprintAssignable)
    FDoorEvent OnOpen;
    UPROPERTY(BlueprintAssignable)
    FDoorEvent OnClose;
    
    
private:
    UPROPERTY(EditAnywhere)
    ATriggerVolume* PressurePlate = nullptr;
    
    UPROPERTY(EditAnywhere)
    float TriggerMass = 30.f;
    
    // The owning door
    AActor* Owner = nullptr;
    
    // Returns total mass in kg
    float GetTotalMassOfActorsOnPlate();
};
