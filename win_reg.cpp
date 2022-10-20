#include <iostream>
#include <string>
#include <Windows.h>

class dwordStart {
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
        std::cout << "ds - " << this->value << std::endl;
        return 0;
    };
    int disable() {
        this->write(this->value_disabled);
        if (this->write_open_result != ERROR_SUCCESS)
            return -1;
        if (this->write_result == ERROR_SUCCESS)
            return 0;
        return -2;
    };
    int enable() {
        this->write(this->value_auto);
        if (this->write_open_result != ERROR_SUCCESS)
            return -1;
        if (this->write_result == ERROR_SUCCESS)
            return 0;
        return -2;
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
    LPCWSTR store = L"SYSTEM\\CurrentControlSet\\services\\Dnscache";

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
            (BYTE *)& this->value,
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
        this->write_result = RegSetKeyValue(
            hkey,
            L"Start",
            0,
            REG_DWORD,
            &nbc,
            sizeof(DWORD)
        );
        RegCloseKey(hkey);

    };
};


int main(int argc, char* argv[]){
    dwordStart* dS = new dwordStart();
    std::cout << "ds - " << dS->check() << std::endl;
    std::cout << dS->enable() << std::endl;
    std::cout << "ds - " << dS->check() << std::endl;
    return 0;
}