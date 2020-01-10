#pragma once

#include "CoreMinimal.h"
#include "OSCMessage.h"
#include "AugmentaReceiver.generated.h"

//Forward Declarations
class UOSCServer;

USTRUCT(BlueprintType, Category = "Augmenta|Data")
struct FAugmentaPerson
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Person")
	int32 Pid;

	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Person")
	int32 Oid;

	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Person")
	int32 Age;

	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Person")
	FVector2D Centroid;

	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Person")
	FVector2D Velocity;

	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Person")
	float Depth;

	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Person")
	FVector2D BoundingRectPos;

	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Person")
	FVector2D BoundingRectSize;

	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Person")
	FVector Highest;
};

USTRUCT(BlueprintType, Category = "Augmenta|Data")
struct FAugmentaScene
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Scene")
	int32 CurrentTime;
	
	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Scene")
	float PercentCovered;
	
	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Scene")
	int32 NumPeople;

	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Scene")
	FVector2D AverageMotion;

	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Scene")
	FIntVector SceneSize;
};

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSceneUpdatedEvent, const FAugmentaScene, Scene);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPersonUpdatedEvent, const FAugmentaPerson, Person);

UCLASS(BlueprintType, Category = "Augmenta")
class AUGMENTAUNREAL_API UAugmentaReceiver : public UObject
{
	GENERATED_BODY()

public:
	UAugmentaReceiver();
	~UAugmentaReceiver();

	UFUNCTION(BlueprintCallable, Category = "Augmenta")
	void Connect(FString receiveIPAddress, int32 port);

	UFUNCTION(BlueprintCallable, Category = "Augmenta")
	void Stop();

	UFUNCTION(BlueprintCallable, Category = "Augmenta")
	static UAugmentaReceiver* CreateAugmentaReceiver(FString receiveIPAddress, int32 port);

	UPROPERTY(BlueprintAssignable, Category = "Augmenta")
	FSceneUpdatedEvent OnSceneUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Augmenta")
	FPersonUpdatedEvent OnPersonEntered;

	UPROPERTY(BlueprintAssignable, Category = "Augmenta")
	FPersonUpdatedEvent OnPersonUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Augmenta")
	FPersonUpdatedEvent OnPersonWillLeave;

	UFUNCTION(BlueprintPure, Category = "Augmenta")
	bool IsConnected() const;

	UFUNCTION(BlueprintPure, Category = "Augmenta")
	FAugmentaScene GetScene() const;

	UFUNCTION(BlueprintPure, Category = "Augmenta")
	TArray<FAugmentaPerson> GetPersonsArray() const;

	UFUNCTION(BlueprintPure, Category = "Augmenta")
	FAugmentaPerson GetNewestPerson() const;

	UFUNCTION(BlueprintPure, Category = "Augmenta")
	FAugmentaPerson GetOldestPerson() const;

private:

	UPROPERTY()
	UOSCServer* OSCServer;

	FAugmentaScene Scene;
	TMap<int32, FAugmentaPerson> ActivePersons;

	UFUNCTION()
	void OnMessageReceived(const FOSCMessage& message);

	// Data Processing functions
	void UpdateScene(const FOSCMessage& message);
	void UpdatePerson(const FOSCMessage& message, bool hasEntered);
	void RemovePerson(const FOSCMessage& message);
};
