// -----------------------------------
// Wi-Fi Management Module
// -----------------------------------

#include "wifi.h"
#include "setting.h" // For NVS settings
#include <WiFi.h>

// =====================================================================
// Internal Variables
// =====================================================================
static unsigned long lastReconnectAttempt = 0; // Stores the timestamp of the last reconnect attempt
static bool wasConnected = false;              // Tracks the previous connection state to detect changes
static bool skipMonitoringLogged = false;      // Tracks if a log has been printed when monitoring is skipped

// =====================================================================
// Helper Function: Check if a string is "0"
// =====================================================================
/**
 * Checks whether the provided string is set to "0".
 *
 * This is primarily used to verify if stored credentials (e.g., SSID or password)
 * have been cleared or are invalid.
 *
 * @param str Pointer to the string to check.
 * @return `true` if the string is "0", otherwise `false`.
 */
bool isZeroString(const char *str)
{
    return (strcmp(str, "0") == 0);
}

// =====================================================================
// Wi-Fi Setup Function
// =====================================================================
/**
 * Initializes and connects the ESP32 to a Wi-Fi network based on stored credentials.
 *
 * Steps:
 * 1. Retrieve stored SSID and password from NVS.
 * 2. Validate the credentials (ensure they are not set to "0").
 * 3. Scan for available networks to confirm that the target SSID is within range.
 * 4. Attempt to connect to the specified Wi-Fi network.
 *
 * Notes:
 * - Adds a small delay to ensure NVS and Wi-Fi modules are initialized properly.
 * - Skips connection attempts if credentials are invalid or the target SSID is not found.
 */
void wifiSetup()
{

    Serial.println("🛜 [WiFi] Starting Wi-Fi setup 🔁");

    // Add a delay to ensure NVS is properly initialized
    // delay(100);

    // Step 1: Retrieve SSID and Password from NVS
    char ssid[STRING_MAX_LENGTH];
    char password[STRING_MAX_LENGTH];

    if (!getStringSetting(WIFI_SSID, ssid, STRING_MAX_LENGTH) ||
        !getStringSetting(WIFI_PASSWORD, password, STRING_MAX_LENGTH))
    {

        Serial.println("🛜 [WiFi] Failed to retrieve credentials from NVS ❌");

        return;
    }

    // Step 2: Validate credentials
    if (isZeroString(ssid) || isZeroString(password))
    {

        Serial.println("🛜 [WiFi] Credentials set to '0'. Skipping Wi-Fi setup ❌");

        return;
    }

    // Step 3: Initialize Wi-Fi
    WiFi.disconnect(true); // Reset any previous Wi-Fi configuration
    WiFi.mode(WIFI_STA);   // Set Wi-Fi to station mode

    Serial.println("🔍🔍 [WiFi] Scanning for available networks...");

    delay(500); // Allow some time before scanning

    // Step 4: Scan for target SSID
    int networksFound = WiFi.scanNetworks();
    bool ssidFound = false;
    for (int i = 0; i < networksFound; i++)
    {
        if (WiFi.SSID(i).equals(ssid))
        {
            ssidFound = true;
            break;
        }
    }

    if (!ssidFound)
    {

        Serial.println("🛜 [WiFi] Target SSID not found. Skipping connection attempt ❌");

        return;
    }

    // Step 5: Attempt connection to Wi-Fi

    Serial.print("🔁 [WiFi] Connecting to SSID: ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 15000)
    {
        delay(500);
        Serial.print("...");
    }

    if (WiFi.status() == WL_CONNECTED)
    {

        Serial.println("\n🛜 [WiFi] Connected successfully ✅");
        Serial.print("🛜 [WiFi] IP Address: ");
        Serial.println(WiFi.localIP());
    }
    else
    {

        Serial.println("\n🛜 [WiFi] Failed to connect. Please check credentials ❌");
    }

    Serial.println("🛜 [WiFi] Wi-Fi setup complete ✅");
}

// =====================================================================
// Wi-Fi Loop Function with Proper Temp Flag Handling
// =====================================================================
/**
 * Periodically checks the Wi-Fi connection status and attempts to reconnect if disconnected.
 * Uses Temporary flags to control NVS reads.
 */
void wifiLoop()
{
    static unsigned long lastCheckTime = 0;        // Tracks the last time the loop was executed
    const unsigned long CHECK_INTERVAL = 120000;   // Interval for checking (in seconds)
    static char ssid[STRING_MAX_LENGTH] = {0};     // Cached SSID
    static char password[STRING_MAX_LENGTH] = {0}; // Cached Password

    // Ensure the loop runs only every CHECK_INTERVAL
    if (millis() - lastCheckTime < CHECK_INTERVAL)
        return;

    lastCheckTime = millis(); // Update the time of the last check

    // ---------------------------------------------------
    // Step 1: Check Wi-Fi Connection Status
    // ---------------------------------------------------
    if (WiFi.status() == WL_CONNECTED)
    {
        if (!wasConnected) // Detect a new connection
        {

            Serial.println("🛜 [WiFi] Connected to network ✅");
            Serial.print("🛜 [WiFi] IP Address: ");
            Serial.println(WiFi.localIP());

            wasConnected = true; // Update connection state
        }
        return; // Exit if already connected
    }

    // ---------------------------------------------------
    // Step 2: If Disconnected, Check Temporary Flags
    // ---------------------------------------------------
    wasConnected = false;

    Serial.println("🛜 [WiFi] Disconnected. Checking NVS and attempting reconnection... ❗️");

    // Check Temporary Flags to Decide NVS Access
    bool ssidFlagTemp = getChangedFlagTemp("string", WIFI_SSID);
    bool passwordFlagTemp = getChangedFlagTemp("string", WIFI_PASSWORD);

    // Reload credentials from NVS only if Temp flags are true
    if (ssidFlagTemp || passwordFlagTemp)
    {
        getStringSetting(WIFI_SSID, ssid, STRING_MAX_LENGTH);
        getStringSetting(WIFI_PASSWORD, password, STRING_MAX_LENGTH);

        // Reset Temporary Flags after reading NVS
        if (resetChangedFlagTemp("string", WIFI_SSID))

            Serial.println("🏁 [NVS] Temporary flag for SSID reset.");

        if (resetChangedFlagTemp("string", WIFI_PASSWORD))

            Serial.println("🏁 [NVS] Temporary flag for Password reset.");

        Serial.println("🛜 [WiFi] Credentials reloaded from NVS.");
    }

    // Validate credentials
    if (isZeroString(ssid) || isZeroString(password))
    {

        Serial.println("🛜 [WiFi] Invalid credentials in NVS. Skipping reconnection ❗️");

        return; // Skip the process if credentials are invalid
    }

    // ---------------------------------------------------
    // Step 3: Scan Available Networks (Non-blocking)
    // ---------------------------------------------------

    Serial.println("🔍 [WiFi] Initiating non-blocking scan for available networks...");

    // Start asynchronous scan
    if (WiFi.scanComplete() == -2)
    {
        WiFi.scanNetworks(true); // Start scan in non-blocking mode
        return;                  // Exit until the scan is complete
    }

    int networksFound = WiFi.scanComplete();
    if (networksFound == -1)
    {

        Serial.println("🔍 [WiFi] Scan in progress...");

        return; // Exit if scan is still running
    }

    bool ssidFound = false;
    for (int i = 0; i < networksFound; i++)
    {
        if (WiFi.SSID(i).equals(ssid))
        {
            ssidFound = true; // SSID found
            break;
        }
    }

    // Free memory used by the scan results
    WiFi.scanDelete();

    if (!ssidFound)
    {

        Serial.println("🛜 [WiFi] Target SSID not found. Retrying in the next cycle ❗️");

        return; // Skip connection attempt if SSID is not found
    }

    // ---------------------------------------------------
    // Step 4: Attempt to Connect to Wi-Fi
    // ---------------------------------------------------

    Serial.print("🛜 [WiFi] Attempting to connect to SSID: ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    // Wait for up to 5 seconds to establish a connection
    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 5000)
    {
        delay(500); // Short delay to avoid excessive looping
        Serial.print("...");
    }

    // ---------------------------------------------------
    // Step 5: Log Connection Status
    // ---------------------------------------------------
    if (WiFi.status() == WL_CONNECTED)
    {

        Serial.println("\n🛜 [WiFi] Connected successfully ✅");
        Serial.print("🛜 [WiFi] IP Address: ");
        Serial.println(WiFi.localIP());
    }
    else
    {

        Serial.println("\n🛜 [WiFi] Failed to connect. Retrying in the next cycle ❗️");
    }
}

// =====================================================================
// Wi-Fi Disconnect Function
// =====================================================================
/**
 * Disconnects from the current Wi-Fi network and clears stored credentials.
 *
 * This function:
 * 1. Disconnects the ESP32 from the currently connected Wi-Fi network.
 * 2. Erases Wi-Fi credentials from the chip memory.
 * 3. Updates the settings storage (e.g., NVS or JSON) to reflect cleared credentials.
 */
void wifiDisconnect()
{

    Serial.println("🛜 [WiFi] Disconnecting from Wi-Fi...🔁");

    WiFi.disconnect(true);                // Disconnect from Wi-Fi and erase credentials from flash memory
    setStringSetting(WIFI_SSID, "0");     // Clear the stored SSID
    setStringSetting(WIFI_PASSWORD, "0"); // Clear the stored password

    Serial.println("🛜 [WiFi] Disconnected and credentials cleared ❗️");
}

// =====================================================================
// Helper Function: Check Wi-Fi Connection
// =====================================================================
/**
 * Checks if the device is connected to a Wi-Fi network.
 *
 * This function:
 * 1. Verifies the Wi-Fi connection status using `WiFi.status()`.
 * 2. If connected:
 *    - Optionally copies the connected SSID to the provided buffer (`connectedSSID`).
 * 3. If not connected:
 *    - Clears the `connectedSSID` buffer (if provided) and returns `false`.
 *
 * @param connectedSSID (Optional) Pointer to a buffer for storing the SSID of the connected network.
 * @param maxLength Maximum length of the buffer provided for the SSID (including null-terminator).
 * @return `true` if connected to a Wi-Fi network, otherwise `false`.
 */
bool wifiIsConnected(char *connectedSSID = nullptr, size_t maxLength = 0)
{
    wl_status_t status = WiFi.status(); // Check Wi-Fi connection status
    if (status == WL_CONNECTED)
    {
        // If a buffer is provided, copy the SSID into it
        if (connectedSSID != nullptr && maxLength > 0)
        {
            strncpy(connectedSSID, WiFi.SSID().c_str(), maxLength - 1);
            connectedSSID[maxLength - 1] = '\0'; // Ensure null-termination
        }

        Serial.println("🛜 [WiFi] Connected to network ✅");
        Serial.printf("🛜 [WiFi] Connected SSID: %s\n", WiFi.SSID().c_str());

        return true;
    }

    // Clear the buffer if provided, since the connection is not active
    if (connectedSSID != nullptr && maxLength > 0)
    {
        connectedSSID[0] = '\0';
    }

    Serial.println("🛜 [WiFi] Not connected to any network ❗️");

    return false;
}

// =====================================================================
// Test Available Wi-Fi Networks
// =====================================================================
/**
 * Scans available Wi-Fi networks and sorts them by signal strength (RSSI).
 *
 * This function:
 * 1. Performs a scan of available Wi-Fi networks using `WiFi.scanNetworks()`.
 * 2. Retrieves the SSID and RSSI of each network.
 * 3. Sorts the networks in descending order based on RSSI (strongest signals first).
 * 4. Prints the top networks (up to `MAX_NETWORKS`) to the Serial Monitor.
 */
void scanWiFiNetworks()
{

    Serial.println("🛜 [WiFi] Starting Wi-Fi network scan...🔁");

    // Scan for available networks
    int numNetworks = WiFi.scanNetworks();
    if (numNetworks <= 0)
    {

        Serial.println("🛜 [WiFi] No networks found ❗️");

        return;
    }

    Serial.printf("🛜 [WiFi] Found %d networks.\n", numNetworks);

    // Create arrays to store SSID and RSSI
    String ssidList[numNetworks];
    int rssiList[numNetworks];

    // Collect SSID and RSSI values
    for (int i = 0; i < numNetworks; i++)
    {
        ssidList[i] = WiFi.SSID(i);
        rssiList[i] = WiFi.RSSI(i);
    }

    // Sort networks in-place by RSSI (descending order) using Bubble Sort
    for (int i = 0; i < numNetworks - 1; i++)
    {
        for (int j = 0; j < numNetworks - i - 1; j++)
        {
            if (rssiList[j] < rssiList[j + 1])
            {
                // Swap RSSI
                int tempRssi = rssiList[j];
                rssiList[j] = rssiList[j + 1];
                rssiList[j + 1] = tempRssi;

                // Swap SSID
                String tempSsid = ssidList[j];
                ssidList[j] = ssidList[j + 1];
                ssidList[j + 1] = tempSsid;
            }
        }
    }

    // Print the top networks (up to MAX_NETWORKS)
    for (int i = 0; i < min(numNetworks, MAX_NETWORKS); i++)
    {

        Serial.printf("🛜 [WiFi] SSID: %s, RSSI: %d\n", ssidList[i].c_str(), rssiList[i]);
    }

    Serial.println("🛜 [WiFi] Network scan complete ✅");
}
