#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

// �|�[�g�ԍ�
const unsigned short SERVERPORT = 8888;
// ����M���郁�b�Z�[�W�̍ő�l
const unsigned int MESSAGELENGTH = 1024;


int main() {
    // WinSock�̏�����
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    // �\�P�b�g�̍쐬
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        return 1;
    }

    // �T�[�o�[�ɐڑ�
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
   
    serverAddr.sin_port = htons(SERVERPORT); // �T�[�o�[�̃|�[�g�ԍ����w��
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr.s_addr);
    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to server." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server." << std::endl;

    // ����񂯂�̑I��
    int choice;
    do {
        std::cout << "Enter your move (1: Rock, 2: Paper, 3: Scissors): ";
        std::cin >> choice;
    } while (choice < 1 || choice > 3);

    // �I�����T�[�o�[�ɑ��M
    send(clientSocket, reinterpret_cast<const char*>(&choice), sizeof(choice), 0);

    // ���ʂ���M���ĕ\��
    int result;
    int bytesReceived = recv(clientSocket, reinterpret_cast<char*>(&result), sizeof(result), 0);
    if (bytesReceived > 0) {
        if (result == 0)
            std::cout << "It's a tie!" << std::endl;
        else if (result == 1)
            std::cout << "You win!" << std::endl;
        else
            std::cout << "You lose!" << std::endl;
    }
    else {
        std::cerr << "Recv failed or server disconnected." << std::endl;
    }

    // �\�P�b�g�����
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}