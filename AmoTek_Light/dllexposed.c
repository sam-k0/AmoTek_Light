#include "libblinkstick.h"
#include "gmsexport.h"
#include "config.h"
#include <stdbool.h>
#include <stdio.h>
/** Variable Defs */

blinkstick_device* deviceArray[BLINKSTICK_ARRAY_SIZE]; // Holds devices
bool deviceValid[BLINKSTICK_ARRAY_SIZE]; // If the device at deviceArray[i] is valid

GMBOOL initialized = GMFALSE;


#pragma region DLL specific
/**
 * Checks if the DLL is loaded successfully.
 * 
 * \param in Any value above 0
 * \return The val *2
 */
gmx GMINT checkConnection(GMINT in)
{
	return in * 2;
}


int getFreeDeviceSlot()
{
	int i = 0;
	for (i = 0; i < BLINKSTICK_ARRAY_SIZE; i++)
	{
		if (deviceValid[i] == false)
		{
			return i; // we found a free slot, that means it is NOT FULL
		}
	}
	// No free slots found
	return SLOTS_FULL;
}

#pragma endregion DLL Specific

/**
 * Initializes the DLL.
 * 
 * \return Success (1) or Fail (0)
 */
gmx GMBOOL initBlinkStick()
{
	if (initialized == GMTRUE)
	{
		return GMFALSE;
	}
	// Loop device array
	int i = 0;
	for (i = 0; i < BLINKSTICK_ARRAY_SIZE; i++)
	{
		deviceArray[i] = NULL;
		deviceValid[i] = false;
	}

	initialized = GMTRUE;
	return initialized;
}

/**
 * Finds the first available BlinkStick device.
 * 
 * \return Index to the device in the devices array
 */
gmx GMINT findDevice()
{
	// Exit when not inited
	if (initialized == GMFALSE) { return GMFALSE; }

	// Check if slots free
	int slot = getFreeDeviceSlot();
	if (slot == SLOTS_FULL)
	{
		return DEVICE_INVALID;
	}

	blinkstick_device* result = blinkstick_find();
	// Check if result is not null
	if (result == NULL)
	{
		return DEVICE_INVALID;
	}

	// Check if the handle is valid
	if (result->handle == NULL)
	{
		return DEVICE_INVALID;
	}

	// The device seems valid, set flag valid and save
	deviceValid[slot] = true;
	deviceArray[slot] = result;

	return (double)slot;
}


gmx GMBOOL freeDevice(GMINT slot)
{
	//Check if initialized
	if (initialized == GMFALSE) { return GMFALSE; }

	// Typecast slot
	int deviceSlot = (int)slot;

	// check for boundaries
	if (deviceSlot < 0 || deviceSlot >= BLINKSTICK_ARRAY_SIZE)
	{
		printf("Device slot OOB!");
		return GMFALSE;
	}

	// Check if the device is valid
	if (deviceValid[deviceSlot])
	{
		// Destroy the device
		blinkstick_destroy(deviceArray[deviceSlot]);
		deviceValid[deviceSlot] = false;
		printf("Released device at slot %d", deviceSlot);
		return GMTRUE;
	}
	// Invalid
	printf("Device does not exist at slot %d", deviceSlot);
	return GMFALSE;
}




