#ifndef WIFIPROP2CONNECTION_H
#define WIFIPROP2CONNECTION_H

#include <string>
#include <list>
#include "propconnection.h"
#include "sock.h"
#include "wifiinfo.h"

#define WIFI_REQUIRED_MAJOR_VERSION         "v1."
#define WIFI_REQUIRED_MAJOR_VERSION_LEGACY  "02-"

// timeout used when making an HTTP request or connecting a telnet session
#define CONNECT_TIMEOUT             3000
#define RESPONSE_TIMEOUT            3000
#define DISCOVER_REPLY_TIMEOUT      250
#define DISCOVER_ATTEMPTS           3

class WiFiProp2Connection : public PropConnection
{
public:
    WiFiProp2Connection();
    ~WiFiProp2Connection();
    int setAddress(const char *ipaddr);
    int getVersion();
    int checkVersion();
    int checkChipVersion();
    const char *version() { return m_version ? m_version : "(unknown)"; }
    bool isOpen();
    int close();
    int connect();
    int disconnect();
    int sendDownloadHeader();
    int sendDownloadDataHex(const uint8_t *image, int imageSize);
    int sendDownloadDataTxt(const uint8_t *image, int imageSize);
    int setName(const char *name);
    int setResetMethod(const char *method);
    int generateResetSignal();
    int identify(int *pVersion);
    int loadImage(const uint8_t *image, int imageSize, uint8_t *response, int responseSize);
    int loadImage(const uint8_t *image, int imageSize, LoadType loadType = ltDownloadAndRun, int info = false);
    int sendData(const uint8_t *buf, int len);
    int receiveData(const uint8_t *buf, int len);
    int receiveDataTimeout(uint8_t *buf, int len, int timeout);
    int receiveDataExactTimeout(uint8_t *buf, int len, int timeout);
    int setBaudRate(int baudRate);
    int maxDataSize() { return 1024; }
    int terminal(bool checkForExit, bool pstMode);
    static int findModules(bool show, WiFiInfoList &list, int count = -1);
private:
    int sendRequest(uint8_t *req, int reqSize, uint8_t *res, int resMax, int *pResult);
    static uint8_t *getBody(uint8_t *msg, int msgSize, int *pBodySize);
    static void dumpHdr(const uint8_t *buf, int size);
    static void dumpResponse(const uint8_t *buf, int size);
    char *m_ipaddr;
    char *m_version;
    SOCKADDR_IN m_httpAddr;
    SOCKADDR_IN m_telnetAddr;
    SOCKET m_telnetSocket;
    int m_resetPin;
};

#endif // WIFIPROP2CONNECTION_H
