#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#define VVVVVV_SERVER_PORT 8000 // Default port for the server

#include <array>
#include <iostream>
#include <memory>
#include <asio.hpp>
#include <thread>

#include "GameController.h"


class SocketServer
{
public:
    SocketServer(); // Constructor
    ~SocketServer(); // Destructor
    void run();     // Starts the server

private:
    void start_accept(); // Begins accepting connections
    void start_read(std::shared_ptr<asio::ip::tcp::socket> socket); // Reads data from a client

    asio::io_context io_context_; // ASIO context for managing I/O
    asio::ip::tcp::acceptor acceptor_; // TCP acceptor for handling connections
    std::thread server_thread_;
    GameController game_controller_; // Game controller instance
};

#endif // SOCKETSERVER_H
