#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <Wininet.h>  // Windows-specific header for internet functions
using namespace std;

void downloadFile(string url);

int main() {
    string url;
    cout << "Enter the URL of the file to download: ";
    cin >> url;

    downloadFile(url);

    return 0;
}

void downloadFile(string url){
    hInternet = InternetOpenA("FileDownload", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

    if (!hInternet) {
        cout << "Failed to initialize WinINet." << endl;
        return;
    }

    hConnect = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hConnect) {
        cout << "Failed to open the URL: " << GetLastError() << endl;
        return;
    }

    string outputFileName = "downloaded_file.zip"; // Default filename
    size_t lastSlashPos = url.find_last_of('/');
    if (lastSlashPos != string::npos) {
        outputFileName = url.substr(lastSlashPos + 1);
    }


    ofstream outputFile(outputFileName, ios::binary);
    if (!outputFile) {
        cout << "Failed to create output file." << endl;
        return;
    }

    char buffer[1024];
    DWORD bytesRead;
    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        outputFile.write(buffer, bytesRead);
    }
    outputFile.close();

    if (hConnect) {
        InternetCloseHandle(hConnect);
    }
    if (hInternet) {
        InternetCloseHandle(hInternet);
    }
    
    cout << "File downloaded successfully to: " << outputFileName << endl;
}


