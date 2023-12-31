/**
 * @file client.cpp
 * @author Шурыгин Д.Д.
 * @brief Файл реализации класса Client.
 * @date 2023-12-16
 * @warning Создано только в учебных целях.
 */
#include <limits>
#include <memory>
#include <random>
#include <netinet/in.h>
#include <unistd.h>

#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/simple.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>

#include "server_exceptions.hpp"
#include "client_base.hpp"
#include "log_writer.hpp"

#include "client.hpp"

std::pair< bool, std::string > Client::RecvMessage() {
    ssize_t capacity = 32;
    std::unique_ptr< char[] > buf(new char[capacity]);
    std::string recieved_line;

    ssize_t line_size;
    while( (line_size = recv(work_socket_, buf.get(), capacity, MSG_PEEK)) == capacity ) {
        if( line_size == -1 ) {
            return std::make_pair(false, recieved_line);
        }
        capacity *= 2;
        buf = std::unique_ptr< char[] >(new char[capacity]);
    }

    if( recv(work_socket_, buf.get(), capacity, 0) == -1) {
        return std::make_pair(false, recieved_line);
    }

    recieved_line = std::string(buf.get(), line_size);

    return std::make_pair(true, recieved_line);
}

void Client::Authentificate() {
    auto [rcv_id_status, id] = RecvMessage();

    if (!rcv_id_status) {
        throw AuthException("Failed to recieve ID");
    }

    logger_("Port: "+std::to_string(port_)+". Recieved ID: "+id, Debug);

    auto [pass_status, pass] = cb_viewer_(id);
    if( !pass_status ) {
        send(work_socket_, "ERR", 4, 0);
        throw AuthException("Port: "+std::to_string(port_)+". ID "+id+" doesn`t exist in the client base");
    }

    std::random_device rnd;
    std::mt19937_64 rnd_generator(rnd());
    uint64_t rnd_number = rnd_generator();
    std::string salt;

    CryptoPP::StringSource((const CryptoPP::byte*)&rnd_number,
                           8,
                           true,
                           new CryptoPP::HexEncoder(new CryptoPP::StringSink(salt))
                          );

    if( send(work_socket_, salt.c_str(), 16, 0) == -1) {
        throw AuthException("Port: "+std::to_string(port_)+". Failed to send SALT");
    }

    auto [rcv_hash_status, client_hash] = RecvMessage();
    if( !rcv_hash_status ) {
        throw AuthException("Port: "+std::to_string(port_)+". Failed to recieve HASH");
    }

    CryptoPP::Weak::MD5 hash;
    std::string result_hash;
    CryptoPP::StringSource(salt + pass, true,
                           new CryptoPP::HashFilter(hash,
                                   new CryptoPP::HexEncoder(
                                       new CryptoPP::StringSink(result_hash)
                                   )
                                )
                          );

    if( client_hash != result_hash ) {
        send(work_socket_, "ERR", 4, 0);
        throw AuthException("Port: "+std::to_string(port_)+". Authentification error");
    }
    if( send(work_socket_, "OK", 3, 0) == -1 ) {
        throw AuthException("Port: "+std::to_string(port_)+". Failed to send OK");
    }
}

std::vector<int32_t> Client::RecvVector() {
    uint32_t vector_size;
    if( recv(work_socket_, (void*)&vector_size, sizeof(uint32_t), 0) == -1) {
        throw ClientException("Port: "+std::to_string(port_)+". Failed to receive size of vector");
    }

    logger_("Port: "+std::to_string(port_)+". Recieved size of vector: "+std::to_string(vector_size), Debug);

    std::unique_ptr< int32_t[] > v(new int32_t[vector_size]);
    int v_size;
    if( (v_size = recv(work_socket_, (void*)v.get(), sizeof(int32_t)*vector_size, 0)) == -1 ) {
        throw ClientException("Port: "+std::to_string(port_)+". Failed to receive vector");
    }
    if( sizeof(int32_t)*vector_size != (uint32_t)v_size ) {
        throw ClientException("Port: "+std::to_string(port_)+". Failed to receive vector. Size error");
    }

    logger_("Port: "+std::to_string(port_)+". Vector is successfuly recieved", Debug);

    std::vector<int32_t> vec(vector_size);
    for (uint32_t i=0; i<vector_size; ++i) {
        vec.at(i) = v[i];
    }
    return vec;
}

int32_t Client::CalculateVector(const std::vector<int32_t>& vec) {
    int32_t sum_result = 0;
    for(uint32_t i=0; i<vec.size(); i++) {
        if( (vec.at(i) > 0) && (sum_result > (std::numeric_limits< int32_t >::max() - vec.at(i))) ) {
            sum_result = std::numeric_limits< int32_t >::max();
            logger_("Port: "+std::to_string(port_)+". Overflow during summation in vector", Debug);
            break;
        }
        if( (vec.at(i) < 0) && (sum_result < (std::numeric_limits< int32_t >::min() - vec.at(i))) ) {
            sum_result = std::numeric_limits< int32_t >::min();
            logger_("Port: "+std::to_string(port_)+". Underflow during summation in vector", Debug);
            break;
        }
        sum_result += vec.at(i);
    }

    logger_("Port: "+std::to_string(port_)+". Summation in vector is successfuly ended", Debug);

    return sum_result;
}

void Client::Calculate() {
    uint32_t vectors_count;

    if( recv(work_socket_, (void*)&vectors_count, sizeof(uint32_t), 0) == -1) {
        throw ClientException("Port: "+std::to_string(port_)+". Failed to receive vectors count");
    }

    logger_("Port: "+std::to_string(port_)+". Recieved vectors count "+std::to_string(vectors_count), Debug);

    for( uint32_t i=0; i<vectors_count; i++) {       
        int32_t sum_result = CalculateVector(RecvVector());
        
        if( send(work_socket_, (void*)&sum_result, sizeof(int32_t), 0) == -1 ) {
            throw ClientException("Port: "+std::to_string(port_)+". Failed to send result");
        }
        logger_("Port: "+std::to_string(port_)+". Result for vector No"+std::to_string(i+1)+" is successfuly sent", Debug);
    }
}