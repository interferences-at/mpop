import QtQuick 2.0

/**
 * Configuration options for this test suite.
 */
Item {
    /**
     * Parses the command-line arguments to populate the configuration options.
     */
    function parseCommandLineOptions() {
        var args = Qt.application.arguments;
        console.log(args);
        switch (args.length) {
            // eslint-disable-next-line no-fallthrough
        case 4:
            is_verbose = true;
            // eslint-disable-next-line no-fallthrough
        case 3:
            service_port = parseInt(args[const_ARGUMENT_INDEX_PORT]);
            // eslint-disable-next-line no-fallthrough
        case 2:
            service_host = args[const_ARGUMENT_INDEX_HOST];
            // eslint-disable-next-line no-fallthrough
        case 1:
            break;
        }
    }

    property int service_port_number: 3333
    property string service_host: "0.0.0.0"
    property bool is_verbose: false

    readonly property int const_ARGUMENT_INDEX_HOST: 1
    readonly property int const_ARGUMENT_INDEX_PORT: 2
    readonly property int const_ARGUMENT_INDEX_VERBOSE: 3

}
