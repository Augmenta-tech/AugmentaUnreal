// Copyright Augmenta, All Rights Reserved.

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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVideoOutputUpdatedEvent, const FAugmentaVideoOutput, VideoOutput);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FExtraDataEvent, const FAugmentaObjectExtra, ExtraData);

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
	 * @param Port The port of the device to listen to, to get the OSC Messages.
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
	 * @param Port The port of the device to listen to, to get the OSC Messages.
	 *
	 * @return UAugmentaReceiver* A pointer to the created instance of UAugmentaReceiver.
	 */
	UFUNCTION(BlueprintCallable, Category = "Augmenta")
	static UAugmentaReceiver* CreateAugmentaReceiver(FString ReceiveIPAddress, int32 Port);

	/** A delegate that is fired when an Augmenta scene OSC Message is received. */
	UPROPERTY(BlueprintAssignable, Category = "Augmenta")
	FSceneUpdatedEvent OnSceneUpdated;

	/** A delegate that is fired when an Augmenta Object Entered OSC Message is received. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnObjectEntered", Category = "Augmenta")
	FPersonUpdatedEvent OnPersonEntered;

	/** A delegate that is fired when an Augmenta Object Updated OSC Message is received. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnObjectUpdated", Category = "Augmenta")
	FPersonUpdatedEvent OnPersonUpdated;

	/** A delegate that is fired when an Augmenta Object Will Leave OSC Message is received. */
	UPROPERTY(BlueprintAssignable, DisplayName = "OnObjectLeft", Category = "Augmenta")
	FPersonUpdatedEvent OnPersonWillLeave;

	/** A delegate that is fired when an Augmenta video output (fusion) OSC Message is received. */
	UPROPERTY(BlueprintAssignable, Category = "Augmenta")
	FVideoOutputUpdatedEvent OnVideoOutputUpdated;

	/** A delegate that is fired when an Augmenta object enter extra data OSC Message is received. */
	UPROPERTY(BlueprintAssignable, Category = "Augmenta")
	FExtraDataEvent OnEnteredExtraData;

	/** A delegate that is fired when an Augmenta object update extra data OSC Message is received. */
	UPROPERTY(BlueprintAssignable, Category = "Augmenta")
	FExtraDataEvent OnUpdatedExtraData;
	
	/** A delegate that is fired when an Augmenta object leave extra data OSC Message is received. */
	UPROPERTY(BlueprintAssignable, Category = "Augmenta")
	FExtraDataEvent OnLeaveExtraData;

	/** Returns if the OSCServer is active and connected. */
	UFUNCTION(BlueprintPure, Category = "Augmenta")
	bool IsConnected() const;

	/** Returns the current Augmenta Scene. */
	UFUNCTION(BlueprintPure, Category = "Augmenta")
	FAugmentaScene GetScene() const;

	/** Returns an array containing the Augmenta Objects being tracked. */
	UFUNCTION(BlueprintPure, DisplayName = "GetObjectsArray", Category = "Augmenta")
	TArray<FAugmentaPerson> GetPersonsArray() const;

	/**
	 * Returns the Augmenta Object that has the lowest age in the scene i.e., 
	 * the object who recently entered the scene.
	 */
	UFUNCTION(BlueprintPure, DisplayName = "GetNewestObject", Category = "Augmenta")
	FAugmentaPerson GetNewestPerson() const;

	/** 
	 * Returns the Augmenta Object that has the highest age in the scene i.e., 
	 * the object who has been there for the longest time.
	 */
	UFUNCTION(BlueprintPure, DisplayName = "GetOldestObject", Category = "Augmenta")
	FAugmentaPerson GetOldestPerson() const;

	/**
	 * Helper function to find the Object with a given Id.
	 *
	 * @param Id The Id used to find the Augmenta Object.
	 * @param Object The Augmenta Object with the given Id if successful.
	 *
	 * @return true if successful, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Augmenta")
	bool GetObject(const int32 Id, FAugmentaPerson& Object) const;

	/** Returns the current Augmenta VideoOutput Data. */
	UFUNCTION(BlueprintPure, Category = "Augmenta")
	FAugmentaVideoOutput GetVideoOutput() const;

	/** Returns an array containing the Augmenta Objects Extra data. */
	UFUNCTION(BlueprintPure, Category = "Augmenta")
	TArray<FAugmentaObjectExtra> GetObjectExtrasArray() const;

	/**
	 * Helper function to find the Object Extra data with a given Id.
	 *
	 * @param Id The Id used to find the Augmenta Object Extra data.
	 * @param Extra The Augmenta Object Extra data with the given Id if successful.
	 *
	 * @return true if successful, false otherwise.
	 */
	UFUNCTION(BlueprintPure, Category = "Augmenta")
	bool GetObjectExtra(const int32 Id, FAugmentaObjectExtra& Extra) const;

private:

	/** The OSCServer that is used to connect and stop. */
	UPROPERTY()
	UOSCServer* OSCServer;

	/** The current Augmenta scene where the Augmenta objects are being tracked. */
	FAugmentaScene Scene;
	/** A key value pair that stores the Augmenta Objects being tracked with the their id as the unique key. */
	TMap<int32, FAugmentaPerson> ActiveObjects;
	/** The current Augmenta VideoOutput data. */
	FAugmentaVideoOutput VideoOutput;
	/** A key value pair that stores the Augmenta Objects extra data with the their id as the unique key. */
	TMap<int32, FAugmentaObjectExtra> ActiveObjectsExtraData;

	const FString ContainerObject = "object";
	const FString MethodScene = "scene";
	const FString MethodObjectEnter = "enter";
	const FString MethodObjectUpdate = "update";
	const FString MethodObjectLeave = "leave";
	const FString MethodVideoOutput = "fusion";
	const FString MethodObjectExtra = "extra";
	
	/**
	 * Processes the valid Augmenta OSC Message and gets the data accordingly for the Augmenta Scene and
	 * Augmenta Objects.
	 * 
	 * @param Message The OSC Message to process and get the Augmenta data.
	 * @param IPAddress The OSC receive IP address of the device.
	 * @param Port The port of the device where the OSC Messages are received.
	 */
	UFUNCTION()
	void OnMessageReceived(const FOSCMessage& Message, const FString& IPAddress, int32 Port);

	/** Processes the Augmenta Scene OSC Message. */
	void UpdateScene(const FOSCMessage& Message);
	/** Processes the Augmenta Object Entered and Updated OSC Message. */
	void UpdateObject(const FOSCMessage& Message, bool HasEntered);
	/** Processes the Augmenta Object Will Leave OSC Message. */
	void RemoveObject(const FOSCMessage& Message);
	/** Processes the Augmenta VideoOutput (Fusion) OSC Message. */
	void UpdateVideoOutputData(const FOSCMessage& Message);
	/** Processes the Augmenta Object enter and update extra data OSC Message. */
	void UpdateObjectExtraData(const FOSCMessage& Message, bool HasEntered);
	/** Processes the Augmenta Object leave extra data OSC Message. */
	void RemoveObjectExtraData(const FOSCMessage& Message);
};
