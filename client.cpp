#include "client.h"

#include <iostream>
#include <vector>

#include <Windows.h>
#include <Commdlg.h>
#include <locale>
#include <codecvt>
#include <string>
#include <fstream>

#include <filesystem>


#define MAX_BUFFER_SIZE 8192

Client::Client()
{

}

void Client::sendFileData(const std::vector<uint8_t>& fileData)
{
    size_t remainingBytes = fileData.size();

    size_t offset = 0;

    while (remainingBytes > 0)
    {
        size_t chunkSize = std::min(remainingBytes, static_cast<size_t>(MAX_BUFFER_SIZE));

        if (send(client.getSocket(), reinterpret_cast<const char*>(&fileData[offset]), chunkSize, 0) == SOCKET_ERROR)
        {
            std::cerr << "Failed to send file data." << std::endl;
            return;
        }

        remainingBytes -= chunkSize;
        offset += chunkSize;
    }
}

void Client::start()
{
    client.create_socket("Client","127.0.0.1",54000);

    int result = client.connect_socket();

    while(result == -1)
    {
        std::cerr << "Client: failed to connect to server"<< std::endl;
        result = client.connect_socket();
    }

    if(result == 1)
    {
        std::cout << "Client: Connected" << std::endl;

        std::filesystem::path selected_path = openFileExplorer();

        std::vector<uint8_t> fileBytes = readFileBytes(selected_path.string());

        int size_of_file = fileBytes.size();

        if(!fileBytes.empty())
        {
            std::string path_to_save;

            std::cout << "Enter path of file to save: ";

            std::cin >> path_to_save;

            path_to_save.append('\\' + selected_path.filename().string());

            if(!path_to_save.empty())
            {
                int bytesSent_path = send(client.getSocket(), path_to_save.c_str(), path_to_save.size(), 0);

                int converted_number = htonl(size_of_file);        
                int bytesSent_size = send(client.getSocket(),  reinterpret_cast<char*>(&converted_number), sizeof(converted_number), 0);

                sendFileData(fileBytes);

                if (bytesSent_path == SOCKET_ERROR || bytesSent_size == SOCKET_ERROR)
                    std::cerr << "Failed to send data to the server." << std::endl;

                else
                    std::cout << "File was sent." << std::endl;
            }
        }
        else
        {
            std::cout << "File selection was canceled" << std::endl;
        }

        client.close_socket();
    }
}

std::vector<uint8_t> Client::readFileBytes(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (!file)
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return std::vector<uint8_t>();
    }

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(fileSize);

    if (file.read(reinterpret_cast<char*>(buffer.data()), fileSize))
    {
        std::cout << "File read successfully. Bytes read: " << fileSize << std::endl;
    }
    else
    {
        std::cerr << "Failed to read file: " << filename << std::endl;
        buffer.clear();
    }

    file.close();

    return buffer;
}

std::string Client::openFileExplorer()
{
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = new TCHAR[MAX_PATH];
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = TEXT("All Files\0*.*\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn))
    {
        std::wstring filePath = ofn.lpstrFile;
        delete[] ofn.lpstrFile;

        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

        return converter.to_bytes(filePath);
    }
    else
    {
        DWORD errorCode = CommDlgExtendedError();

        if (errorCode != 0)
            std::cerr << "Client: Error occurred while opening file dialog. Error code: " << errorCode << std::endl;

        else
            std::cout << "Client: File selection canceled by the user." << std::endl;

    }

    delete[] ofn.lpstrFile;

    return "";
}
