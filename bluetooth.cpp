#include "bluetooth.h"
#include <Arduino.h>
#include <NimBLEDevice.h>

bool deviceConnected = false;  // Flag to track Bluetooth device connection

// Callback for when the device connects to the Bluetooth server
class ServerCallbacks : public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) override {
        deviceConnected = true;
        Serial.println("Device connected");
    };
    void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override {
        deviceConnected = false;
        Serial.println("Device disconnected");
        NimBLEDevice::startAdvertising();  // Restart advertising
    }
};

// Set up the Bluetooth service and characteristics
void setupBluetooth() {
    Serial.println("Setting up Bluetooth...");

    // Initialize NimBLE
    NimBLEDevice::init("ESP32C3_BLE_UART");

    // Create the server
    NimBLEServer *pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());

    // Create the service
    NimBLEService *pService = pServer->createService(SERVICE_UUID);

    // Create the Tx and Rx characteristics
    NimBLECharacteristic *pTxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_TX_UUID, NIMBLE_PROPERTY::NOTIFY
    );
    NimBLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_RX_UUID, NIMBLE_PROPERTY::WRITE
    );

    // Start the service
    pService->start();

    // Start advertising the Bluetooth service
    NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->start();

    Serial.println("Bluetooth Initialized. Waiting for client connection...");
}

bool isConnected() {
    if(deviceConnected) return true;
    return false;
}

// Notify connected Bluetooth device with the given data
void notifyBluetooth(const char *data) {
    NimBLECharacteristic *pTxCharacteristic = NimBLEDevice::getServer()->getServiceByUUID(SERVICE_UUID)->getCharacteristic(CHARACTERISTIC_TX_UUID);
    pTxCharacteristic->setValue((uint8_t*)data, strlen(data));
    pTxCharacteristic->notify();  // Notify the connected client
    Serial.println("Sent data via Bluetooth: ");
    Serial.println(data);
}
