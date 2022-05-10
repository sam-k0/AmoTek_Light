#include "libblinkstick.h"
#include "gmsexport.h"
#include "config.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
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
gmx GMBOOL initEx(GMBOOL debug)
{
	if (initialized == GMTRUE)
	{
		return GMFALSE;
	}

	if (debug == GMTRUE)
	{
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
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
		printf("Device slots full!");
		return DEVICE_INVALID;
	}

	blinkstick_device* result = blinkstick_find();
	// Check if result is not null
	if (result == NULL)
	{
		printf("Unknown error when searching devices / Is the device connected?");
		return DEVICE_INVALID;
	}

	// Check if the handle is valid
	if (result->handle == NULL)
	{
		printf("Invalid device handle");
		return DEVICE_INVALID;
	}

	// The device seems valid, set flag valid and save
	deviceValid[slot] = true;
	deviceArray[slot] = result;
	printf("Success on saving device");
	return (double)slot;
}

/**
 * Releases the device.
 * 
 * \param slot
 * \return success
 */
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



/**
 * Sets the lighting.
 * 
 * \param slot
 * \param channel
 * \param index
 * \param red
 * \param blue
 * \param green
 * \return success
 */
gmx GMBOOL setDeviceLighting(GMINT slot, GMINT channel, GMINT index, GMINT red, GMINT green, GMINT blue)
{
	//Check if initialized
	if (initialized == GMFALSE) { return GMFALSE; }

	// Round...
	int slotReal = (int)floor(slot);

	// Check if device exists
	if (deviceValid[(int)slotReal])
	{
		// Set the color on device
		if (blinkstick_set_color(deviceArray[(int)slotReal], (int)channel, (int)index, (int)red, (int)green, (int)blue))
		{
			return GMTRUE;
		}
		else
		{
			return GMFALSE;
		}
	}
	return GMTRUE;
}

/**
 * Get the red color from a device index LED.
 * 
 * \param slot the device slot
 * \param index the index of the LED
 * \return red color value
 */
gmx GMINT getDeviceRed(GMINT slot, GMINT index)
{
	GMINT colorVal = DEVICE_INVALID;
	//Check if initialized
	if (initialized == GMFALSE) { return DEVICE_INVALID; }

	// Check if device exists
	if (deviceValid[(int)slot])
	{
		blinkstick_color* col = blinkstick_get_color(deviceArray[(int)slot], (int)index);
		// typecast
		colorVal = (GMINT)(col->red);
		// free the struct
		free(col);
	}

	return colorVal;
}

/**
 * Get the green color from a device index LED.
 *
 * \param slot the device slot
 * \param index the index of the LED
 * \return green color value
 */
gmx GMINT getDeviceGreen(GMINT slot, GMINT index)
{
	GMINT colorVal = DEVICE_INVALID;
	//Check if initialized
	if (initialized == GMFALSE) { return DEVICE_INVALID; }

	// Check if device exists
	if (deviceValid[(int)slot])
	{
		blinkstick_color* col = blinkstick_get_color(deviceArray[(int)slot], (int)index);
		// typecast
		colorVal = (GMINT)(col->green);
		// free the struct
		free(col);
	}

	return colorVal;
}

/**
 * Get the blue color from a device index LED.
 *
 * \param slot the device slot
 * \param index the index of the LED
 * \return blue color value
 */
gmx GMINT getDeviceBlue(GMINT slot, GMINT index)
{
	GMINT colorVal = DEVICE_INVALID;
	//Check if initialized
	if (initialized == GMFALSE) { return DEVICE_INVALID; }

	// Check if device exists
	if (deviceValid[(int)slot])
	{
		blinkstick_color* col = blinkstick_get_color(deviceArray[(int)slot], (int)index);
		// typecast
		colorVal = (GMINT)(col->blue);
		// free the struct
		free(col);
	}

	return colorVal;
}

gmx GMINT resetLED(GMINT device, GMINT channel, GMINT index)
{
	//Check if initialized
	if (initialized == GMFALSE) { return DEVICE_INVALID; }

	// Check if device exists
	if (deviceValid[(int)device])
	{
		blinkstick_off(deviceArray[(int)device], (int)channel, (int)index);
		return GMTRUE;
	}
	return GMFALSE;
	
}

