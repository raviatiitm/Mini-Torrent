#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <bits/stdc++.h>
#include <pthread.h>
using namespace std;

pthread_t readerthreads;

struct users
{
    string UID, IP_PORT, PASS;
    bool LoginStatus = false;
};

struct groups
{
    string UID, GID;
    set<string> members;
    set<string> pending_req;
    set<string> file_names;
};

struct files
{
    string GID, file_name, file_path;
    set<pair<string, string>> seeders;
    long long file_size;
};

unordered_map<string, users> user_list;
unordered_map<string, groups> group_list;
unordered_map<string, files> file_list;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}