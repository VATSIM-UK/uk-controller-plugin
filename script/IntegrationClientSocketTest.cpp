// TEST CONNECTION TO NEW SERVER
std::this_thread::sleep_for(std::chrono::seconds(2));
struct addrinfo *addressInfo = nullptr, hints;
ZeroMemory(&hints, sizeof(hints));
hints.ai_family = AF_INET;
hints.ai_socktype = SOCK_STREAM;
hints.ai_protocol = IPPROTO_TCP;
hints.ai_flags = AI_PASSIVE;

// Resolve the local address and port to be used by the server
int getResult = getaddrinfo("127.0.0.1", "52814", &hints, &addressInfo);
if (getResult != 0) {
    LogError("getaddrinfo failed when connecting to integration server: " + std::to_string(getResult));
    return;
}

// Create the socket
SOCKET clientSocket = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);
if (clientSocket == INVALID_SOCKET) {
    LogError("Failed to initialise integration client socket: " + std::to_string(WSAGetLastError()));
    return;
}

int connectResult = connect(clientSocket, addressInfo->ai_addr, (int)addressInfo->ai_addrlen);
if (connectResult == SOCKET_ERROR) {
    LogError("Failed to connect to integration client socket: " + std::to_string(WSAGetLastError()));
}

// Send init message
thread = std::make_shared<std::thread>([clientSocket]() {
    nlohmann::json init{
        {"type", "initialise"},
        {"version", 1},
        {"id", "init_message"},
        {"type", "initialise"},
        {"data",
         {{"integration_name", "UKCPTEST"},
          {"integration_version", "TEST01"},
          {"event_subscriptions",
           nlohmann::json::array(
               {{{"type", "stand_assigned"}, {"version", 1}},
                {{"type", "stand_unassigned"}, {"version", 1}}})}}},
    };
    std::string initMsg = init.dump().c_str();
    initMsg.append({'\x1F'});
    int sendResult = send(clientSocket, initMsg.c_str(), initMsg.size(), 0);
    if (sendResult == SOCKET_ERROR) {
        LogError("Failed to send: " + std::to_string(WSAGetLastError()));
    }
    
    int bytesReceived;
    std::array<char, 4096> receiveBuffer;
    bool sentStandRequest = false;
    do {
        bytesReceived = recv(clientSocket, &receiveBuffer[0], 4096, 0);
        if (bytesReceived > 0) {
            std::string message = std::string(receiveBuffer.cbegin(), receiveBuffer.cbegin() + bytesReceived);
            LogInfo("Message received: " + message + "\n\n");
            if (!sentStandRequest) {
                nlohmann::json stand{
                    {"type", "assign_stand"},
                    {"version", 1},
                    {"id", "test_message_id"},
                    {"data",
                     {{"callsign", "BAW999"},
                         {"airfield", "EGLL"},
                         {"stand", "502"},
                     }},
                };
                std::string standMessage = stand.dump().c_str();
                standMessage.append({'\x1F'});
                int sendResult = send(clientSocket, standMessage.c_str(), standMessage.size(), 0);
                if (sendResult == SOCKET_ERROR) {
                    LogError("Failed to send: " + std::to_string(WSAGetLastError()));
                }
                sentStandRequest = true;
            }
        }
    } while (bytesReceived > 0);
});
thread->detach();
