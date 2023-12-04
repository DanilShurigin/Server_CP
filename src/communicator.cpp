#include <string>
#include <iostream>

#include "server_exceptions.hpp"
#include "client_base.hpp"
#include "log_writer.hpp"
#include "client.hpp"

#include "communicator.hpp"

Communicator::Communicator(
    int port,
    int queue_length,
    ClientBase &viewer,
    LogWriter& logger,
    int max_threads
):
    listen_socket_(socket(AF_INET, SOCK_STREAM, 0)),
    queue_length_(queue_length),
    self_addr_(new sockaddr_in),
    remote_addr_(new sockaddr_in),
    client_base_(viewer),
    logger_(logger),
    max_threads_(max_threads) {
    if( listen_socket_ == -1 ) {
        throw ServerException("Failed to create socket");
    }

    const int enable = 1;

    if( setsockopt(listen_socket_, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof enable) == -1 ) {
        throw ServerException("Failed to set socket options (SO_REUSEADDR)");
    }

    logger_("Threads limit: "+std::to_string(threads_limit_), Debug);
    if( max_threads_ > threads_limit_ || max_threads_ < 0 ) {
        logger_("The number of threads exceeds the limit. The value is forsed set to 1", Warn);
        max_threads_ = 1;
    }

    self_addr_->sin_family = AF_INET;
    self_addr_->sin_port = htons(port);
    self_addr_->sin_addr.s_addr = htonl(INADDR_ANY);

    if( bind(listen_socket_, reinterpret_cast< const sockaddr* >(self_addr_.get()), sizeof(sockaddr_in)) == -1 ) {
        throw ServerException("Failed to bind socket");
    }
}

void Communicator::operator()() {
    if( listen(listen_socket_, queue_length_) == -1 ) {
        throw ServerException("Failed to start listen");
    }

    logger_("Server is running", Info);

    socklen_t addr_len = sizeof(sockaddr_in);
    int client_socket;

    while( true ) {
        if( client_socket = accept(listen_socket_,
                                   reinterpret_cast< sockaddr* >(remote_addr_.get()),
                                   &addr_len); client_socket == -1 ) {
            logger_("Server fails to accept a client", Error);
            continue;
        }

        struct timeval timeout {30, 0};
        if( setsockopt(client_socket, SOL_SOCKET,
                       SO_RCVTIMEO, &timeout, sizeof timeout) == -1 ) {
            logger_("Failed to set client socket options (SO_RCVTIMEO)", Warn);
            continue;
        }

        uint16_t client_port = ntohs(remote_addr_->sin_port);
        logger_("Server succeed to accept a client. Port: "+std::to_string(client_port), Info);

        if( max_threads_ == 1 ) {
            ServeClient(client_socket, client_port);
        } else if(threads_ < max_threads_) {
            threads_++;
            std::thread new_thread([this,client_socket,client_port]() {
                ServeClient(client_socket, client_port);
            });
            new_thread.detach();
        }
    }
}

void Communicator::ServeClient(int work_socket, uint16_t port) {
    if( max_threads_ == 1 ) {
        logger_("Port: "+std::to_string(port)+". Start serving client", Info);
    } else {
        logger_("Port: "+std::to_string(port)+". Start serving client in datached thread", Info);
    }

    try {
        Client client(work_socket, port, client_base_, logger_);
        client.Serve();
    } catch( const ServerException& ex ) {
        logger_(ex.what(), Error);
    }

    close(work_socket);

    logger_("Port: "+std::to_string(port)+". Connection is closed", Info);

    if( max_threads_ > 1 ) {
        threads_--;
    }
}