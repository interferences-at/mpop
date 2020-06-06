# OSC Message signatures for this project

In this document, we detail the signature of the OSC messages we use for this project.

For each entry, there is a short description of the behaviour it triggers, and then an example.
The details of the arguments, if necessary, is written after the description.
We keep this documentation short, so that it's easy to change, if needed.

The mpop_dataviz software can receive Open Sound Control messages.
That is how the mpop_kiosk controls it.
See http://opensoundcontrol.org/spec-1_0 for more details about OSC.

The type tags for the arguments we use are: sfi - that stands for string, float32 and int32.

Generally, the beginning of the path is /dataviz/N/ where N is the index of the window.
We currently use one mpop_dataviz process per window, so the index is alway 0.
What varies is the OSC port it listens on.
This document doesn't involve mention the OSC port numbers per se.
That is a concern for the devops.

Note that each message can be sent multiple times. Once we are in a layout,
there can be new values that are sent, so that it updates the values shown.


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


## screensaver

```
/dataviz/0/screensaver
```

Sends all bars to the screensaver mode.


## view_answer/by_age

Shows the answer to a single question, with one row for each
```
/dataviz/0/view_answer/by_age iiiiiiiiiiiiiiiiiiiii 99 21 5 10 15 20 25 30 35 40 45 50 55 60 65 70 75 80 85 90 95 100
```

The first argument is my answer.
The second argument is my age.
Finally, the value for each of the twenty (20) rows.

Total: 22 integer arguments.


## view_answer/by_age

```
/dataviz/0/view_answer/by_gender sisisi "Hommes" 10 "Femmes" 20 "Autres" 30

```

## view_answers

```
/dataviz/0/view_answers isiisiisiisiisii 5 "Les victimes" 50 50 "Les personnes marginalisées" 50 50 "Les jeunes contrevenants" 50 50 "Les gens fortunés" 50 50 "Les minorités visibles ou culturelles" 50 50
```

The first int is: how many answers to show. (it should always be 5)

Then, for each culture, there is:

- the text (s)
- the average (i)
- my answer (i)


## by_culture

```
/dataviz/0/view_answer/by_culture iiisisisisisisi 6 2 50 "Québécoise" 50 "Canadienne" 50 "Autochtone" 50 "Européenne" 50 "Autre" 50
```

The first int is: how many cultures to show. (it should always be 6)

The 2n and 3rd int is:

- my culture index - starting at 0
- and my answer.

The first number is how many rows to show


## screensaver/speed_ratio

Sets the speed ratio for the screensaver.

A ratio of 1.0 is normal. You can speed it up or slow it down by setting it to another value.

```
/dataviz/0/screensaver/speed_ratio f 1.0
```


# screensaver/radius_ratio

Sets the radis ratio for the screensaver.

```
/dataviz/0/screensaver/radius_ratio f 1.0
```

A ratio of 1.0 is normal. You can speed it up or slow it down by setting it to another value.
