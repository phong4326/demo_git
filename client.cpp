#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    const char* hello = "Hello tu Client!";
    char buffer[1024] = {0};

    // 1. Tao Socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Loi tao Socket" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    // Chuyen dia chi IP sang dang nhi phan
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cout << "Dia chi khong hop le" << std::endl;
        return -1;
    }

    // 2. Ket noi (Connect)
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Ket noi that bai" << std::endl;
        return -1;
    } 

    // 3. Gui va Nhan du lieu
    send(sock, hello, strlen(hello), 0);
    std::cout << "Da gui loi chao toi Server." << std::endl;

    read(sock, buffer, 1024);
    std::cout << "Server phan hoi: " << buffer << std::endl;

    char s[1024];
    std::cout << "Nhap chuoi: ";
    std::cin.getline(s, 1024);
    
    send (sock, s, strlen(s), 0);

    memset(buffer, 0, 1024);
    read(sock, buffer, 1024);
    std::cout << buffer << std::endl;

    close(sock);
    return 0;
}

//g++ client.cpp -o client