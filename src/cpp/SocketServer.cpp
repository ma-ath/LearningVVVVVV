#include <SDL2/SDL.h>

#include "Messages.h"
#include "SocketServer.h"

SocketServer::SocketServer()
    : io_context_(),
      acceptor_(io_context_, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), VVVVVV_SERVER_PORT))

{
    start_accept();
}

SocketServer::~SocketServer()
{
    // Gracefully stop the server thread
    io_context_.stop();
    if (server_thread_.joinable())
        server_thread_.join();
}

void SocketServer::run()
{
    std::cout << "Server is running on port " << VVVVVV_SERVER_PORT << "..." << std::endl;

    // Launch the io_context event loop in a background thread
    server_thread_ = std::thread([this]() {
        io_context_.run();
    });
}

void SocketServer::start_accept()
{
    auto new_socket = std::make_shared<asio::ip::tcp::socket>(acceptor_.get_executor());
    acceptor_.async_accept(*new_socket, [this, new_socket](const asio::error_code& error) {
        if (!error)
        {
            std::cout << "New connection established!" << std::endl;
            start_read(new_socket);
        }
        else
        {
            std::cerr << "Accept error: " << error.message() << std::endl;
        }
        start_accept(); // Continue accepting more connections
    });
}

void SocketServer::start_read(std::shared_ptr<asio::ip::tcp::socket> socket)
{
    auto buffer = std::make_shared<std::array<char, 1024>>();
    socket->async_read_some(asio::buffer(*buffer), [this, socket, buffer](const asio::error_code& error, std::size_t bytes_transferred) {
        if (!error)
        {
            std::cout << "Received: " << std::string(buffer->data(), bytes_transferred) << std::endl;
            // Game Controller logic
            {
                switch (buffer->data()[0])
                {
                    case GAME_CONTROLLER_PRESS_UP:
                        game_controller_.pressKey(SDLK_UP);
                        break;
                    case GAME_CONTROLLER_RELEASE_UP:
                        game_controller_.releaseKey(SDLK_UP);
                        break;
                    case GAME_CONTROLLER_PRESS_DOWN:
                        game_controller_.pressKey(SDLK_DOWN);
                        break;
                    case GAME_CONTROLLER_RELEASE_DOWN:
                        game_controller_.releaseKey(SDLK_DOWN);
                        break;
                    case GAME_CONTROLLER_PRESS_LEFT:
                        game_controller_.pressKey(SDLK_LEFT);
                        break;
                    case GAME_CONTROLLER_RELEASE_LEFT:
                        game_controller_.releaseKey(SDLK_LEFT);
                        break;
                    case GAME_CONTROLLER_PRESS_RIGHT:
                        game_controller_.pressKey(SDLK_RIGHT);
                        break;
                    case GAME_CONTROLLER_RELEASE_RIGHT:
                        game_controller_.releaseKey(SDLK_RIGHT);
                        break;
                    case GAME_CONTROLLER_PRESS_Z:
                        game_controller_.pressKey(SDLK_z);
                        break;
                    case GAME_CONTROLLER_RELEASE_Z:
                        game_controller_.releaseKey(SDLK_z);
                        break;
                    case GAME_CONTROLLER_PRESS_ESC:
                        game_controller_.pressKey(SDLK_ESCAPE);
                        break;
                    case GAME_CONTROLLER_RELEASE_ESC:
                        game_controller_.releaseKey(SDLK_ESCAPE);
                        break;
                    default:
                        std::cout << "Unknown command received." << std::endl;
                }
            }
            start_read(socket); // Continue reading from the same client
        }
        else
        {
            std::cout << "Connection closed or error occurred: " << error.message() << std::endl;
        }
    });
}
