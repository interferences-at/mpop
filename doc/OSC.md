# OSC Messages

This document describes the OSC messages for the MPOP system.

It contains examples with the `oscsend` utilities. (part of liblo-utils)

## Dataviz

Here are the OSC messages that the mpop-dataviz software can receive.

The general idea is that the OSC addresses it support follow the pattern `/dataviz/<window>/<method>` where `<window>` is the window index, and `<method>` is the RPC-style method to call.
If there are more than one window in the same instance, (process) windows have indices.
They are numbered from 1 to `N`, where `N` is the total number of windows.
If there is more than one instance running, they should use a different port number.

Here are the OSC methods of mpop-dataviz:

## /dataviz/<n>/varchart iii

The barchart method:

`/dataviz/1/barchart iii 70 20 10`

The number of integer argument can vary.

Example:

```
oscsend osc.udp://localhost:31337 /dataviz/1/barchart iii 70 20 10
```

