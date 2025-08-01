#include "logManager.h"

// --- Module-internal variables ---
static LogEntry logBuffer[MAX_LOGS_IN_BUFFER];
static LastState stateMap[MAX_STATES];
static uint32_t logBufferCount = 0;
static uint32_t stateCount = 0;
static unsigned long lastFlushTime = 0;
static unsigned long lastCleanupTime = 0;
static bool isCleaning = false; // Mutex lock for cleanup process

const char *LOG_FILE_PATH = "/logs.bin";
const char *TEMP_LOG_FILE_PATH = "/logs_temp.bin";
const size_t NUM_CATEGORIES = sizeof(CATEGORY_DEFINITIONS) / sizeof(CategoryDefinition);
const size_t NUM_TYPES = sizeof(TYPES) / sizeof(char *);

// --- Helper functions (Now fully dynamic) ---

// Converts a string to its corresponding code by searching in an array.
uint8_t getCodeFromString(const char *str, const char *const arr[], size_t arr_size)
{
    if (!str)
        return 255; // Not found
    for (uint8_t i = 0; i < arr_size; i++)
    {
        if (strcmp(str, arr[i]) == 0)
            return i;
    }
    return 255; // Not found
}

// Converts a code back to its string representation.
const char *getStringFromCode(uint8_t code, const char *const arr[], size_t arr_size)
{
    if (code >= arr_size)
        return "Unknown";
    return arr[code];
}

uint8_t getCategoryCode(const char *category)
{
    if (!category)
        return 255;
    for (uint8_t i = 0; i < NUM_CATEGORIES; i++)
    {
        if (strcmp(category, CATEGORY_DEFINITIONS[i].name) == 0)
            return i;
    }
    return 255;
}

uint8_t getLabelCode(uint8_t categoryCode, const char *label)
{
    if (categoryCode >= NUM_CATEGORIES)
        return 255;
    // Directly uses the definition array, no switch-case needed.
    return getCodeFromString(label, CATEGORY_DEFINITIONS[categoryCode].labels, CATEGORY_DEFINITIONS[categoryCode].label_count);
}

uint8_t getTypeCode(const char *type)
{
    return getCodeFromString(type, TYPES, NUM_TYPES);
}

// --- Core Functions ---

void flushLogsToLittleFS();

void initLogManager()
{
    if (!psramFound())
    {
        Serial.println("[Log][ERROR] PSRAM not found! LogManager disabled.");
        return;
    }
    if (!LittleFS.begin())
    {
        Serial.println("[Log][ERROR] LittleFS mount failed!");
        return;
    }
    Serial.println("[Log][INFO] LogManager initialized");
}

void setLogMessage(const char *category, const char *label, const char *type, const char *message)
{
    uint8_t categoryCode = getCategoryCode(category);
    uint8_t typeCode = getTypeCode(type);
    if (categoryCode == 255 || typeCode == 255)
        return; // Invalid category or type
    uint8_t labelCode = getLabelCode(categoryCode, label);
    if (labelCode == 255)
        return; // Invalid label

    // Deduplication Logic: Check against the PSRAM state cache.
    for (uint32_t i = 0; i < stateCount; ++i)
    {
        if (stateMap[i].category == categoryCode && stateMap[i].label == labelCode)
        {
            // Found the state. Check if the message is identical.
            if (stateMap[i].type == typeCode && strncmp(stateMap[i].message, message, 15) == 0)
            {
                return; // Identical message, do nothing.
            }
            // Message is different, update the state and proceed to log it.
            stateMap[i].type = typeCode;
            strncpy(stateMap[i].message, message, 15);
            stateMap[i].message[15] = '\0'; // Ensure null termination
            goto AddLog;
        }
    }

    // State not found, add a new one if space is available.
    if (stateCount < MAX_STATES)
    {
        stateMap[stateCount].category = categoryCode;
        stateMap[stateCount].label = labelCode;
        stateMap[stateCount].type = typeCode;
        strncpy(stateMap[stateCount].message, message, 15);
        stateMap[stateCount].message[15] = '\0';
        stateCount++;
    }

AddLog:
    // Add the new log to the buffer, flush if the buffer is full.
    if (logBufferCount >= MAX_LOGS_IN_BUFFER)
    {
        flushLogsToLittleFS();
    }
    logBuffer[logBufferCount].timestamp = millis();
    logBuffer[logBufferCount].category = categoryCode;
    logBuffer[logBufferCount].label = labelCode;
    logBuffer[logBufferCount].type = typeCode;
    strncpy(logBuffer[logBufferCount].message, message, 15);
    logBuffer[logBufferCount].message[15] = '\0';
    logBufferCount++;
}

void flushLogsToLittleFS()
{
    while (isCleaning)
    {
        delay(10);
    } // Wait if a cleanup is in progress.
    if (logBufferCount == 0)
        return;

    File file = LittleFS.open(LOG_FILE_PATH, "a"); // Open in append mode
    if (!file)
    {
        // Keep logs in buffer for the next attempt if file opening fails.
        return;
    }
    file.write((const uint8_t *)logBuffer, logBufferCount * sizeof(LogEntry));
    file.close();
    logBufferCount = 0; // Reset buffer
    lastFlushTime = millis();
}

void deleteOldLogs()
{
    isCleaning = true; // Set lock
    File file = LittleFS.open(LOG_FILE_PATH, "r");
    if (!file || file.size() == 0)
    {
        isCleaning = false;
        return;
    }

    // If file size exceeds the max limit, delete it completely.
    if (file.size() > MAX_FILE_SIZE)
    {
        file.close();
        LittleFS.remove(LOG_FILE_PATH);
        isCleaning = false;
        return;
    }

    // Optimization: check the first log's timestamp to see if any cleanup is needed.
    LogEntry firstEntry;
    file.read((uint8_t *)&firstEntry, sizeof(LogEntry));
    uint32_t currentTime = millis();

    // This subtraction method is robust against millis() rollover.
    if (currentTime - firstEntry.timestamp < MAX_LOG_AGE)
    {
        file.close();
        isCleaning = false; // No old logs found, no need to proceed.
        return;
    }
    file.seek(0); // Rewind file to the beginning.

    File tempFile = LittleFS.open(TEMP_LOG_FILE_PATH, "w");
    if (!tempFile)
    {
        file.close();
        isCleaning = false;
        return;
    }

    LogEntry entry;
    // Iterate through the file and write only non-expired logs to the temp file.
    while (file.read((uint8_t *)&entry, sizeof(LogEntry)) > 0)
    {
        if (currentTime - entry.timestamp < MAX_LOG_AGE)
        {
            tempFile.write((const uint8_t *)&entry, sizeof(LogEntry));
        }
    }
    file.close();
    tempFile.close();

    // Safely replace the old log file with the new, cleaned one.
    LittleFS.remove(LOG_FILE_PATH);
    LittleFS.rename(TEMP_LOG_FILE_PATH, LOG_FILE_PATH);
    isCleaning = false; // Release lock
}

void logManagerLoop()
{
    unsigned long currentMillis = millis();
    // These time checks are robust against millis() rollover.
    if (currentMillis - lastFlushTime >= LOG_FLUSH_INTERVAL)
    {
        flushLogsToLittleFS();
    }
    if (currentMillis - lastCleanupTime >= LOG_CLEANUP_INTERVAL)
    {
        deleteOldLogs();
        lastCleanupTime = currentMillis;
    }
}

std::vector<DecodedLogEntry> getLogMessages(const char *categoryFilter, const char *labelFilter, const char *typeFilter, uint16_t count)
{
    std::vector<DecodedLogEntry> results;
    File file = LittleFS.open(LOG_FILE_PATH, "r");
    if (!file || file.size() == 0)
    {
        return results;
    }

    uint8_t categoryCodeFilter = getCategoryCode(categoryFilter);
    uint8_t typeCodeFilter = getTypeCode(typeFilter);

    long fileSize = file.size();
    long totalRecords = fileSize / sizeof(LogEntry);
    long recordsToRead = (count > totalRecords) ? totalRecords : count;

    // Read the file backwards for efficiency.
    for (long i = 0; i < recordsToRead; ++i)
    {
        file.seek(fileSize - (i + 1) * sizeof(LogEntry));
        LogEntry entry;
        file.read((uint8_t *)&entry, sizeof(LogEntry));

        // Apply filters
        bool match = true;
        if (categoryFilter && entry.category != categoryCodeFilter)
            match = false;
        if (typeFilter && entry.type != typeCodeFilter)
            match = false;
        if (labelFilter)
        {
            if (entry.category >= NUM_CATEGORIES)
            {
                match = false;
            }
            else
            {
                uint8_t labelCodeFilter = getLabelCode(entry.category, labelFilter);
                if (entry.label != labelCodeFilter)
                    match = false;
            }
        }

        if (match)
        {
            DecodedLogEntry decoded;
            decoded.timestamp = entry.timestamp;
            if (entry.category < NUM_CATEGORIES)
            {
                const auto &def = CATEGORY_DEFINITIONS[entry.category];
                decoded.category = def.name;
                decoded.label = getStringFromCode(entry.label, def.labels, def.label_count);
            }
            else
            {
                decoded.category = "Unknown";
                decoded.label = "Unknown";
            }
            decoded.type = getStringFromCode(entry.type, TYPES, NUM_TYPES);
            decoded.message = String(entry.message);
            results.push_back(decoded);
        }
    }
    file.close();
    // Results are in reverse-chronological order (newest first).
    return results;
}

void printLogEntry(const DecodedLogEntry &entry)
{
    Serial.printf("Log: time=%lu, cat=%s, lbl=%s, typ=%s, msg=%s\n",
                  entry.timestamp, entry.category.c_str(), entry.label.c_str(),
                  entry.type.c_str(), entry.message.c_str());
}