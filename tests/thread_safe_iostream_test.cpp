/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   thread_safe_iostream_test.cpp                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: bfranco <bfranco@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/27 10:01:22 by bfranco       #+#    #+#                 */
/*   Updated: 2025/07/27 20:56:28 by bfranco       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <mutex>
#include <stdexcept>
#include "libftpp.hpp"

void assert(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}

class CoutCapture {
        std::ostringstream& _target;
        std::streambuf* _old;
    public:
        CoutCapture(std::ostringstream& target) : _target(target) {
            _old = std::cout.rdbuf(_target.rdbuf());
        }
        ~CoutCapture() {
            std::cout.rdbuf(_old);
        }
};

using TSIO = ThreadSafeIOStream;

int main() {
    std::ostringstream capture;
    std::mutex captureMutex;

    // Main thread output
    {
        CoutCapture redirect(capture);
        TSIO::threadSafeCout << "Hello world" << std::endl;
    }

    assert(capture.str().find("[Main] Hello world") != std::string::npos,
        "Prefix or output missing from threadSafeCout");
    TSIO::threadSafeCout << "Captured output from threads:\n" << capture.str() << std::endl;
    
    capture.str("");
    capture.clear();

    // Thread-local prefix test (safe capture)
    Thread t1("T1", [&]() {
        std::lock_guard<std::mutex> lock(captureMutex);
        CoutCapture redirect(capture);
        TSIO::threadSafeCout << "Message from T1" << std::endl;
    });

    Thread t2("T2", [&]() {
        std::lock_guard<std::mutex> lock(captureMutex);
        CoutCapture redirect(capture);
        TSIO::threadSafeCout << "Message from T2" << std::endl;
    });

    t1.start(); t2.start();
    t1.stop();  t2.stop();

    assert(capture.str().find("[T1] Message from T1") != std::string::npos,
        "Missing output from T1");
    assert(capture.str().find("[T2] Message from T2") != std::string::npos,
        "Missing output from T2");
        
    TSIO::threadSafeCout << "Captured output from threads:\n" << capture.str() << std::endl;

    capture.str("");
    capture.clear();

    // Concatenation test
    Thread t3("Multi", [&]() {
        std::lock_guard<std::mutex> lock(captureMutex);
        CoutCapture redirect(capture);
        TSIO::threadSafeCout << "Part1 " << 123 << " Part2" << std::endl;
    });

    t3.start();
    t3.stop();

    assert(capture.str().find("[Multi] Part1 123 Part2") != std::string::npos,
        "Concatenated stream failed");
    
    TSIO::threadSafeCout << "Captured output from threads:\n" << capture.str() << std::endl;

    capture.str("");
    capture.clear();

    // Deferred flush test
    Thread t4("FlushTest", [&]() {
        std::lock_guard<std::mutex> lock(captureMutex);
        CoutCapture redirect(capture);
        TSIO::threadSafeCout << "Pending...";
        assert(capture.str().empty(), "Flush occurred without std::endl");
        TSIO::threadSafeCout << std::endl;
    });
    
    t4.start();
    t4.stop();
    
    assert(capture.str().find("[FlushTest] Pending...") != std::string::npos,
        "Flush with std::endl failed");
    TSIO::threadSafeCout << "Captured output from threads:\n" << capture.str() << std::endl;

    return 0;
}
