Reverse Echo Server
============
---

## Setup
After typing make into command prompt it will compile both server.c and client.c and to run the program type ./server [port number] and type ./client [csci-gnode the server is running on] [port number].

My execution environment is Windows 11 Home, Version 22H2, OS Build 22621.1413.
---

## Usage
This program connects a socket server and a client accross two UCDenver CSCI gnodes and take a message as user input, reverses the message in the server, and sends it back to the client.

When the client sends the message "fin" the server will reverse the message and send it back, then close the socket.