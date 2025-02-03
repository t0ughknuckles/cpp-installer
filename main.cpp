#include <iostream>
#include <string>
#include "Web.hpp"
#include <windows.h>
#include <urlmon.h>
#include <thread>
#pragma comment(lib, "urlmon.lib")

const std::string DL1 = DownloadURL("file1 link");
const std::string DL2 = DownloadURL("file2 link");

const std::wstring file1 = L".\\Folder\\file1.exe";
const std::wstring file2 = L".\\Folder\\file2.dll";

const wchar_t* folder = L".\\Folder";

int main() 
{
    SetConsoleTitleA("Installer");

    HRESULT hr{};
    std::wstring wDL1 = StringToWString(DL1);
    std::wstring wDL2 = StringToWString(DL2);

    DWORD fileType = GetFileAttributes(folder);
    if (fileType == INVALID_FILE_ATTRIBUTES) 
    {
        std::cout << "Creating directory... ";

        if (CreateDirectory(folder, 0))
            std::cout << "Success." << std::endl;
        else 
            std::cout << "Failed: " << GetLastError() << std::endl;
    }
    else 
        std::cout << "Directory exists, downloading latest files" << std::endl;

    std::cout << "[1/2] Downloading files... ";

    if (DownloadFile(hr, wDL1.c_str(), file1) == true)
        std::cout << "Success." << std::endl;
    else
        std::cout << "Failed: " << hr << std::endl;

    std::cout << "[2/2] Downloading files... ";

    if(DownloadFile(hr, wDL2.c_str(), file2) == true)
        std::cout << "Success." << std::endl;
    else
        std::cout << "Failed: " << hr << std::endl;
    
    std::cout << "Opening directory...";
    
    try { system("start .\\UI"); }
    catch (std::exception) { std::cout << "Failed." << std::endl; }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    return 0;
}
