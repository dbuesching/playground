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

using namespace boost::asio;

void testSimpleTimer()
{
	io_service ioservice;

	steady_timer timer{ ioservice, std::chrono::seconds{3} };
	timer.async_wait([](const boost::system::error_code &ec)
	{ std::cout << "3 sec\n"; });

	ioservice.run();
}

void test2Timers()
{
	io_service ioservice;

	steady_timer timer1{ ioservice, std::chrono::seconds{3} };
	timer1.async_wait([](const boost::system::error_code &ec)
	{ std::cout << "3 sec\n"; });

	steady_timer timer2{ ioservice, std::chrono::seconds{4} };
	timer2.async_wait([](const boost::system::error_code &ec)
	{ std::cout << "4 sec\n"; });

	ioservice.run();
}

void KlasingExample()
{
	using namespace std;
	io_service ioservice;

	steady_timer timer{ ioservice, std::chrono::seconds{5} };
	timer.async_wait([](const boost::system::error_code& ec) {
		cout << "-> 5 sec (non blocking)\n"; });

	//ioservice.run();
	//thread a{ static_cast<io_context::count_type(io_service::*)()>(&io_service::run), &ioservice };
	//thread a(&io_service::run, &ioservice);
	thread a{ [&ioservice]() { ioservice.run(); } };
	a.detach();

	cout << "-> doing something else" << endl;

	steady_timer timer1{ ioservice, std::chrono::seconds{6} };
	timer1.wait();
	cout << "-> 6 sec (blocking)\n";

	return;
}

void testMultiple_io_service()
{
	io_service ioservice1;
	io_service ioservice2;

	steady_timer timer1{ ioservice1, std::chrono::seconds{3} };
	timer1.async_wait([](const boost::system::error_code &ec)
	{ std::cout << "3 sec\n"; });

	steady_timer timer2{ ioservice2, std::chrono::seconds{3} };
	timer2.async_wait([](const boost::system::error_code &ec)
	{ std::cout << "3 sec\n"; });

	std::thread thread1{ [&ioservice1]() { ioservice1.run(); } };
	std::thread thread2{ [&ioservice2]() { ioservice2.run(); } };
	thread1.join();
	thread2.join();
}

using namespace boost::asio::ip;

io_service ioservice;
tcp::resolver resolv{ ioservice };
tcp::socket tcp_socket{ ioservice };
std::array<char, 4096> bytes;

void read_handler(const boost::system::error_code &ec,
	std::size_t bytes_transferred)
{
	if (!ec)
	{
		std::cout.write(bytes.data(), bytes_transferred);
		tcp_socket.async_read_some(buffer(bytes), read_handler);
	}
}

void connect_handler(const boost::system::error_code &ec)
{
	if (!ec)
	{
		std::string r =
			"GET / HTTP/1.1\r\nHost: theboostcpplibraries.com\r\n\r\n";
		write(tcp_socket, buffer(r));
		tcp_socket.async_read_some(buffer(bytes), read_handler);
	}
}

void resolve_handler(const boost::system::error_code &ec,
	tcp::resolver::iterator it)
{
	if (!ec)
		tcp_socket.async_connect(*it, connect_handler);
}

void testWebClient()
{
	tcp::resolver::query q{ "theboostcpplibraries.com", "80" };
	resolv.async_resolve(q, resolve_handler);
	ioservice.run();
}


int main()
{
	//testSimpleTimer();
	//test2Timers();
	//KlasingExample();
	//testMultiple_io_service();
	testWebClient();
}