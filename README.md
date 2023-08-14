# Mini-Torrent
The Mini-torrent project is basically a peer-to-peer file sharing network. The user has functionalities like sharing a file, downloading a file, removing a file from sharing etc. It is basically like the Bit-Torrent we have on the internet.

The architecture is such that there are multiple clients(users) and a tracker which stores the metadata of which all users have a file, basically a file-user mapping.

image

Prerequisites
Software Requirement

G++ compiler

To install G++ : sudo apt-get install g++
OpenSSL library

To install OpenSSL library : sudo apt-get install openssl
Platform: Linux

Installation
1. cd client
2. make
3. cd ../tracker
5. make
6. cd ..
Usage
Tracker
Run Tracker:
cd tracker
./tracker​ <TRACKER INFO FILE> <TRACKER NUMBER>
ex: ./tracker tracker_info.txt 1
<TRACKER INFO FILE> contains the IP, Port details of all the trackers.
