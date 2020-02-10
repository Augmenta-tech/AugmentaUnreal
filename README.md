# Augmenta Unreal Plugin

This repo contains the Augmenta Unreal Plugin only. It is currently compiled with Unreal version **4.23**.

In order to use this plugin in any Unreal project, kindly make this repo as a submodule inside the `Plugins` folder similar to the [Augmenta Unreal Demo project](https://github.com/Theoriz/AugmentaUnreal-Demo).

## Dependency

This plugin depends on the `OSC` Plugin by Epic Games Inc. which is enabled in the `Plugins` section of [AugmentaUnreal.uplugin](AugmentaUnreal.uplugin#L25) and also added to the `PrivateDependencyModuleNames` in the [AugmentaUnreal.Build.cs](Source/AugmentaUnreal/AugmentaUnreal.Build.cs#L42).

## Plugin Source

 - [AugmentaReceiver](Source/AugmentaUnreal/Public/AugmentaReceiver.h#L69) : A child class of UObject and is responsible for the following actions.
 	- Connecting to the `OSCServer` with the given Ip Address and Port.
	- Processing the OSC Messages received from the `Augmenta Fusion` or the `Augmenta Node(s)` and for firing off the `OnSceneUpdated`, `OnPersonEntered`, `OnPersonUpdated` and `OnPersonWillLeave` events that can be used in Blueprints.
	- Stopping/disconnecting the connection to the `OSCServer`.

 - [AugmentaPerson](Source/AugmentaUnreal/Public/AugmentaReceiver.h#L11) : A struct to hold the data for the Augmenta Person (blob) like the Pid, Oid, Age, Centroid, Velocity, Depth, BoundingRectPos, BoundingRectSize, Highest.
 - [AugmentaScene](Source/AugmentaUnreal/Public/AugmentaReceiver.h#L44) : A struct to hold the data for the Augmenta Scene like the CurrentTime, PercentCovered, NumPeople, AverageMotion, SceneSize.
