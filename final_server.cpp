#include <iostream>
#include <windows.h>
#include <thread>
#include<queue>
# include<string>
#include<map>
#include<set>
# include<filesystem>
# include<fstream>
using namespace std;
map<string, string> reguser;
map<string, int> onlineuser;

#pragma comment(lib, "ws2_32.lib")

class user {
public:
    string name, id, password, favorite_music;
    SOCKET clientSocket;
public:
    void find_friend(string s) {

    }
    void accept_request(string s) {

    }
    void send_request(string s) {

    }
    void recomend_friend() {

    }
    void send_message() {

    }
    void send_notification() {

    }
    void logout() {
        onlineuser[this->id] -= 1;
        if (onlineuser[this->id] == 0) onlineuser.erase(this->id);
    }
};



string  recieved(SOCKET clientSocket) {
    char buffer[1024];
    //  std::string response1 = "(1) press 1 and enter to log in ,(2) press 2 to create a new account ,";
     //send(clientSocket, response1.c_str(), response1.length(), 0);

      // Receive data from the client
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    string result = buffer;
    return result;

}

void f_logged_in(string, SOCKET);
void f_logged_in(string s1, SOCKET clientSocket) {
    onlineuser[s1]++;
    user obj1;
    obj1.id = s1;
    obj1.clientSocket = clientSocket;
    while (1) {
        string result = recieved(clientSocket);
        string s1;
        int i = 0;
        while (result[i] != ',') {
            s1.push_back(result[i]);
            i++;
        }
        if (s1 == "find_friend") {
            string s2;
            //  find string s2
            obj1.find_friend(s2);


        }
        else if (s1 == "accept_request") {
            string s2;
            // find string s2
            obj1.accept_request(s2);

        }
        else if (s1 == "recomend_friend") {
            
            obj1.recomend_friend();

        }
        else if (s1 == "logout") {
            obj1.logout();
            /// from here we have to run the function  of f_start_page 

            break;
        }
        else if (s1=="send_request") {
            string s2;
            // find string s2

            obj1.send_request(s2);
        }
        else if (s1=="send_message") {

        }
    }

}
void start_page(SOCKET clientSocket) {
    int flag_start = 0;
    while (1) {
        char buffer[1024];
        //  std::string response1 = "(1) press 1 and enter to log in ,(2) press 2 to create a new account ,";
         //send(clientSocket, response1.c_str(), response1.length(), 0);

          // Receive data from the client
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead > 0) {
            cout << buffer[0] << endl;
            if (buffer[0] == '1') {
                //continue;
               // log_in(clientSocket, flag_start);
                int i = 2;
                string s1, s2;
                while (buffer[i] != ',') {
                    s1.push_back(buffer[i]);
                    i++;
                }
                i++;
                while (buffer[i] != ',') {
                    s2.push_back(buffer[i]);
                    i++;
                }
                // log in succesful or not 
                if (reguser.count(s1) && reguser[s1] == s2) {
                    std::string response1 = "log in sucessful";
                    send(clientSocket, response1.c_str(), response1.length(), 0);
                    f_logged_in(s1,clientSocket);
                    continue;
                }
                else {
                    // send an string  "error" 
                    std::string response1 = "error";
                    send(clientSocket, response1.c_str(), response1.length(), 0);

                    continue;
                }

                if (flag_start) continue;

            }
            else if (buffer[0] == '2') {
                //continue;
               // sign_up(clientSocket, flag_start);
                int i = 2;
                string s1, s2;
                while (buffer[i] != ',') {
                    s1.push_back(buffer[i]);
                    i++;
                }
                i++;
                while (buffer[i] != ',') {
                    s2.push_back(buffer[i]);
                    i++;
                }
                /// account created or failed
                if (reguser.count(s1) ){
                    std::string response1 = "account already exist";
                   send(clientSocket, response1.c_str(), response1.length(), 0);
                }
                else {
                    reguser[s1] = s2;
                    std::string response1 = "account created";
                    send(clientSocket, response1.c_str(), response1.length(), 0);
                }
                continue;
                
            }
            else if (buffer[0] == '3') {
                continue;
            }
            else if (buffer[0] == '4') {
                closesocket(clientSocket);
            }

        }
    }
   
}
int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock" << std::endl;
        return 1;
    }

    // Create a socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket" << std::endl;
        WSACleanup();
        return 1;
    }

    // Bind the socket to a specific IP address and port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");; // Listen on all available network interfaces
    serverAddr.sin_port = htons(12345); // Replace with desired port number

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind socket" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Failed to listen for connections" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server listening on port 12345" << std::endl;
   // thread handle_message(handle_message_queue);
   // handle_message.detach();
    //std::string fileName = "MAHAKAL_DATABASE";
    //std::fstream outputFile;
    //outputFile.open(fileName);

    while (true) {
        // Accept a client connection
        clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Failed to accept client connection" << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        std::cout << "Connected to client" << std::endl;

        // Handle the client connection in a separate thread
        // when client get connect connect person to home page 
        std::thread clientThread(start_page, clientSocket);
        clientThread.detach();
    }

    // Cleanup Winsock
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
