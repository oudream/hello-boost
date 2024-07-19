#include <boost/asio.hpp>
#include <iostream>
#include <functional>
#include <chrono>
#include <string>
#include <sstream> // Include for std::stringstream
#include <iomanip>  // Include for std::setw and std::setfill

using namespace boost::asio;
using namespace std::chrono;
using std::placeholders::_1;
using std::placeholders::_2;

class SerialPortReader {
public:
    SerialPortReader(io_service& io_service, const std::string& port_name)
            : io_service_(io_service), serial_port_(io_service, port_name), total_bytes_received_(0), timer_(io_service) {
        // 配置串口参数
        serial_port_.set_option(serial_port_base::baud_rate(9600));
        serial_port_.set_option(serial_port_base::character_size(8));
        serial_port_.set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::one));
        serial_port_.set_option(serial_port_base::parity(serial_port_base::parity::none));
        serial_port_.set_option(serial_port_base::flow_control(serial_port_base::flow_control::none));

        // 开始异步读取
        start_read();

        // 开始定时发送数据
        start_send();
    }

private:
    io_service& io_service_;
    serial_port serial_port_;
    enum { max_read_length = 512 };
    char data_[max_read_length];
    size_t total_bytes_received_;
    steady_clock::time_point last_receive_time_;
    steady_timer timer_;

    void start_read() {
        serial_port_.async_read_some(buffer(data_, max_read_length),
                                     [this](const boost::system::error_code& error, size_t bytes_transferred) {
                                         this->handle_read(error, bytes_transferred);
                                     });
    }

    void handle_read(const boost::system::error_code& error, size_t bytes_transferred) {
        if (!error) {
            auto now = steady_clock::now();
            if (total_bytes_received_ == 0) { // First byte received
                last_receive_time_ = now;
            }

            // Calculate duration since last receive
            auto duration = duration_cast<milliseconds>(now - last_receive_time_).count();

            // Convert received data to hex string
            std::stringstream hex_stream;
            hex_stream << std::hex << std::uppercase;
            for (size_t i = 0; i < bytes_transferred; ++i) {
                hex_stream << std::setw(2) << std::setfill('0') << static_cast<int>(static_cast<unsigned char>(data_[i])) << " ";
            }

            total_bytes_received_ += bytes_transferred;
            std::cout << "Time since last receive: " << duration << " ms, "
                      << "Total bytes received: " << total_bytes_received_
                      << ", Current bytes received: " << bytes_transferred
                      << ", Data: " << hex_stream.str()
                      << std::endl;

            // Update last receive time
            last_receive_time_ = now;

            // 继续异步读取
            start_read();
        } else {
            std::cerr << "Error during receive: " << error.message() << std::endl;
        }
    }

    void start_send() {
        timer_.expires_after(std::chrono::milliseconds (500)); // 每5秒发送一次数据
        timer_.async_wait([this](boost::system::error_code ec) {
            if (!ec) {
                send_data("Hello, world!"); // 发送数据
                start_send(); // 重新启动定时器
            }
        });
    }

    void send_data(const std::string& message) {
        write(serial_port_, buffer(message)); // 同步发送数据
    }

};

int main() {
    try {
        io_service io_service;
        // 请替换下面的串口名称为你的实际串口名称，例如 "COM3"。
        SerialPortReader reader(io_service, "COM3");

        // 运行 io_service 事件循环
        io_service.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
