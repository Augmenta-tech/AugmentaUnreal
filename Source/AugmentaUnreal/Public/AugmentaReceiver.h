#pragma once

#include "CoreMinimal.h"
#include "OSCMessage.h"
#include "AugmentaData.h"
#include "AugmentaReceiver.generated.h"

/** Forward Declarations */
class UOSCServer;

/** Delegates */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSceneUpdatedEvent, const FAugmentaScene, Scene);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPersonUpdatedEvent, const FAugmentaPerson, Person);

/**
 * A child class of UObject that is responsible for :
 * - Connecting to the OSCServer with the given Ip Address and Port.
 * - Processing the OSC Messages received from the Augmenta Fusion or the Augmenta Node(s) and for firing off the OnSceneUpdated, 
     OnPersonEntered, OnPersonUpdated and OnPersonWillLeave events that can be used in Blueprints.
 * - Stopping/disconnecting the connection to the OSCServer.
 */
UCLASS(BlueprintType, Category = "Augmenta")
class AUGMENTAUNREAL_API UAugmentaReceiver : public UObject
{
	GENERATED_BODY()

public:
	/** Ctor */
	UAugmentaReceiver();
	/** Destructor */
	~UAugmentaReceiver();

	/** 
	 * Connects to the OSCServer with the given ip address and port.
	 * 
	 * @param ReceiveIPAddress The ip address of the device to connect to get the OSC Messages.
	 * @param Port The port of the device to listen to to get the OSC Messages.
	 */
	UFUNCTION(BlueprintCallable, Category = "Augmenta")
	void Connect(FString ReceiveIPAddress, int32 Port);

	/** Stops the connection with the OSCServer. */
	UFUNCTION(BlueprintCallable, Category = "Augmenta")
	void Stop();

	/**
	 * Creates an instance of the UAugmentaReceiver and connects to the OSCServer with the given details.
	 *
	 * @param ReceiveIPAddress The ip address of the device to connect to get the OSC Messages.
	 * @param Port The port of the device to listen to to get the OSC Messages.
	 *
	 * @return UAugmentaReceiver* A pointer to the created instance of UAugmentaReceiver.
	 */
	UFUNCTION(BlueprintCallable, Category = "Augmenta")
	static UAugmentaReceiver* CreateAugmentaReceiver(FString ReceiveIPAddress, int32 Port);

	/** A delegate that is fired when an Augmenta scene OSC Message is received. */
	UPROPERTY(BlueprintAssignable, Category = "Augmenta")
	FSceneUpdatedEvent OnSceneUpdated;

	/** A delegate that is fired when an Augmenta Person Entered OSC Message is received. */
	UPROPERTY(BlueprintAssignable, Category = "Augmenta")
	FPersonUpdatedEvent OnPersonEntered;

	/** A delegate that is fired when an Augmenta Person Updated OSC Message is received. */
	UPROPERTY(BlueprintAssignable, Category = "Augmenta")
	FPersonUpdatedEvent OnPersonUpdated;

	/** A delegate that is fired when an Augmenta Person Will Leave OSC Message is received. */
	UPROPERTY(BlueprintAssignable, Category = "Augmenta")
	FPersonUpdatedEvent OnPersonWillLeave;

	/** Returns if the OSCServer is active and connected. */
	UFUNCTION(BlueprintPure, Category = "Augmenta")
	bool IsConnected() const;

	/** Returns the current Augmenta Scene. */
	UFUNCTION(BlueprintPure, Category = "Augmenta")
	FAugmentaScene GetScene() const;

	/** Returns an array containing the Augmenta Persons being tracked. */
	UFUNCTION(BlueprintPure, Category = "Augmenta")
	TArray<FAugmentaPerson> GetPersonsArray() const;

	/**
	 * Returns the Augmenta Person that has the lowest age in the scene i.e., 
	 * the person who recently entered the scene.
	 */
	UFUNCTION(BlueprintPure, Category = "Augmenta")
	FAugmentaPerson GetNewestPerson() const;

	/** 
	 * Returns the Augmenta Person that has the highest age in the scene i.e., 
	 * the person who has been there for the longest time.
	 */
	UFUNCTION(BlueprintPure, Category = "Augmenta")
	FAugmentaPerson GetOldestPerson() const;

private:

	/**
	 * Processes the valid Augmenta OSC Message and gets the data accordingly for the Augmenta Scene and
	 * Augmenta Persons.
	 * 
	 * @param Message The OSC Message to process and get the Augmenta data.
	 */
	UFUNCTION()
	void OnMessageReceived(const FOSCMessage& Message);

	/** Processes the Augmenta Scene OSC Message. */
	void UpdateScene(const FOSCMessage& Message);
	/** Processes the Augmenta Person Entered and Updated OSC Message. */
	void UpdatePerson(const FOSCMessage& Message, bool HasEntered);
	/** Processes the Augmenta Person Will Leave OSC Message. */
	void RemovePerson(const FOSCMessage& Message);
	
	/** The OSCServer that is used to connect and stop. */
	UPROPERTY()
	UOSCServer* OSCServer;

	/** The current Augmenta scene where the Augmenta persons are being tracked. */
	FAugmentaScene Scene;
	/** A key value pair that stores the Augmenta Persons being tracked with the their Pid as the unique key. */
	TMap<int32, FAugmentaPerson> ActivePersons;

	const FString ObjectContainer = "object";
	const FString SceneContainer = "scene";
	const FString ObjectMethodEnter = "enter";
	const FString ObjectMethodUpdate = "update";
	const FString ObjectMethodLeave = "leave";
};
