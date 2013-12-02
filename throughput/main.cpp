#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <Windows.h>

using boost::asio::ip::udp;

#define PORT 3007

class UdpServer {

public:
	UdpServer(boost::asio::io_service& io_service) : socket(io_service, udp::endpoint(udp::v4(), PORT)), count(0) {
		start_receive();
	}

private:
	void start_receive() {
		socket.async_receive_from( boost::asio::buffer(recv_buffer), remote_endpoint,
			boost::bind(&UdpServer::handle_receive, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		);
	}

	void handle_receive(const boost::system::error_code& error, std::size_t /*bytes_transferred*/) {
		count++;
		std::cout << "Got message:" << count << std::endl;
		start_receive();
		/*
		if (!error || error == boost::asio::error::message_size) {
			boost::shared_ptr<std::string> message(new std::string("Test String."));
			
			socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
				boost::bind(&UdpServer::handle_send, this, message,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)
			);
			
			start_receive();
		}
		*/
	}

	/*
	void handle_send(boost::shared_ptr<std::string>, const boost::system::error_code&, std::size_t) {
	
	}
	*/

	udp::socket socket;
	udp::endpoint remote_endpoint;
	boost::array<char, 1> recv_buffer;
	int count;

};

class UdpClient {
public:
	UdpClient(boost::asio::io_service& io_service) : socket(io_service), count(0) {
		
		socket.open(udp::v4());

	}

	void StartSending() {
		while(true) {
			//Sleep(16);
			SendString("Test String");
		}
	}

	void SendString(const char* msg) {
		boost::asio::ip::udp::endpoint destination( boost::asio::ip::address::from_string("72.80.102.69"), PORT );
		socket.send_to(boost::asio::buffer("My Text"), destination);
		count++;
		std::cout << "Sent count: " << count << std::endl;

		if (count >= 30000) {
			system("PAUSE");
			exit(0);
		}
	}

/*
	void Receive() {
		boost::array<char, 128> recv_buf;
		udp::endpoint sender_endpoint;
		size_t len = socket.receive_from(
        boost::asio::buffer(recv_buf), sender_endpoint);

		std::cout.write(recv_buf.data(), len);
	}
*/

private:

	udp::socket socket;
	int count;
	//udp::resolver resolver;

};

int main(int argc, char* argv[]) {
	
	boost::asio::io_service io_service;

	try {

		if (argc >= 2) {
			std::cout << "Running server..." << std::endl;
			UdpServer server(io_service);
			io_service.run();
		} else {
			std::cout << "Running client..." << std::endl;
			UdpClient client(io_service);
			io_service.run();
			std::cout << "Sending messages..." << std::endl;
			client.StartSending();
		}

	} catch (std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		system("PAUSE");
	}
	
	return 0;
}