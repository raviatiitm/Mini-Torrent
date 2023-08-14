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

Ex:
127.0.0.1
5000
127.0.0.1
6000
Close Tracker:
quit
Client:
Run Client:
cd client
./client​ <IP>:<PORT> <TRACKER INFO FILE>
ex: ./client 127.0.0.1:18000 tracker_info.txt
Create user account:
create_user​ <user_id> <password>
Login:
login​ <user_id> <password>
Create Group:
create_group​ <group_id>
Join Group:
join_group​ <group_id>
Leave Group:
leave_group​ <group_id>
List pending requests:
list_requests ​<group_id>
Accept Group Joining Request:
accept_request​ <group_id> <user_id>
List All Group In Network:
list_groups
List All sharable Files In Group:
list_files​ <group_id>
Upload File:
​upload_file​ <file_path> <group_id​>
Download File:​
download_file​ <group_id> <file_name> <destination_path>
Logout:​
logout
Show_downloads: ​
show_downloads
Stop sharing: ​
stop_share ​<group_id> <file_name>
Working
User should create an account and register with tracker.
Login using the user credentials.
Tracker maintains information of clients with their files(shared by client) to assist the clients for the communication between peers.
User can create Group and hence will become admin of that group.
User can fetch list of all Groups in server.
User can join/leave group.
Group admin can accept group join requests.
Share file across group: Shares the filename and SHA1 hash of the complete file as well as piecewise SHA1 with the tracker.
Fetch list of all sharable files in a Group.
Download:
Retrieve peer information from tracker for the file.
Download file from multiple peers (different pieces of file from different peers - ​piece selection algorithm​) simultaneously and all the files which client downloads will be shareable to other users in the same group. File integrity is ensured using SHA1 comparison.
Piece selection algorithm used: Selects random piece and then downloads it from a random peer having that piece.
Show downloads.
Stop sharing file.
Logout - stops sharing all files.
Whenever client logins, all previously shared files before logout should automatically be on sharing mode.
Assumptions
Only one tracker is implemented and that tracker should always be online.
The peer can login from different IP addresses, but the details of his downloads/uploads will not be persistent across sessions.
SHA1 integrity checking doesn't work correctly for binary files, even though in most likelihood the file would have downloaded correctly.
File paths should be absolute.