#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <Wininet.h>  // Windows-specific header for internet functions
using namespace std;


class FileDownloader {
public:
    FileDownloader() {
        // Initialize WinINet
        hInternet = InternetOpenA("FileDownload", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        if (!hInternet) {
            cerr << "Failed to initialize WinINet." << endl;
        }
    }

    ~FileDownloader() {
        // Close WinINet handles in the destructor
        if (hConnect) {
            InternetCloseHandle(hConnect);
        }
        if (hInternet) {
            InternetCloseHandle(hInternet);
        }
    }

    bool downloadFile(const string& url) {
        if (!hInternet) {
            cerr << "WinINet is not initialized." << endl;
            return false;
        }

        // Open the URL
        hConnect = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (!hConnect) {
            cerr << "Failed to open the URL: " << GetLastError() << endl;
            return false;
        }

        // Generate a filename based on the URL
        string outputFileName = "downloaded_file.csv"; // Default filename
        size_t lastSlashPos = url.find_last_of('/');
        if (lastSlashPos != string::npos) {
            outputFileName = url.substr(lastSlashPos + 1);
        }

        // Save the downloaded content to a file
        ofstream outputFile(outputFileName, ios::binary);
        if (!outputFile) {
            cerr << "Failed to create output file." << endl;
            return false;
        }

        // Read and save the content
        char buffer[1024];
        DWORD bytesRead;
        while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
            outputFile.write(buffer, bytesRead);
        }

        outputFile.close();
        cout << "File downloaded successfully to: " << outputFileName << endl;

        return true;
    }

private:
    HINTERNET hInternet = nullptr;
    HINTERNET hConnect = nullptr;
};

int main() {
    string url;
    cout << "Enter the URL of the file to download: ";
    cin >> url;

    FileDownloader downloader;
    if (downloader.downloadFile(url)) {
        cout << "Download successful!" << endl;
    } else {
        cerr << "Download failed." << endl;
    }

    return 0;
}