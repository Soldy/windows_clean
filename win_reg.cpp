#include <iostream>
#include <string>
#include <Windows.h>

class DwordAbstract {
  public:
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
    int disable() {
        this->write(this->value_disabled);
        return this->writeCheck();
    };
    int enable() {
        this->write(this->value_auto);
        return this->writeCheck();
    };
    void init(
      std::string name,
      LPCWSTR store
    ){
        this->name = name;
        this->store = store;

    }
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
            this->store(),
            0,
            KEY_READ,
            &hkey
        );
        this->read_result = RegQueryValueEx(
            hkey,
            L"Start",
            NULL,
            &this->data_type,
            (BYTE *)& this->value,
            &this->data_size
        );
        RegCloseKey(hkey);
    };
    void write(DWORD nbc) {
        HKEY hkey;
        this->write_open_result = RegOpenKeyEx(
            HKEY_LOCAL_MACHINE,
            this->store(),
            0,
            KEY_WRITE,
            &hkey
        );
        this->write_result = RegSetValueEx(
            hkey,
            L"Start",
            0,
            REG_DWORD,
            (BYTE *)&nbc,
            sizeof(DWORD)
        );
        RegCloseKey(hkey);

    };
    int writeCheck(){
        if (this->write_open_result != ERROR_SUCCESS)
            return -1;
        if (this->write_result == ERROR_SUCCESS)
            return 0;
        return -2;
    }
};



class DnsCacheClass : public DwordAbstract {
  private:
    DnsCacheClass(){
      this->init("DnsCache", L"SYSTEM\\CurrentControlSet\\services\\Dnscache");
    }
};

int main(int argc, char* argv[]){
    DnsCacheClass * dS = new DnsCacheCLass();
    std::cout << "ds - " << dS->check() << std::endl;
    std::cout << dS->enable() << std::endl;
    std::cout << "ds - " << dS->check() << std::endl;
    return 0;
}
