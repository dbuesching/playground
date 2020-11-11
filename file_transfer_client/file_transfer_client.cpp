// file_transfer_client.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include "pch.h"
#include <iostream>
#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <fstream>

using namespace boost::asio;
using namespace boost::asio::ip;

io_service ioservice;
tcp::resolver resolv{ ioservice };
tcp::socket tcp_socket{ ioservice };
std::array<char, 4096> bytes;

std::ifstream inStream;


void write_handler(const boost::system::error_code &ec,
	std::size_t bytes_transferred)
{
	if (!ec)
	{
		std::cout << "written a block" << std::endl;
		inStream.read(bytes.data(), bytes.size());
		auto sizeRead = inStream.gcount();
		if (sizeRead > 0)
			tcp_socket.async_write_some(buffer(bytes, sizeRead), write_handler);
        else
            inStream.close();
	}
	else
	{
		std::cout << "no more data to read\n" << std::endl;
        inStream.close();
	}
}

void connect_handler(const boost::system::error_code &ec)
{
	if (!ec)
	{
		inStream.read(bytes.data(), bytes.size());
		auto sizeRead = inStream.gcount();
		if (sizeRead > 0)
			tcp_socket.async_write_some(buffer(bytes, sizeRead), write_handler);
	}
}

void resolve_handler(const boost::system::error_code &ec,
	tcp::resolver::iterator it)
{
	if (!ec)
		tcp_socket.async_connect(*it, connect_handler);
}

void test_file_transfer_client(const std::string& ip, const std::string& sFilename )
{
	using namespace std;
	inStream.open(sFilename.c_str(), ios_base::binary | ios_base::in);
	tcp::resolver::query q{ ip, "2014" };
	resolv.async_resolve(q, resolve_handler);
	ioservice.run();
}


int main(int argc, char** argv)
{
	std::string ip = argv[1];
	std::string sFilename = argv[2];
	test_file_transfer_client(ip, sFilename);
}

