#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
 
class ServiceDword {
  public:
    ServiceDword(
        std::string name,
        LPCWSTR store
    ) {
        this->name = name;
        this->store = store;
    };
    int check() {
        this->read();
        if (this->read_open_result != ERROR_SUCCESS)
            return -1;
        if (this->read_result != ERROR_SUCCESS)
            return -2;
        if (this->value == this->value_disabled)
            return 4;
        if (this->value == this->value_manual)
            return 3;
        if (this->value == this->value_delayed)
            return 2;
        if (this->value == this->value_auto)
            return 1;
        return 0;
    };
    std::string getName() {
        return this->name;
    };
    int disable() {
        this->write(this->value_disabled);
        return this->writeCheck();
    };
    int enable() {
        this->write(this->value_auto);
        return this->writeCheck();
    };
  private:
    long read_open_result;
    long read_result;
    long write_open_result;
    long write_result;
    DWORD data_type;
    DWORD value = 0;
    DWORD data_size = sizeof(this->value);
    DWORD value_auto = 1;
    DWORD value_delayed = 2;
    DWORD value_manual = 3;
    DWORD value_disabled = 4;
    std::string name;
    LPCWSTR store;
    void read() {
        HKEY hkey;
        this->read_open_result = RegOpenKeyEx(
            HKEY_LOCAL_MACHINE,
            this->store,
            0,
            KEY_READ,
            &hkey
        );
        this->read_result = RegQueryValueEx(
            hkey,
            L"Start",
            NULL,
            &this->data_type,
            (BYTE*)&this->value,
            &this->data_size
        );
        RegCloseKey(hkey);
    };
    void write(DWORD nbc) {
        HKEY hkey;
        this->write_open_result = RegOpenKeyEx(
            HKEY_LOCAL_MACHINE,
            this->store,
            0,
            KEY_WRITE,
            &hkey
        );
        this->write_result = RegSetValueEx(
            hkey,
            L"Start",
            0,
            REG_DWORD,
            (BYTE*)&nbc,
            sizeof(DWORD)
        );
        RegCloseKey(hkey);
 
    };
    int writeCheck() {
        if (this->write_open_result != ERROR_SUCCESS)
            return -1;
        if (this->write_result == ERROR_SUCCESS)
            return 0;
        return -2;
    }
};
 
 
 
 
int main(int argc, char* argv[]) {
    std::vector<ServiceDword *> regs;
    regs.push_back(new ServiceDword("DnsCache", L"SYSTEM\\CurrentControlSet\\services\\Dnscache"));
    regs.push_back(new ServiceDword("Asus App Service", L"SYSTEM\\CurrentControlSet\\Services\\AsusAppService"));
    regs.push_back(new ServiceDword("Asus Link Near", L"SYSTEM\\CurrentControlSet\\Services\\ASUSLinkNear"));
    regs.push_back(new ServiceDword("Asus Link Remote", L"SYSTEM\\CurrentControlSet\\Services\\ASUSLinkRemote"));
    regs.push_back(new ServiceDword("Asus Optimalization", L"SYSTEM\\CurrentControlSet\\Services\\ASUSOptimization"));
    regs.push_back(new ServiceDword("Asus PTP Driver", L"SYSTEM\\CurrentControlSet\\Services\\AsusPTPDrv"));
    regs.push_back(new ServiceDword("Asus SAIO", L"SYSTEM\\CurrentControlSet\\Services\\AsusSAIO"));
    regs.push_back(new ServiceDword("Asus Software Manager", L"SYSTEM\\CurrentControlSet\\Services\\ASUSSoftwareManager"));
    regs.push_back(new ServiceDword("Asus Swutch", L"SYSTEM\\CurrentControlSet\\Services\\ASUSSwitch"));
    regs.push_back(new ServiceDword("Asus System Analysis", L"SYSTEM\\CurrentControlSet\\Services\\ASUSSystemAnalysis"));
    regs.push_back(new ServiceDword("Asus System Diagnosis", L"SYSTEM\\CurrentControlSet\\Services\\ASUSSystemDiagnosis"));
    regs.push_back(new ServiceDword("Modem", L"SYSTEM\\CurrentControlSet\\Services\\Modem"));
    regs.push_back(new ServiceDword("Natural Authentication", L"SYSTEM\\CurrentControlSet\\Services\\NaturalAuthentication"));
    regs.push_back(new ServiceDword("Telemetry", L"SYSTEM\\CurrentControlSet\\Services\\Telemetry"));
    regs.push_back(new ServiceDword("xblAuth Manager", L"SYSTEM\\CurrentControlSet\\Services\\XblAuthManager"));
    regs.push_back(new ServiceDword("xbl game save", L"SYSTEM\\CurrentControlSet\\Services\\XblGameSave"));
    regs.push_back(new ServiceDword("xboxgip", L"SYSTEM\\CurrentControlSet\\Services\\xboxgip"));
    regs.push_back(new ServiceDword("XboxGipSvc", L"SYSTEM\\CurrentControlSet\\Services\\XboxGipSvc"));
    regs.push_back(new ServiceDword("XboxNetApiSvc", L"SYSTEM\\CurrentControlSet\\Services\\XboxNetApiSvc"));
    for (auto i : regs)
        std::cout << i->getName() << " : " << i->check() << std::endl;
    for (auto i : regs)
        std::cout << i->getName() << " : " << i->disable() << std::endl;
    for (auto i : regs)
        std::cout << i->getName() << " : " << i->check() << std::endl;
    return 0;
}
