#pragma once
#include <Qt>
#include <QDebug>

/**
 * @brief Configuration options for this application.
 */
struct ApplicationOptions {
public: // public data members
    bool verbose = false;
    bool show_window_frame = false;
    bool align_right = false;
    int window_width = 1920;
    int window_height = 1080;
    int contents_width = 1260;
    int window_x = 0;
    int window_y = 0;
    int screen = -1;
    quint16 osc_receive_port = 31337;
    bool show_cursor = false;

public: // methods
    void printAll() {
        qDebug() <<
                    "verbose" << verbose <<
                    "show_window_frame" << show_window_frame <<
                    "window_height" << window_height <<
                    "window_width" << window_width <<
                    "window_x" << window_x <<
                    "window_y" << window_y <<
                    "osc_receive_port" << osc_receive_port <<
                    "show_cursor" << show_cursor;
    }
};
