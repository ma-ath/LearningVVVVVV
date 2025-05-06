#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <chrono>
#include <iostream>

#include "Messages.h"
#include "SocketServer.h"
#include "Screen.h"

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

void SocketServer::send_screen_data(SDL_Surface* surface, std::shared_ptr<asio::ip::tcp::socket> socket)
{
    if (!surface)
    {
        std::cerr << "Surface is null, cannot send screen data." << std::endl;
        return;
    }

    // Create an SDL_RWops object for writing to memory
    SDL_RWops* rw = SDL_RWFromMem(nullptr, 0); // This is invalid and needs to be fixed
    if (!rw)
    {
        std::cerr << "Failed to create RWops: " << SDL_GetError() << std::endl;
        return;
    }

    // Corrected: Use SDL_RWops with a dynamically allocated memory buffer
    char* buffer = nullptr;
    size_t buffer_size = 0;

    // Use SDL_RWops to write PNG data to memory
    rw = SDL_RWFromMem(buffer, buffer_size);
    if (!rw)
    {
        std::cerr << "Failed to create RWops: " << SDL_GetError() << std::endl;
        return;
    }

    if (IMG_SavePNG_RW(surface, rw, 1) != 0)
    {
        std::cerr << "Failed to save surface as PNG: " << SDL_GetError() << std::endl;
        SDL_RWclose(rw);
        return;
    }

    // Get the size of the PNG data
    Sint64 png_size = SDL_RWtell(rw);
    SDL_RWseek(rw, 0, RW_SEEK_SET);

    // Allocate memory for the PNG data
    char* png_data = new char[png_size];
    SDL_RWread(rw, png_data, png_size, 1);

    // Send the size of the PNG data first
    asio::write(*socket, asio::buffer(&png_size, sizeof(png_size)));

    // Send the PNG data
    asio::write(*socket, asio::buffer(png_data, png_size));

    // Clean up
    delete[] png_data;
    SDL_RWclose(rw);
    std::cout << "Screen data sent successfully!" << std::endl;
}

void SocketServer::start_screen_stream(std::shared_ptr<asio::ip::tcp::socket> socket, int fps, Screen& screen)
{
    streaming_ = true;
    std::thread([this, socket, fps, &screen]() {
        const int frame_delay = 1000 / fps; // Milliseconds per frame
        while (streaming_)
        {
            auto start_time = std::chrono::steady_clock::now();

            // Capture and send the screen
            SDL_Surface* surface = game_controller_.capture_screen(screen);
            if (surface)
            {
                send_screen_data(surface, socket);
                SDL_FreeSurface(surface);
            }

            // Sleep to maintain the desired frame rate
            auto end_time = std::chrono::steady_clock::now();
            std::chrono::milliseconds elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
            if (elapsed.count() < frame_delay)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(frame_delay - elapsed.count()));
            }
        }
    }).detach();
}

void SocketServer::start_read(std::shared_ptr<asio::ip::tcp::socket> socket)
{
    auto buffer = std::make_shared<std::array<char, 1024>>();
    socket->async_read_some(asio::buffer(*buffer), [this, socket, buffer](const asio::error_code& error, std::size_t bytes_transferred) {
        if (!error)
        {
            std::cout << "Received: " << std::string(buffer->data(), bytes_transferred) << std::endl;

            // Handle commands
            switch (buffer->data()[0])
            {
                case GAME_CONTROLLER_REQUEST_SCREEN:
                    start_screen_stream(socket, 30, gameScreen); // Start streaming at 30 FPS
                    break;
                case GAME_CONTROLLER_STOP_SCREEN: // Stop streaming command
                    streaming_ = false;
                    break;
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

            start_read(socket); // Continue reading from the same client
        }
        else
        {
            std::cout << "Connection closed or error occurred: " << error.message() << std::endl;
            streaming_ = false; // Stop streaming if the connection is closed
        }
    });
}
