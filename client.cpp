#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <thread>
#include <netdb.h>
#include <pthread.h>
#include<openssl/sha.h>
using namespace std;

string IP_PORT, IP, IP_SERVER;
int PORT, sockfd;
string UID = "not set";
string PASS = "not set";
bool loginStatus = false;
vector<pthread_t> tids;
vector<pair<string, string>> downloadedfiles;

int TRACKER_PORT;
string TRACKER_IP;

string SHA1Hash(int chnkSize, char *data)
{

    unsigned char messageDigest[20];

    char buf[40];

    bzero(buf, sizeof(buf));

    bzero(messageDigest, sizeof(messageDigest));

    SHA1((unsigned char *)data, chnkSize, messageDigest);
    int i=0;
    while (i < 20)

    {
        sprintf(&(buf[i * 2]), "%02x", messageDigest[i]);
        i++;
    }

    return string(buf);
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void create_user(string buffer, string id, string pass)
{
    string r = buffer.substr(0, buffer.size() - 1);
    buffer = r + " " + IP_PORT;
    int n;
    n = write(sockfd, buffer.c_str(), strlen(buffer.c_str()));
    if (n < 0)
        error("ERROR writing to socket");
    char buff[256];
    bzero(buff, 256);
    n = read(sockfd, buff, 255);
    if (n < 0)
        error("ERROR reading from socket");
    if (strcmp(buff, "user created successfully") == 0)
    {
        UID = id;
        PASS = pass;
        bool loginStatus = false;
        cout << "User Created" << endl;
    }
    else if (strcmp(buff, "user already exist") == 0)
    {
        cout << "user already exist" << endl;
    }
}void login(string buffer)
{
    buffer = buffer.substr(0, buffer.size() - 1);
    int n;
    n = write(sockfd, buffer.c_str(), strlen(buffer.c_str()));
    if (n < 0)
        error("ERROR writing to socket");
    char buff[256];
    bzero(buff, 256);
    n = read(sockfd, buff, 255);
    if (n < 0)
        error("ERROR reading from socket");
    if (strcmp(buff, "Login Successful") == 0)
    {
        cout << "Login Successful" << endl;
    }
    else if (strcmp(buff, "Already logged in") == 0)
    {
        cout << "Already logged in" << endl;
    }
    else if (strcmp(buff, "Wrong Password") == 0)
    {
        cout << "Wrong Password" << endl;
    }
    else
    {
        cout << "User is not registered" << endl;
    }
}

void logout(string buffer)
{
    string r = buffer.substr(0, buffer.size() - 1);
    buffer = r + " " + UID;
    cout << buffer << endl;
    int n;
    n = write(sockfd, buffer.c_str(), strlen(buffer.c_str()));
    if (n < 0)
        error("ERROR writing to socket");
    char buff[256];
    bzero(buff, 255);
    n = read(sockfd, buff, 255);
    if (n < 0)
        error("ERROR reading from socket");
    if (strcmp(buff, "Logout Successful") == 0)
    {
        cout << "Logout Successful" << endl;
    }
    else if (strcmp(buff, "Already logged out") == 0)
    {
        cout << "Already logged out" << endl;
    }
    else
    {
        cout << "User is not registered" << endl;
    }
}
void create_group(string buffer, string gid)
{
    string r = buffer.substr(0, buffer.size() - 1);
    buffer = r + " " + UID;
    int n;
    n = write(sockfd, buffer.c_str(), strlen(buffer.c_str()));
    if (n < 0)
        error("ERROR writing to socket");
    char buff[256];
    bzero(buff, 256);
    n = read(sockfd, buff, 255);
    if (n < 0)
        error("ERROR reading from socket");
    if (strcmp(buff, "Group created sucessfully.") == 0)
    {
        cout << "Group created sucessfully." << endl;
    }
    else if (strcmp(buff, "Group already exist.") == 0)
    {
        cout << "Group already exist." << endl;
    }
    else
    {
        cout << "Login First!" << endl;
    }
}

void join_group(string buffer)
{
    string r = buffer.substr(0, buffer.size() - 1);
    buffer = r + " " + UID;
    int n;
    n = write(sockfd, buffer.c_str(), strlen(buffer.c_str()));
    if (n < 0)
        error("ERROR writing to socket");
    char buff[256];
    bzero(buff, 256);
    n = read(sockfd, buff, 255);
    if (n < 0)
        error("ERROR reading from socket");
    if (strcmp(buff, "You're already a member of this group.") == 0)
    {
        cout << "You're already a member of this group." << endl;
    }
    else if (strcmp(buff, "Your joining request is pending.") == 0)
    {
        cout << "Your joining request is pending." << endl;
    }
    else if (strcmp(buff, "Group joining request Sent.") == 0)
    {
        cout << "Group joining request Sent." << endl;
    }
    else if (strcmp(buff, "Login First!") == 0)
    {
        cout << "Login First!" << endl;
    }
    else if (strcmp(buff, "You're the Admin of this group.") == 0)
    {
        cout << "You're the Admin of this group." << endl;
    }
    else
    {
        cout << "Group Doesn't exist." << endl;
    }
}

void leave_group(string buffer)
{
    string r = buffer.substr(0, buffer.size() - 1);
    buffer = r + " " + UID;
    int n;
    n = write(sockfd, buffer.c_str(), strlen(buffer.c_str()));
    if (n < 0)
        error("ERROR writing to socket");
    char buff[256];
    bzero(buff, 256);
    n = read(sockfd, buff, 255);
    if (n < 0)
        error("ERROR reading from socket");
    if (strcmp(buff, "You've left the group.") == 0)
    {
        cout << "You've left the group." << endl;
    }
    else if (strcmp(buff, "You're not present in this group.") == 0)
    {
        cout << "You're not present in this group." << endl;
    }
    else if (strcmp(buff, "Group Doesn't exist.") == 0)
    {
        cout << "Group Doesn't exist." << endl;
    }
    else
    {
        cout << "Login First!" << endl;
    }
}

void pending_list_request(string buffer)
{
    string r = buffer.substr(0, buffer.size() - 1);
    buffer = r + " " + UID;
    int n;
    n = write(sockfd, buffer.c_str(), strlen(buffer.c_str()));
    if (n < 0)
        error("ERROR writing to socket");
    char buff[256];
    bzero(buff, 256);
    n = read(sockfd, buff, 255);
    if (n < 0)
        error("ERROR reading from socket");
    if (strcmp(buff, "No pending requests.") == 0)
    {
        cout << "No pending requests." << endl;
    }
    else if (strcmp(buff, "You are not Admin of this group.") == 0)
    {
        cout << "You are not Admin of this group." << endl;
    }
    else if (strcmp(buff, "Group Doesn't exist.") == 0)
    {
        cout << "Group Doesn't exist." << endl;
    }
    else if (strcmp(buff, "Login First!") == 0)
    {
        cout << "Login First!" << endl;
    }
    else
    {
        cout << buff << endl;
    }
}

void accept_request(string buffer)
{
    string r = buffer.substr(0, buffer.size() - 1);
    buffer = r + " " + UID;
    int n;
    n = write(sockfd, buffer.c_str(), strlen(buffer.c_str()));
    if (n < 0)
        error("ERROR writing to socket");
    char buff[256];
    bzero(buff, 256);
    n = read(sockfd, buff, 255);
    if (n < 0)
        error("ERROR reading from socket");
    if (strcmp(buff, "No pending requests.") == 0)
    {
        cout << "No pending requests." << endl;
    }
    else if (strcmp(buff, "You are not Admin of this group.") == 0)
    {
        cout << "You are not Admin of this group." << endl;
    }
    else if (strcmp(buff, "Group Doesn't exist.") == 0)
    {
        cout << "Group Doesn't exist." << endl;
    }
    else if (strcmp(buff, "Login First!") == 0)
    {
        cout << "Login First!" << endl;
    }
    else
    {
        cout << buff << endl;
    }
}

void list_groups(string buffer)
{
    string r = buffer.substr(0, buffer.size() - 1);
    buffer = r + " " + UID;
    int n;
    n = write(sockfd, buffer.c_str(), strlen(buffer.c_str()));
    if (n < 0)
        error("ERROR writing to socket");
    char buff[256];
    bzero(buff, 256);
    n = read(sockfd, buff, 255);
    if (n < 0)
        error("ERROR reading from socket");
    if (strcmp(buff, "No Group exist.") == 0)
    {
        cout << "No Group exist." << endl;
    }
    else if (strcmp(buff, "Login First!") == 0)
    {
        cout << "Login First!" << endl;
    }
    else
    {
        cout << buff << endl;
    }
}

void list_files(string buffer)
{
    string r = buffer.substr(0, buffer.size() - 1);
    buffer = r + " " + UID;
    int n;
    n = write(sockfd, buffer.c_str(), strlen(buffer.c_str()));
    if (n < 0)
        error("ERROR writing to socket");
    char buff[256];
    bzero(buff, 256);
    n = read(sockfd, buff, 255);
    if (n < 0)
        error("ERROR reading from socket");
    if (strcmp(buff, "Group Doesn't exist.") == 0)
    {
        cout << "Group Doesn't exist." << endl;
    }
    else if (strcmp(buff, "Login First!") == 0)
    {
        cout << "Login First!" << endl;
    }
    else
    {
        cout << buff << endl;
    }
}

void upload_file(string buffer)
{
    string r = buffer.substr(0, buffer.size() - 1);
    buffer = r + " " + UID + " " + IP_PORT;
    int n;
    n = write(sockfd, buffer.c_str(), strlen(buffer.c_str()));
    if (n < 0)
        error("ERROR writing to socket");
    char buff[256];
    bzero(buff, 256);
    n = read(sockfd, buff, 255);
    if (n < 0)
        error("ERROR reading from socket");
    if (strcmp(buff, "You're not a member of this group.") == 0)
    {
        cout << "You're not a member of this group." << endl;
    }
    else if (strcmp(buff, "Login First!") == 0)
    {
        cout << "Login First!" << endl;
    }
    else
    {
        cout << buff << endl;
    }
}

void stop_sharing(string buffer)
{
    string r = buffer.substr(0, buffer.size() - 1);
    buffer = r + " " + UID+" " + IP_PORT;
    int n;
    n = write(sockfd, buffer.c_str(), strlen(buffer.c_str()));
    if (n < 0)
        error("ERROR writing to socket");
    char buff[256];
    bzero(buff, 256);
    n = read(sockfd, buff, 255);
    if (n < 0)
        error("ERROR reading from socket");
    if (strcmp(buff, "You're not a member of this group.") == 0)
    {
        cout << "You're not a member of this group." << endl;
    }
    else if (strcmp(buff, "Login First!") == 0)
    {
        cout << "Login First!" << endl;
    }
    else
    {
        cout << buff << endl;
    }
}

void downloadChunk(string ip, int port, string path, string destination, long long fileSize)
{

    int sockfd, n;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Error Opening socket!");
    struct hostent *server;
    server = gethostbyname(ip.c_str());
    if (server == NULL)
        error("No such host..");

    bzero((char *)&server_addr, sizeof(server_addr));
    string cmd;
    cmd = "get_chunk|";
    cmd += path + "|";
    cmd += to_string(fileSize);
    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        error("Connection failed ! inside downloads");

    ofstream destination_file(destination, ios_base::out | ios_base::binary | ios_base::trunc);
    n = write(sockfd, cmd.c_str(), strlen(cmd.c_str()));
    if (n < 0)
        error("Error on writing!");

    char buff[3114374];
    bzero(buff, 3114374);
    if (n < 0)
        error("Error on reading!");

    long long total_size = fileSize, chunk_no, chunk_size = 512;
    for (long long pos = 0; pos < total_size; pos += chunk_size)
    {
        n = read(sockfd, buff, fileSize);
        destination_file.seekp(pos, ios::beg);

        if (n <= 0)
            break;

        destination_file.write(buff, n);

        write(sockfd, "OK", 2);
    }

    destination_file.close();
}

void download_file(string buffer, string gid, string filename, string destination)
{
    string r = buffer.substr(0, buffer.size() - 1);
    buffer = r + " " + UID;
    int n;
    n = write(sockfd, buffer.c_str(), strlen(buffer.c_str()));
    if (n < 0)
        error("ERROR writing to socket");
    char buff[256];
    bzero(buff, 256);
    n = read(sockfd, buff, 255);
    if (n < 0)
        error("ERROR reading from socket");
    if (strcmp(buff, "You're not a member of this group.") == 0)
    {
        cout << "You're not a member of this group." << endl;
    }
    else if (strcmp(buff, "Login First!") == 0)
    {
        cout << "Login First!" << endl;
    }
    else if (strcmp(buff, "No such file exist!") == 0)
    {
        cout << "No such file exist!" << endl;
    }
    else if (strcmp(buff, "Group Doesn't exist.") == 0)
    {
        cout << "Group Doesn't exist." << endl;
    }
    else if (strcmp(buff, "No seeder available") == 0)
    {
        cout << "No seeder available" << endl;
    }
    else if (strcmp(buff, "Currently all seeders are offline.") == 0)
    {
        cout << "Currently all seeders are offline." << endl;
    }
    else
    {
        string r = buff;
        vector<string> seederList;
        vector<string> seederIP;
        vector<int> seederPort;
        stringstream ss(r);
        while (ss.good())
        {
            string sb;
            getline(ss, sb, ' ');
            seederList.push_back(sb);
        }
        long long FILE_SIZE = stoll(seederList[1]);
        int NO_OF_CHUNKS = (FILE_SIZE) / 524288;
        string filePath = seederList[2];
        string seedAddr;
        for (int i = 3; i < seederList.size(); i++)
        {

            seedAddr = seederList[i];
            vector<string> sda;
            stringstream ss1(seedAddr);
            while (ss1.good())
            {
                string sb1;
                getline(ss1, sb1, ':');
                sda.push_back(sb1);
            }
            auto sIP = sda[0];
            auto sP = sda[1];
            seederIP.push_back(sIP);
            seederPort.push_back(stoi(sP));
        }
        if (NO_OF_CHUNKS < 1)
        {
            NO_OF_CHUNKS = 1;
        }

        cout << "\nSeeder List\n";
        for (int k = 0; k < seederIP.size(); k++)
        {
            cout << k + 1 << ". IP : " << seederIP[k] << " | Port : " << seederPort[k] << endl;
        }
        string sip = seederIP[0];
        int sport = seederPort[0];
        cout << endl;
        cout << "No. of chunks created : " << NO_OF_CHUNKS << endl;

        downloadChunk(sip, sport, filePath, destination, FILE_SIZE);
        downloadedfiles.push_back({gid, filename});
        cout << "\nFile Downloaded Successfully." << endl;
        // upload_file( "upload_file "+filePath+" <group_id >/");
        cout << endl;
    }
}

void ConnectTracker()
{
    int portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    portno = TRACKER_PORT;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(IP_SERVER.c_str());
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");
}

void request_for_download(int newsockfd, int port)
{
    string ip = IP_SERVER;

    while (true)
    {
        char buffer[512];
        string sb;
        vector<string> tokens;
        bzero(buffer, 512);
        int n = read(newsockfd, buffer, 512);
        if (n < 0)
            error("Error on reading!");
        string command = buffer;
        stringstream ss(command);
        while (ss.good())
        {
            getline(ss, sb, '|');
            tokens.push_back(sb);
        }
        string cmd = tokens[0];

        if (cmd == "get_chunk")
        {
            string path = tokens[1];
            long long fileSize = stoll(tokens[2]);

            ifstream source_file(path, ifstream::binary);

            long long pos;
            int total_size = fileSize, chunk_size = 512;
            for (pos = 0; pos < (total_size - (total_size % chunk_size)); pos += chunk_size)
            {
                int idx = 0;

                source_file.seekg(pos, ios::beg);
                source_file.read(buffer, chunk_size);

                write(newsockfd, buffer, chunk_size);

                bzero(buffer, 512);
                if (read(newsockfd, buffer, chunk_size) <= 0)
                    perror("ERROR reading from socket");

                if (strcmp(buffer, "OK") != 0)
                {
                    break;
                }
                bzero(buffer, chunk_size);
            }
            source_file.seekg(pos, ios::beg);
            source_file.read(buffer, total_size % chunk_size);
            write(newsockfd, buffer, total_size % chunk_size);
            if (read(newsockfd, buffer, chunk_size) <= 0)
                perror("ERROR reading from socket");
            break;
        }
    }

    return;
}

void *init_server(void *param)
{
    int sockfd;
    struct sockaddr_in server_addr, client_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Error Opening socket!");

    bzero((char *)&server_addr, sizeof(server_addr));
    socklen_t client_len;
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, IP.c_str(), &server_addr.sin_addr);
    int newsockfd;
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        error("Binding failed!");

    listen(sockfd, 10);
    
    client_len = sizeof(client_addr);
    
    while (true)
    {
        newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
        if (newsockfd < 0)
            break;
        int port;
        port = (ntohs(client_addr.sin_port));
        request_for_download(newsockfd, port);
    }
    return NULL;
}