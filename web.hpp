#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <wininet.h>
#include <stdexcept>
#pragma comment(lib, "wininet")

inline bool DownloadFile(HRESULT& hr, std::wstring url, std::wstring location)
{
    hr = URLDownloadToFile(0, url.c_str(), location.c_str(), 0, 0);
    return SUCCEEDED(hr);
}

inline std::wstring StringToWString(const std::string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, 0, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size_needed);
    return wstr;
}

inline std::string replaceAll(std::string subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}

std::string DownloadURL(const char* URL) {
    HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

    if (!interwebs) 
        throw std::runtime_error("Failed to open internet session: " + std::to_string(GetLastError()));

    HINTERNET urlFile = InternetOpenUrlA(interwebs, URL, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!urlFile) {
        InternetCloseHandle(interwebs);
        throw std::runtime_error("Failed to open URL: " + std::to_string(GetLastError()));
    }

    std::vector<char> buffer(4096); // dynamic buffer
    std::string result;
    DWORD bytesRead = 0;

    while (InternetReadFile(urlFile, buffer.data(), buffer.size(), &bytesRead) && bytesRead > 0) 
        result.append(buffer.data(), bytesRead);

    InternetCloseHandle(urlFile);
    InternetCloseHandle(interwebs);

    return replaceAll(result, "|n", "\r\n");
}
