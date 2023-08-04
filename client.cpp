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