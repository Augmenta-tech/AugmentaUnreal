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

void UAugmentaReceiver::Connect(FString ReceiveIPAddress, int32 Port)
{
	if (OSCServer) return;

	OSCServer = UOSCManager::CreateOSCServer(ReceiveIPAddress, Port, false, true);

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

UAugmentaReceiver* UAugmentaReceiver::CreateAugmentaReceiver(FString ReceiveIPAddress, int32 Port)
{
	UAugmentaReceiver* receiver = NewObject<UAugmentaReceiver>();
	receiver->Connect(ReceiveIPAddress, Port);
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

void UAugmentaReceiver::OnMessageReceived(const FOSCMessage& Message)
{
	FOSCAddress addr = Message.GetAddress();
	const FString Container = addr.GetContainer(0);
	
	// Ensure it is an Augmenta message
	if (Container == ObjectContainer)
	{
		const FString Method = addr.GetMethod();
		// Send it off to the proper processing function based on the method
		if (Method == ObjectMethodEnter || Method == ObjectMethodUpdate)
		{
			UpdatePerson(Message, Method == ObjectMethodEnter);
		}
		else if (Method == ObjectMethodLeave)
		{
			RemovePerson(Message);
		}
	}
	else if (Container == SceneContainer)
	{
		UpdateScene(Message);
	}
}

void UAugmentaReceiver::UpdateScene(const FOSCMessage& Message)
{
	// The way it is done in UOSCManager relies on private classes, so this may be the best way for now.
	UOSCManager::GetInt32(Message, 0, Scene.CurrentTime);
	UOSCManager::GetInt32(Message, 1, Scene.NumPeople);
	UOSCManager::GetFloat(Message, 2, Scene.SceneSize.X);
	UOSCManager::GetFloat(Message, 3, Scene.SceneSize.Y);

	OnSceneUpdated.Broadcast(Scene);
}

void UAugmentaReceiver::UpdatePerson(const FOSCMessage& Message, bool HasEntered)
{
	int32 pid = -1;
	UOSCManager::GetInt32(Message, 1, pid);

	// Find or add a person entry
	FAugmentaPerson person = ActivePersons.FindOrAdd(pid);
	// Update the values
	person.Pid = pid;
	UOSCManager::GetInt32(Message, 0, person.Frame);
	UOSCManager::GetInt32(Message, 2, person.Oid);
	UOSCManager::GetFloat(Message, 3, person.Age);
	UOSCManager::GetFloat(Message, 4, person.Centroid.X);
	UOSCManager::GetFloat(Message, 5, person.Centroid.Y);
	UOSCManager::GetFloat(Message, 6, person.Velocity.X);
	UOSCManager::GetFloat(Message, 7, person.Velocity.Y);
	UOSCManager::GetFloat(Message, 8, person.Orientation);
	UOSCManager::GetFloat(Message, 9, person.BoundingRectPos.X);
	UOSCManager::GetFloat(Message, 10, person.BoundingRectPos.Y);
	UOSCManager::GetFloat(Message, 11, person.BoundingRectSize.X);
	UOSCManager::GetFloat(Message, 12, person.BoundingRectSize.Y);
	UOSCManager::GetFloat(Message, 13, person.BoundingRectRotation);
	UOSCManager::GetFloat(Message, 14, person.Height);

	ActivePersons[pid] = person;

	if (HasEntered)
	{
		OnPersonEntered.Broadcast(person);
	}
	else
	{
		OnPersonUpdated.Broadcast(person);
	}
}

void UAugmentaReceiver::RemovePerson(const FOSCMessage& Message)
{
	int32 pid = -1;
	UOSCManager::GetInt32(Message, 1, pid);

	// Remove the person entry from the map
	FAugmentaPerson oldPerson;
	ActivePersons.RemoveAndCopyValue(pid, oldPerson);

	OnPersonWillLeave.Broadcast(oldPerson);
}
