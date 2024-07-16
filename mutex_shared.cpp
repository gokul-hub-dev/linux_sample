#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <chrono>

std::queue<std::string> msgQueue;
std::mutex msgQueueMutex;
std::condition_variable msgCv;

/////////queue - FIFO////////////////
/* 
producer:
lock
update or push queue
notify using condition_variable

std::lock_guard is a RAII (Resource Acquisition Is Initialization) class that simplifies mutex locking. When you create a std::lock_guard object, it locks the mutex, and when the std::lock_guard object goes out of scope (i.e., the end of the block {}), it automatically unlocks the mutex.
*/
void producer() {
    for (int i = 0; i < 5; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work
        std::string message = "Message " + std::to_string(i);

        {
            std::lock_guard<std::mutex> lock(msgQueueMutex);
            msgQueue.push(message);
            std::cout << "Produced: " << message << std::endl;
        }

        msgCv.notify_one(); // Notify the consumer that a new message is available
    }
}
/*
consumer:
wait for notify using conditional_variable, once received notify from producer
read the msg from queue
pop the msg
unlock mutex
*/
void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(msgQueueMutex);
        msgCv.wait(lock, []{ return !msgQueue.empty(); });

        std::string message = msgQueue.front();
        msgQueue.pop();
        lock.unlock(); // Unlock the mutex before processing the message

        std::cout << "Consumed: " << message << std::endl;

        if (message == "Message 4") {
            break; // Exit the loop after processing the last message
        }
    }
}

int main() {
    std::thread producerThread(producer);
    std::thread consumerThread(consumer);

    producerThread.join();
    consumerThread.join();

    return 0;
}

/*
Global Variables:
msgQueue: A queue to hold messages.
msgQueueMutex: A mutex to protect access to the queue.
msgCv: A condition variable for synchronizing the producer and consumer threads.

Producer Function:
Simulates work by sleeping for 1 second.
Creates a message and pushes it onto the queue.
Notifies the consumer that a new message is available using msgCv.notify_one().

Consumer Function:
Waits for a message to be available on the queue using msgCv.wait().
Processes the message by popping it from the queue and printing it.
Exits the loop after processing the last message.

Main Function:
Creates and starts the producer and consumer threads.
Joins the threads to wait for them to finish before exiting the program.
*/
