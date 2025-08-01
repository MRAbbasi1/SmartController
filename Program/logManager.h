#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <LittleFS.h>
#include <vector>

//====================================================================================//
//               CONFIGURATION SECTION FOR CATEGORIES & LABELS                        //
//          *** Edit only this section to add or change items *** //
//====================================================================================//

// Step 1: Define the label arrays for each category.
const char *const SYSTEM_LABELS[] = {"Time", "Config", "Boot"};
const char *const SENSOR_LABELS[] = {"Temp", "Humidity"};
const char *const RELAY_LABELS[] = {"Compressor", "Evaporator"};
const char *const CONTROL_LABELS[] = {"Fan", "Mode"};
// If you have a new category, add its label array here:
// const char* const NEW_CATEGORY_LABELS[] = {"Label1", "Label2"};

// Step 2: Define the main structure for categories.
// Each category has a name, a pointer to its label array, and the count of its labels.
struct CategoryDefinition
{
    const char *name;
    const char *const *labels;
    const size_t label_count;
};

// Step 3: Define your final categories in this array.
// The code for each category is its index in this array (System=0, Sensor=1, ...).
const CategoryDefinition CATEGORY_DEFINITIONS[] = {
    {"System", SYSTEM_LABELS, sizeof(SYSTEM_LABELS) / sizeof(char *)},
    {"Sensor", SENSOR_LABELS, sizeof(SENSOR_LABELS) / sizeof(char *)},
    {"Relay", RELAY_LABELS, sizeof(RELAY_LABELS) / sizeof(char *)},
    {"Control", CONTROL_LABELS, sizeof(CONTROL_LABELS) / sizeof(char *)},
    // To add a new category, add a new line here:
    // {"NewCategory", NEW_CATEGORY_LABELS, sizeof(NEW_CATEGORY_LABELS) / sizeof(char*)}
};

const char *const TYPES[] = {"INFO", "ERROR", "WARNING"};

//====================================================================================//
//              END OF CONFIGURATION SECTION - DO NOT EDIT BELOW THIS LINE            //
//====================================================================================//

#define MAX_LOGS_IN_BUFFER 500
#define MAX_STATES 100
#define LOG_FLUSH_INTERVAL 600000
#define LOG_CLEANUP_INTERVAL 3600000
#define MAX_LOG_AGE 604800000
#define MAX_FILE_SIZE 1048576

// Binary structure for log entries stored in the file (23 bytes)
struct LogEntry
{
    uint32_t timestamp; // 4 bytes: Timestamp (ms from boot)
    uint8_t category;   // 1 byte: Category code
    uint8_t label;      // 1 byte: Label code
    uint8_t type;       // 1 byte: Type code
    char message[16];   // 16 bytes: Short message
};

// Structure for the deduplication cache in PSRAM
struct LastState
{
    uint8_t category;
    uint8_t label;
    uint8_t type;
    char message[16];
};

// Structure for returning human-readable log data
struct DecodedLogEntry
{
    uint32_t timestamp;
    String category;
    String label;
    String type;
    String message;
};

// --- Function Declarations ---
void initLogManager();
void logManagerLoop();
void setLogMessage(const char *category, const char *label, const char *type, const char *message);
std::vector<DecodedLogEntry> getLogMessages(const char *categoryFilter, const char *labelFilter, const char *typeFilter, uint16_t count);
void printLogEntry(const DecodedLogEntry &entry);

#endif // LOG_MANAGER_H