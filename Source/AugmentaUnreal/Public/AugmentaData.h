#pragma once

#include "AugmentaData.generated.h"

/** 
 * A structure to hold the data for the Augmenta Object.
 */
USTRUCT(BlueprintType, Category = "Augmenta|Data")
struct FAugmentaPerson
{
	GENERATED_BODY()

	/** The scene frame number. */
	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Person")
	int32 Frame;
	
	/** The unique Id for each object. (ex: 42nd object to enter is assigned pid=41). */
	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Person")
	int32 Pid;

	/**
	 * The Ordered Id for each object.
	 * (ex: if 3 objects are on stage, 43rd object still present has oid=2).
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Person")
	int32 Oid;

	/** The Alive time in seconds. */
	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Person")
	float Age;

	/** The position projected to the ground (normalized). */
	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Person")
	FVector2D Centroid;

	/** The speed and direction vector (normalized). */
	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Person")
	FVector2D Velocity;

	/** The CCW rotation w.r.t the horizontal axis (right). Range is 0 to 360. */
	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Person")
	float Orientation;

	/** The center co-ordinate of the bounding box (normalized). */
	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Person")
	FVector2D BoundingRectPos;

	/** The size of the bounding box (normalized). */
	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Person")
	FVector2D BoundingRectSize;

	/** The CCW rotation of the bounding box w.r.t the horizontal axis (right). */
	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Person")
	float BoundingRectRotation;

	/** The Height of the Object (in m) (absolute). */
	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Person")
	float Height;
};

/** 
 * A structure to hold data for the Augmenta Scene.
 */
USTRUCT(BlueprintType, Category = "Augmenta|Data")
struct FAugmentaScene
{
	GENERATED_BODY()

	/** The Time in frame number. */
	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Scene")
	int32 CurrentTime;

	/** The number of objects in the scene. */
	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Scene")
	int32 NumPeople;

	/** The scene size in meters. */
	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|Scene")
	FVector2D SceneSize;
};

/** 
 * A structure to hold data for the Augmenta VideoOutput (Fusion).
 */
USTRUCT(BlueprintType, Category = "Augmenta|Data")
struct FAugmentaVideoOutput
{
	GENERATED_BODY()

	/** The offset from the scene top left (in m). */
	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|VideoOutput")
	FVector2D Offset;

	/** The size (in m). X is Width and Y is Height. */
	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|VideoOutput")
	FVector2D Size;

	/** The resolution in pixels. */
	UPROPERTY(BlueprintReadOnly, Category = "Augmenta|VideoOutput")
	FIntPoint Resolution;
};
