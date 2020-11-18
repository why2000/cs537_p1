# CS537 P1 (537ps)

## Claiming:

* **This project repo will be make public only AFTER the assignment is due**

## Grade: 11/10

## Author:

|Name|CSlogin|NetID|
|----|-------|-----|
|Hanyuan Wu|CENSORED|CENSORED|
|Zhihao Shu|CENSORED|CENSORED|

## Install

* `bash> cd cs537_p1`
* `bash> make`

## Usage

* ./537ps [-p \<pid\>] [-s[-]] [-U[-]] [-S[-]] [-v[-]] [-c[-]] [-m \<addr\> \<len\>]

## Brief Summary

* A mini c-based project partly implementing the function of linux `ps` command, with some different features.

## Program Features

* `-p <pid>` (multiple `-p <pid>` supported) Display process information only for the processes whose number is `<pid>`. Each `-p <pid>` means one more process (if exists) to display.
* `-s[-]` Display the single-character state information about each process. Defaultly disabled.`-s` to enable.
* `-U[-]` Display the amount of user time consumed by each process. Defaultly enabled.`-U-` to disable.
* `-S[-]` isplay the amount of system time consumed so far by each process. Defaultly disabled.`-S` to enable.
* `-v[-]` Display the amount of virtual memory currently being used (in pages) by each program. Defaultly disabled.`-v` to enable.
* `-c[-]` Display the command-line that started each program. Defaultly enabled.`-c-` to disable.
* `-m <addr> <len>` Display the contents of a process's memory, starting at hexidecimal address `<addr>` and displaying decimal `<len>` bytes of information.

