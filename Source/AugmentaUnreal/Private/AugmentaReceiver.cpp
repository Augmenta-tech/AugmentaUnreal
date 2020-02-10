#include "AugmentaReceiver.h"
#include "OSCManager.h"


UAugmentaReceiver::UAugmentaReceiver()
{

}

UAugmentaReceiver::~UAugmentaReceiver()
{
	Stop();

	ActivePersons.Empty();
}

void UAugmentaReceiver::Connect(FString receiveIPAddress, int32 port)
{
	if (OSCServer) return;

	OSCServer = UOSCManager::CreateOSCServer(receiveIPAddress, port, false, true);

	OSCServer->OnOscMessageReceived.AddDynamic(this, &UAugmentaReceiver::OnMessageReceived);
}

void UAugmentaReceiver::Stop()
{
	if (OSCServer && OSCServer->IsValidLowLevel())
	{
		OSCServer->Stop(); //This is called by its destruction, calling it here to be safe.
		OSCServer->ConditionalBeginDestroy();
		OSCServer = nullptr;
	}
}

UAugmentaReceiver* UAugmentaReceiver::CreateAugmentaReceiver(FString receiveIPAddress, int32 port)
{
	UAugmentaReceiver* receiver = NewObject<UAugmentaReceiver>();
	receiver->Connect(receiveIPAddress, port);
	return receiver;
}

bool UAugmentaReceiver::IsConnected() const
{
	return (OSCServer && OSCServer->IsValidLowLevel() && OSCServer->IsActive());
}

FAugmentaScene UAugmentaReceiver::GetScene() const
{
	return Scene;
}

TArray<FAugmentaPerson> UAugmentaReceiver::GetPersonsArray() const
{
	TArray<FAugmentaPerson> outArray;
	ActivePersons.GenerateValueArray(outArray);
	return outArray;
}

FAugmentaPerson UAugmentaReceiver::GetNewestPerson() const
{
	FAugmentaPerson youngest;
	youngest.Age = INT32_MAX;
	for (auto& pair : ActivePersons)
	{
		if (pair.Value.Age < youngest.Age)
		{
			youngest = pair.Value;
		}
	}
	return youngest;
}

FAugmentaPerson UAugmentaReceiver::GetOldestPerson() const
{
	FAugmentaPerson oldest;
	for (auto& pair : ActivePersons)
	{
		if (pair.Value.Age > oldest.Age)
		{
			oldest = pair.Value;
		}
	}
	return oldest;
}

void UAugmentaReceiver::OnMessageReceived(const FOSCMessage& message)
{
	FOSCAddress addr = message.GetAddress();
	
	// Ensure it is an Augmenta message
	if (addr.GetContainer(0) != "au") return;

	// Send it off to the proper processing function based on the method
	if (addr.GetMethod() == "scene")
	{
		UpdateScene(message);
	}
	else if (addr.GetMethod() == "personUpdated" || addr.GetMethod() == "personEntered")
	{
		UpdatePerson(message, addr.GetMethod() == "personEntered");
	}
	else if (addr.GetMethod() == "personWillLeave")
	{
		RemovePerson(message);
	}
}

void UAugmentaReceiver::UpdateScene(const FOSCMessage& message)
{
	// The way it is done in UOSCManager relies on private classes, so this may be the best way for now.
	UOSCManager::GetInt32(message, 0, Scene.CurrentTime);
	UOSCManager::GetFloat(message, 1, Scene.PercentCovered);
	UOSCManager::GetInt32(message, 2, Scene.NumPeople);
	UOSCManager::GetFloat(message, 3, Scene.AverageMotion.X);
	UOSCManager::GetFloat(message, 4, Scene.AverageMotion.Y);
	UOSCManager::GetInt32(message, 5, Scene.SceneSize.X);
	UOSCManager::GetInt32(message, 6, Scene.SceneSize.Y);
	UOSCManager::GetInt32(message, 7, Scene.SceneSize.Z);

	OnSceneUpdated.Broadcast(Scene);
}

void UAugmentaReceiver::UpdatePerson(const FOSCMessage& message, bool hasEntered)
{
	int32 pid = -1;
	UOSCManager::GetInt32(message, 0, pid);

	// Find or add a person entry
	FAugmentaPerson person = ActivePersons.FindOrAdd(pid);
	// Update the values
	person.Pid = pid;
	UOSCManager::GetInt32(message, 1, person.Oid);
	UOSCManager::GetInt32(message, 2, person.Age);
	UOSCManager::GetFloat(message, 3, person.Centroid.X);
	UOSCManager::GetFloat(message, 4, person.Centroid.Y);
	UOSCManager::GetFloat(message, 5, person.Velocity.X);
	UOSCManager::GetFloat(message, 6, person.Velocity.Y);
	UOSCManager::GetFloat(message, 7, person.Depth);
	UOSCManager::GetFloat(message, 8, person.BoundingRectPos.X);
	UOSCManager::GetFloat(message, 9, person.BoundingRectPos.Y);
	UOSCManager::GetFloat(message, 10, person.BoundingRectSize.X);
	UOSCManager::GetFloat(message, 11, person.BoundingRectSize.Y);
	UOSCManager::GetFloat(message, 12, person.Highest.X);
	UOSCManager::GetFloat(message, 13, person.Highest.Y);
	UOSCManager::GetFloat(message, 14, person.Highest.Z);

	ActivePersons[pid] = person;

	if (hasEntered)
		OnPersonEntered.Broadcast(person);
	else
		OnPersonUpdated.Broadcast(person);
}

void UAugmentaReceiver::RemovePerson(const FOSCMessage& message)
{
	int32 pid = -1;
	UOSCManager::GetInt32(message, 0, pid);

	// Remove the person entry from the map
	FAugmentaPerson oldPerson;
	ActivePersons.RemoveAndCopyValue(pid, oldPerson);

	OnPersonWillLeave.Broadcast(oldPerson);
}
