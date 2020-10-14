#include "AugmentaReceiver.h"
#include "OSCManager.h"


UAugmentaReceiver::UAugmentaReceiver()
{

}

UAugmentaReceiver::~UAugmentaReceiver()
{
	Stop();

	ActiveObjects.Empty();
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
	UAugmentaReceiver* Receiver = NewObject<UAugmentaReceiver>();
	Receiver->Connect(ReceiveIPAddress, Port);
	return Receiver;
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
	TArray<FAugmentaPerson> OutArray;
	ActiveObjects.GenerateValueArray(OutArray);
	return OutArray;
}

FAugmentaPerson UAugmentaReceiver::GetNewestPerson() const
{
	FAugmentaPerson Youngest;
	Youngest.Age = INT32_MAX;
	for (auto& Pair : ActiveObjects)
	{
		if (Pair.Value.Age < Youngest.Age)
		{
			Youngest = Pair.Value;
		}
	}
	return Youngest;
}

FAugmentaPerson UAugmentaReceiver::GetOldestPerson() const
{
	FAugmentaPerson Oldest;
	for (auto& Pair : ActiveObjects)
	{
		if (Pair.Value.Age > Oldest.Age)
		{
			Oldest = Pair.Value;
		}
	}
	return Oldest;
}

void UAugmentaReceiver::OnMessageReceived(const FOSCMessage& Message)
{
	const FOSCAddress Addr = Message.GetAddress();
	const FString Container = Addr.GetContainer(0);
	const FString Method = Addr.GetMethod();
	
	// Ensure it is an Augmenta message
	if (Container == ContainerObject)
	{
		// Send it off to the proper processing function based on the method
		if (Method == MethodObjectEnter || Method == MethodObjectUpdate)
		{
			UpdateObject(Message, Method == MethodObjectEnter);
		}
		else if (Method == MethodObjectLeave)
		{
			RemoveObject(Message);
		}
	}
	else if (Method == MethodScene)
	{
		UpdateScene(Message);
	}
	else if (Method == MethodVideoOutput)
	{
		UpdateVideoOutputData(Message);
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

void UAugmentaReceiver::UpdateObject(const FOSCMessage& Message, bool HasEntered)
{
	int32 Pid = -1;
	UOSCManager::GetInt32(Message, 1, Pid);

	// Find or add a person entry
	FAugmentaPerson Person = ActiveObjects.FindOrAdd(Pid);
	// Update the values
	Person.Pid = Pid;
	UOSCManager::GetInt32(Message, 0, Person.Frame);
	UOSCManager::GetInt32(Message, 2, Person.Oid);
	UOSCManager::GetFloat(Message, 3, Person.Age);
	UOSCManager::GetFloat(Message, 4, Person.Centroid.X);
	UOSCManager::GetFloat(Message, 5, Person.Centroid.Y);
	UOSCManager::GetFloat(Message, 6, Person.Velocity.X);
	UOSCManager::GetFloat(Message, 7, Person.Velocity.Y);
	UOSCManager::GetFloat(Message, 8, Person.Orientation);
	UOSCManager::GetFloat(Message, 9, Person.BoundingRectPos.X);
	UOSCManager::GetFloat(Message, 10, Person.BoundingRectPos.Y);
	UOSCManager::GetFloat(Message, 11, Person.BoundingRectSize.X);
	UOSCManager::GetFloat(Message, 12, Person.BoundingRectSize.Y);
	UOSCManager::GetFloat(Message, 13, Person.BoundingRectRotation);
	UOSCManager::GetFloat(Message, 14, Person.Height);

	ActiveObjects[Pid] = Person;

	if (HasEntered)
	{
		OnPersonEntered.Broadcast(Person);
	}
	else
	{
		OnPersonUpdated.Broadcast(Person);
	}
}

void UAugmentaReceiver::RemoveObject(const FOSCMessage& Message)
{
	int32 Pid = -1;
	UOSCManager::GetInt32(Message, 1, Pid);

	// Remove the person entry from the map
	FAugmentaPerson OldPerson;
	ActiveObjects.RemoveAndCopyValue(Pid, OldPerson);

	OnPersonWillLeave.Broadcast(OldPerson);
}

void UAugmentaReceiver::UpdateVideoOutputData(const FOSCMessage& Message)
{
	UOSCManager::GetFloat(Message, 0, VideoOutput.Offset.X);
	UOSCManager::GetFloat(Message, 1, VideoOutput.Offset.Y);
	UOSCManager::GetFloat(Message, 2, VideoOutput.Size.X);
	UOSCManager::GetFloat(Message, 3, VideoOutput.Size.Y);
	UOSCManager::GetInt32(Message, 4, VideoOutput.Resolution.X);
	UOSCManager::GetInt32(Message, 5, VideoOutput.Resolution.Y);

	OnVideoOutputUpdated.Broadcast(VideoOutput);
}
