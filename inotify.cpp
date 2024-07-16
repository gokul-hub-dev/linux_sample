#include <iostream>
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>
#include <cstring>
#include <csignal>

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

volatile sig_atomic_t stop = 0;

void handle_signal(int signal) {
    stop = 1;
}

int main() {
    // Directory to be monitored
    const char *path_to_watch = "/home/ubuntu1604/gokul_work_space/underStandTestharnessAPI/";

    // Register signal handler for SIGINT (Ctrl+C)
    std::signal(SIGINT, handle_signal);

    // Initialize inotify
    int inotify_fd = inotify_init();
    if (inotify_fd == -1) {
        std::cerr << "Error initializing inotify: " << strerror(errno) << std::endl;
        return 1;
    }

    // Add watch to the specified directory
    int watch_descriptor = inotify_add_watch(inotify_fd, path_to_watch, IN_CLOSE_WRITE);
    if (watch_descriptor == -1) {
        std::cerr << "Error adding watch: " << strerror(errno) << std::endl;
        close(inotify_fd);
        return 1;
    }

    // Buffer to store event data
    char buffer[BUF_LEN];

    std::cout << "Monitoring directory: " << path_to_watch << std::endl;
    std::cout << "Press Ctrl+C to stop monitoring..." << std::endl;

    while (!stop) {
        // Read events
        int length = read(inotify_fd, buffer, BUF_LEN);
        if (length == -1 && errno != EAGAIN) {
            std::cerr << "Error reading from inotify file descriptor: " << strerror(errno) << std::endl;
            break;
        }

        if (length <= 0) {
            continue;
        }

        // Process each event
        for (char *ptr = buffer; ptr < buffer + length; ) {
            struct inotify_event *event = (struct inotify_event *) ptr;

            if (event->mask & IN_CLOSE_WRITE) {
                if (event->len > 0) {
                    std::cout << "New file detected: " << event->name << std::endl;
                }
            }

            ptr += sizeof(struct inotify_event) + event->len;
        }
    }

    // Clean up
    inotify_rm_watch(inotify_fd, watch_descriptor);
    close(inotify_fd);

    std::cout << "Stopped monitoring." << std::endl;

    return 0;
}


/*
Initialization:
The program initializes variables and constants required for inotify functionality.
Defines the buffer size (BUF_LEN) to read inotify events.

Inotify Setup:
inotify_init() initializes an inotify instance (inotify_fd).
inotify_add_watch() adds a watch to the specified directory (path_to_watch) for events like file close write (IN_CLOSE_WRITE).
If any errors occur during initialization or watch addition, appropriate error messages are displayed.

Event Processing Loop:
Enters an infinite loop (while (true)) to continuously monitor events unless a termination condition (stop flag) is met.
read(inotify_fd, buffer, BUF_LEN) reads events from the inotify file descriptor (inotify_fd).
Each event is processed:
Checks if the event is due to a file being closed after writing (IN_CLOSE_WRITE).
Retrieves the name of the file associated with the event (event->name) and prints a message indicating a new file was detected.

Signal Handling:
Registers a signal handler (handle_signal) to catch SIGINT (Ctrl+C) signals.
When a SIGINT signal is received, it sets the stop flag to exit the loop and terminate the program gracefully.

Cleanup:
Removes the watch (inotify_rm_watch) and closes the inotify file descriptor (close(inotify_fd)) to release system resources.

Execution:
The program continuously monitors the specified directory for new files being written and outputs messages when new files are detected.
It can be terminated by pressing Ctrl+C, which triggers the signal handler to stop the monitoring loop and clean up resources.

*/
