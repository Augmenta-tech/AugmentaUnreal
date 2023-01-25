# [Augmenta](https://www.augmenta-tech.com) [Unreal](https://www.unrealengine.com) Plugin

This repo contains the Augmenta Unreal Plugin only. It is currently compiled with Unreal version **5.1**.

In order to use this plugin in any Unreal project, kindly make this repo as a submodule inside the `Plugins` folder similar to the [Augmenta Unreal Demo project](https://github.com/Augmenta-tech/AugmentaUnreal-Demo).

## Features implementation

### OSC Protocol V1
[V1 Wiki](https://github.com/Augmenta-tech/Augmenta/wiki/Data)

### OSC Protocol V2
[V2 Wiki](https://github.com/Augmenta-tech/Augmenta/wiki/Data)
 - There are very few API breaking changes in V2.
 - `Augmenta Person` is now referred to as `Augmenta Object` and changes have been made to the plugin in a way that it doesn't break the API.
 - Some of the data in the Augmenta Object is moved into Extra data to correspond to the OSC message.

## Dependency

This plugin depends on the `OSC` Plugin by Epic Games Inc. which is enabled in the `Plugins` section of [AugmentaUnreal.uplugin](AugmentaUnreal.uplugin#L25) and also added to the `PrivateDependencyModuleNames` in the [AugmentaUnreal.Build.cs](Source/AugmentaUnreal/AugmentaUnreal.Build.cs#L42).

## Plugin Source

 - [AugmentaReceiver](Source/AugmentaUnreal/Public/AugmentaReceiver.h#L25) : A child class of UObject and is responsible for the following actions.
 	- Connecting to the `OSCServer` with the given Ip Address and Port.
	- Processing the OSC Messages received from the `Augmenta Fusion` or the `Augmenta Node(s)` and for firing off the `OnSceneUpdated`, `OnObjectEntered`, `OnObjectUpdated`, `OnObjectLeft`, `OnVideoOutputUpdated`, `OnEnteredExtraData`, `OnUpdatedExtraData` and `OnLeaveExtraData` events that can be used in Blueprints.
	- Stopping/disconnecting the connection to the `OSCServer`.

 - [AugmentaPerson](Source/AugmentaUnreal/Public/AugmentaData.h#L9) : A struct to hold the data for the Augmenta Object like the `Frame`, `Id`, `Oid`, `Age`, `Centroid`, `Velocity`, `Orientation`, `BoundingRectPos`, `BoundingRectSize`, `BoundingRectRotation`, `Height`.
 - [AugmentaScene](Source/AugmentaUnreal/Public/AugmentaData.h#L65) : A struct to hold the data for the Augmenta Scene like the `Frame`, `ObjectCount`, `SceneSize`.
 - [AugmentaVideoOutput](Source/AugmentaUnreal/Public/AugmentaData.h#L86) : A struct to hold the data for the Augmenta VideoOutput like the `Offset`, `Size`, `Resolution`.
 - [AugmentaObjectExtra](Source/AugmentaUnreal/Public/AugmentaData.h#L107) : A struct to hold the data for the Augmenta Object Extra data like the `Frame`, `Id`, `Oid`, `Highest`, `Distance`, `Reflectivity`.
