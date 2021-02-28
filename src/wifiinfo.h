#ifndef WIFIINFO_H
#define WIFIINFO_H

#include <string>
#include <list>

class WiFiInfo {
public:
    WiFiInfo() {}
    WiFiInfo(std::string name, std::string address) : m_name(name), m_address(address) {}
    const char *name() { return m_name.c_str(); }
    const char *address() { return m_address.c_str(); }
private:
    std::string m_name;
    std::string m_address;
};

typedef std::list<WiFiInfo> WiFiInfoList;

#endif // WIFIINFO_H