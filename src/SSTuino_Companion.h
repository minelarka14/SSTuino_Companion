/******************************************************************************
 *                                                                            *
 * NAME: SSTuino_Companion.h                                                  *
 *                                                                            *
 * PURPOSE: Main header file for the Arduino library                          *
 *                                                                            *
 * GLOBAL VARIABLES:                                                          *
 *                                                                            *
 * Variable Type Description                                                  *
 * -------- ---- -----------                                                  *
 *                                                                            *
 *****************************************************************************/

#ifndef __SSTuino_Companion__
#define __SSTuino_Companion__

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <SoftwareSerial.h>

/*
 * Enumerations and structs
 */

struct ReturnedData {
    int8_t linkID = -1;
    String content = "";
};

enum Status {
    UNRESPONSIVE = -1,
    SUCCESSFUL = 0,
    UNSUCCESSFUL,
    IN_PROGRESS,
    NOT_ATTEMPTED
};

enum HTTP_Operation {
    GET = 'G',
    POST = 'P'
};

enum HTTP_Content {
    CONTENT,
    HEADERS
};

enum FLOWCTRL_TYPE {
    FLOWCTRL_TYPE1 = 0, // Type 1 flow control for large data chunks like printing out a HTTP reply
    FLOWCTRL_TYPE2 = 1  // Type 2 flow control for fast-response replies like getting HTTP status
};

/*
 * Class declaration
 */

class SSTuino {
public:
    SSTuino();
    // void rawInput(String input);
    void openLink();
    void slowOpenLink(int delayTime=5000);

    // Basic functionality
    bool smokeTest();
    bool verifyVersion();
    void reset();

    // Wi-fi functionality
    String getWifiHotspots();
    bool wifiInRange(const String& ssid);
    void connectToWifi(const String& ssid, const String& password);
    Status getWifiStatus();
    void disconnectWifi();

    // Network functionality
    String getIP();

    // HTTP operations
    int setupHTTP(HTTP_Operation op, const String& url);
    bool setHTTPPOSTParameters(int handle, const String& data);
    bool setHTTPHeaders(int handle, const String& data);
    bool transmitHTTP(int handle);

    Status getHTTPProgress(int handle);

    int getHTTPStatusCode(int handle);
    String getHTTPReply(int handle, HTTP_Content field, bool deleteReply);
    bool deleteHTTPReply(int handle);

    // MQTT operations
    bool enableMQTT(const String& server, bool useSecure);
    bool enableMQTT(const String& server, bool useSecure, const String& username, const String& password);
    bool disableMQTT();
    bool isMQTTConnected();
    bool mqttPublish(const String& topic, const String& content);
    bool mqttSubscribe(const String& topic);
    bool mqttUnsubscribe(const String& topic);
    bool mqttNewDataArrived(const String& topic);
    String mqttGetSubcriptionData(const String& topic);

    // MQTT helpers/wrappers
    void mqttPollNewData(bool *newDataArrived, const String& topic, unsigned long delay);
//     int16_t beginDeepSleep(uint16_t sleepTime, bool blocking);

//     int16_t setDHCPEnabled(bool enabled);
//     int16_t setStationName(String& name);
//     int16_t setSoftAPSettings(String& ssid, String& password, uint8_t channel, EncryptionMethod encryptionMethod);
//     int16_t setIP(bool permanent, String ip, String gateway="", String netmask="");
    
private:
    SoftwareSerial _ESP01UART;
    unsigned long previousMillis;
    void writeCommandFromPROGMEM(const char* text, int buffersize=8);
    int16_t waitNoOutput(char* values, uint16_t timeOut);
    void rx_empty(void);
    bool recvFind(String target, uint32_t timeout, uint8_t reserve=8);
    String controlledRecvString(uint32_t timeout, FLOWCTRL_TYPE flowControlType, uint8_t reserve=8);
    String recvString(String target, uint32_t timeout, uint8_t reserve=8);
    int16_t wait(const char* values, uint16_t timeOut);
    int16_t waitXON(const char* values, uint16_t timeOut, FLOWCTRL_TYPE flowControlType);
    // bool debug;
};


#endif  // End of __SSTuino_Companion__ definition check
