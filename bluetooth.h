#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <NimBLEDevice.h>

// UUIDs for the service and characteristics
#define SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_RX_UUID "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_TX_UUID "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

// Forward declarations of functions
void setupBluetooth();
void notifyBluetooth(const char *data);
bool isConnected();

extern bool deviceConnected;  // Track the connection status

#endif // BLUETOOTH_H
