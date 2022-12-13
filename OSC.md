# OSC Message signatures for this project

In this document, we detail the signature of the OSC messages we use for this project.
It contains examples with the `oscsend` utilities. (part of liblo-utils)

For each entry, there is a short description of the behaviour it triggers, and then an example.
The details of the arguments, if necessary, is written after the description.
We keep this documentation short, so that it's easy to change, if needed.

## Dataviz

Here are the OSC messages that the mpop-dataviz software can receive.

The mpop_dataviz software can receive Open Sound Control messages.
That is how the mpop_kiosk controls it.
See http://opensoundcontrol.org/spec-1_0 for more details about OSC.

The type tags for the arguments we use are: sfi - that stands for string, float32 and int32.

The general idea is that the OSC addresses it support follow the pattern `/dataviz/<window>/<method>` where `<window>` is the window index, and `<method>` is the RPC-style method to call.
If there are more than one window in the same instance, (process) windows have indices.
They are numbered from 1 to `N`, where `N` is the total number of windows.
If there is more than one instance running, they should use a different port number.
We currently use one mpop_dataviz process per window, so the index is alway 0.
What varies is the OSC port it listens on.
This document doesn't involve mention the OSC port numbers per se.
That is a concern for the devops.

Note that each message can be sent multiple times. Once we are in a layout,
there can be new values that are sent, so that it updates the values shown.

Here are the OSC methods of mpop-dataviz:

## my_answer

Shows only one red row.

```
/dataviz/0/my_answer i 10
```

The first time it's received, the dataviz goes into that layout.
Next, a new value can be sent to it, so that it updates the value it shows.


## my_answers

Shows multiple red rows.

```
/dataviz/0/my_answers iiiii 10 20 30 40 50
```

One int for each red row.

The first time it's received, the dataviz goes into that layout.
Next, a new set of values can be sent to it, so that it updates the values it shows.


## goto_screensaver

```
/dataviz/0/goto_screensaver
```

Sends all bars to the screensaver mode.


## view_answer_by_age

Shows the answer to a single question, with one row for each
```
/dataviz/0/view_answer_by_age iiiiiiiiiiiiiiiiiiiii 99 21 5 10 15 20 25 30 35 40 45 50 55 60 65 70 75 80 85 90 95 100
```

The first argument is my answer.
The second argument is the index of the row in which my age is. Example: If I am 21 years old, the index is 4. ([0,5], [5,10], [11,19], [20,25], etc.)
Finally, the value for each of the twenty (20) rows.

Total: 22 integer arguments.


## view_answer_by_gender

```
/dataviz/0/view_answer_by_gender iisisisi "Hommes" 10 "Femmes" 20 "Autres" 30
```

- First arg: my gender index.
- Secong arg: my answer

Then, each pair is a string and int. (the gender name, the value)


## view_answer_by_culture

```
/dataviz/0/view_answer_by_culture iiisisisisisi 5 2 50 "Québécoise" 50 "Canadienne" 50 "Autochtone" 50 "Européenne" 50 "Autre" 50
```

The first int is: how many cultures to show. (it should always be 5)

The 2n and 3rd int is:

- my culture index - starting at 0
- and my answer.

The first number is how many rows to show


## screensaver_set_param

Sets the value of a parameter for the screensaver.

```
/dataviz/0/screensaver_set_param sf speed_ratio 1.0
```

Default is one.

- speed_ratio:: multiplier for the speed of each bar 
- radius_ratio: multiplier for the radius of each bar

A speed ratio of 1.0 is normal. You can speed it up or slow it down by setting it to another value.

A radius ratio of 1.0 is the default. You can make the radius of all bars bigger or smaller by changing that.

## set_min_max_labels

Sets the min and max labels for the dataviz.

```
/dataviz/0/set_min_max_labels "0%" "100%"
```

Arguments:
- min label
- max label

