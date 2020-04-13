#pragma once
#include <Qt>

/**
 * @brief Configuration options for this application.
 */
struct ApplicationOptions {
public:
    bool verbose = false;
    int window_width = 1920;
    int window_height = 1080;
    int window_x = 0;
    int window_y = 0;
    int num_windows = 1;
    quint16 osc_receive_port = 31337;
    bool hide_cursor = false;
};
