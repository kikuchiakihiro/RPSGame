#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

// ポート番号
const unsigned short SERVERPORT = 8888;
// 送受信するメッセージの最大値
const unsigned int MESSAGELENGTH = 1024;


int main() {
    // WinSockの初期化
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    // ソケットの作成
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        return 1;
    }

    // サーバーに接続
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
   
    serverAddr.sin_port = htons(SERVERPORT); // サーバーのポート番号を指定
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr.s_addr);
    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to server." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server." << std::endl;

    // じゃんけんの選択
    int choice;
    do {
        std::cout << "Enter your move (1: Rock, 2: Paper, 3: Scissors): ";
        std::cin >> choice;
    } while (choice < 1 || choice > 3);

    // 選択をサーバーに送信
    send(clientSocket, reinterpret_cast<const char*>(&choice), sizeof(choice), 0);

    // 結果を受信して表示
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

    // ソケットを閉じる
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}