#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#define VVVVVV_SERVER_PORT 8000

#include <atomic>
#include <asio.hpp>
#include <thread>
#include <SDL2/SDL.h>
#include "GameController.h"
#include "Screen.h"


class SocketServer
{
public:
    SocketServer();
    ~SocketServer();
    void run();
    void start_screen_stream(std::shared_ptr<asio::ip::tcp::socket> socket, int fps, Screen& screen);

private:
    void start_accept();
    void start_read(std::shared_ptr<asio::ip::tcp::socket> socket);
    void send_screen_data(SDL_Surface* surface, std::shared_ptr<asio::ip::tcp::socket> socket);

    asio::io_context io_context_;
    asio::ip::tcp::acceptor acceptor_;
    std::thread server_thread_;
    GameController game_controller_;

    std::atomic<bool> streaming_; // Flag to control screen streaming
};

#endif // SOCKETSERVER_H
