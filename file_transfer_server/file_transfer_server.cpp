// schaeling_examples.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
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
#include <array>
#include <string>
#include <fstream>
#include <boost/asio.hpp>
#include <algorithm>

using namespace boost::asio;
using namespace boost::asio::ip;



io_service ioservice;
tcp::endpoint tcp_endpoint{ tcp::v4(), 2014 };
tcp::acceptor tcp_acceptor{ ioservice, tcp_endpoint };
tcp::socket tcp_socket{ ioservice };
std::array<char, 4096> bytes;

std::ofstream fileStream;

void read_handler(const boost::system::error_code &ec,
	std::size_t bytes_transferred)
{
	if (!ec)
	{
		fileStream.write(bytes.data(), bytes_transferred);
		tcp_socket.async_read_some(buffer(bytes), read_handler);
	}
	else
		fileStream.close();
}

void accept_handler(const boost::system::error_code &ec)
{
	using namespace std;
	if (!ec)
	{
		fileStream.open("test.dat", ios_base::out | ios_base::binary);
		
		tcp_socket.async_read_some(buffer(bytes), read_handler);
	}
}

void testFileTransferServer()
{
	boost::asio::ip::tcp::resolver resolver{ ioservice };
	
	std::string h = ip::host_name();
	std::cout << "Host name is " << h << '\n';
	std::cout << "IP addresses are: \n";
	
	auto results = resolver.resolve({ h, "" });
	for (const auto& re: results)
		std::cout << re.endpoint().address() << '\n';

	/*std::for_each(resolver.resolve({ h, "" }), {}, [](const auto& re) {
		std::cout << re.endpoint().address() << '\n';
	});*/

	tcp_acceptor.listen();
	tcp_acceptor.async_accept(tcp_socket, accept_handler);
	ioservice.run();
}


int main()
{
	
	testFileTransferServer();
}