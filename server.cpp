#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <vector> //best practice

using namespace std;

void handle_client(int client_socket)
{
    char buffer[1024];
    while (true)
    {
        memset(buffer, 0, 1024);
        // thay cho ham read
        int bytes_received = recv(client_socket, buffer, 1024, 0);

        if (bytes_received <= 0)
        {
            std::cout << "Client da ngat ket noi." << std::endl;
            close(client_socket);
            break;
        }

        std::cout << "Nhan tu Client: " << buffer << std::endl;

        for (int i = 0; i < bytes_received; i++)
        {
            buffer[i] = toupper(buffer[i]);
        }

        send(client_socket, buffer, bytes_received, 0);
    }
}

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // 1. Tao Socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 2. Gan cong (Bind)
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Chap nhan moi IP toi
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // 3. Lang nghe (Listen)
    listen(server_fd, 10);
    std::cout << "Server dang doi ket noi tai cong 8080..." << std::endl;

    while (true)
    {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (new_socket >= 0)
        {
            std::cout << "Co Client moi ket noi!" << std::endl;
            // Tao mot thread moi de xử lý Client nay
            std::thread t(handle_client, new_socket);
            t.detach(); // De thread chay doc lap voi main thread
        }
    }

    close(server_fd);
    return 0;
}

// g++ server.cpp -o server