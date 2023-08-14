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
string extractcmd(string buffer)
{
    vector<string> tokens;
    stringstream ss(buffer);
    while (ss.good())
    {
        string sb;
        getline(ss, sb, ' ');
        tokens.push_back(sb);
    }
    string command = tokens[0];

    if (command == "create_user")
    {
        if (user_list.find(tokens[1]) == user_list.end())
        {
            struct users peer;
            string uid = tokens[1];
            string pass = tokens[2];
            string ip_port = tokens[3];
            peer.UID = uid;
            peer.PASS = pass;
            peer.IP_PORT = ip_port;
            user_list[uid] = peer;
            return "user created successfully";
        }
        else
            return "user already exist";
    }

    else if (command == "login")
    {
        // cout << tokens[2] << " " << user_list[tokens[1]].PASS << endl;

        if (user_list.find(tokens[1]) == user_list.end())
            return "User not registered";
        else
        {
            if (user_list[tokens[1]].LoginStatus == true)
                return "Already logged in";
            else if (user_list[tokens[1]].PASS == tokens[2])
            {
                user_list[tokens[1]].LoginStatus = true;
                return "Login Successful";
            }
            else
                return "Wrong Password";
        }
    }

    else if (command == "logout")
    {
        // cout << "Inside logout" << endl;
        if (user_list.find(tokens[1]) == user_list.end())
            return "User not registered";
        else
        {
            if (user_list[tokens[1]].LoginStatus == false)
                return "Already logged out";
            else
                user_list[tokens[1]].LoginStatus = false;
            return "Logout Successful";
        }
    }

    else if (command == "create_group")
    {

        string gid = tokens[1], uid = tokens[2];
        if (user_list[uid].LoginStatus == false)
        {
            return "Login First!";
        }
        if (group_list.find(gid) != group_list.end())
        {
            return "Group already exist.";
        }
        else
        {
            struct groups gp;
            gp.UID = uid;
            gp.GID = gid;
            gp.members.insert(uid);
            group_list[gid] = gp;
            // admin[gid] = uid;
            return "Group created sucessfully.";
        }
    }
    else if (command == "join_group")
    {
        string gid = tokens[1], uid = tokens[2];
        if (user_list[uid].LoginStatus == false)
        {
            return "Login First!";
        }
        else if (group_list.find(gid) != group_list.end())
        {
            if (group_list[gid].UID == tokens[2])
            {
                return ("You're the Admin of this group.");
            }
            else if (group_list[gid].members.find(uid) != group_list[gid].members.end())
                return ("You're already a member of this group.");
            else if (group_list[gid].pending_req.find(uid) != group_list[gid].pending_req.end())
                return ("Your joining request is pending.");
            group_list[gid].pending_req.insert(uid);
            return ("Group joining request Sent.");
        }
        else
        {
            return ("Group Doesn't exist.");
        }
    }
    else if (command == "leave_group")
    {
        string gid = tokens[1], uid = tokens[2];
        if (user_list[uid].LoginStatus == false)
        {
            return "Login First!";
        }
        else if (group_list.find(gid) != group_list.end())
        {
            if (group_list[tokens[1]].members.find(uid) != group_list[gid].members.end())
            {
                group_list[gid].members.erase(uid);
                return ("You've left the group.");
            }
            else
                return ("You're not present in this group.");
        }
        else
        {
            return ("Group Doesn't exist.");
        }
    }

    else if (command == "requests")
    {
        string gid = tokens[2], uid = tokens[3];
        if (user_list[uid].LoginStatus == false)
        {
            return "Login First!";
        }
        else if (group_list.find(gid) != group_list.end())
        {
            if (group_list[gid].UID == uid)
            {
                if (group_list[gid].pending_req.empty())
                    return ("No pending requests.");
                else
                {
                    string s = "";
                    for (auto id = group_list[gid].pending_req.begin(); id != group_list[gid].pending_req.end(); ++id)
                    {
                        s += "(";
                        s += *id;
                        s += ") ";
                    }
                    return s;
                }
            }
            else
                return ("You are not Admin of this group.");
        }
        else
        {
            return ("Group Doesn't exist.");
        }
    }

    else if (command == "accept_request")
    {
        string gid = tokens[1], uid = tokens[3], peerid = tokens[2];
        if (user_list[uid].LoginStatus == false)
        {
            return "Login First!";
        }
        else if (group_list.find(gid) != group_list.end())
        {
            if (group_list[gid].UID == uid)
            {
                if (group_list[gid].pending_req.find(peerid) == group_list[gid].pending_req.end())
                    return ("No pending requests.");
                else
                {
                    group_list[gid].members.insert(peerid);
                    group_list[gid].pending_req.erase(peerid);
                    string s = "Joining request accepted for " + peerid;

                    return s;
                }
            }
            else
                return ("You are not Admin of this group.");
        }
        else
        {
            return ("Group Doesn't exist.");
        }
    }

    else if (command == "list_groups")
    {
        string uid = tokens[1];
        if (user_list[uid].LoginStatus == false)
        {
            return "Login First!";
        }
        if (group_list.empty())
            return ("No Group exist.");
        else
        {
            string s = "";
            for (auto id : group_list)
            {
                s += "(";
                s += id.first;
                s += ") ";
            }
            return s;
        }
    }

    else if (command == "upload_file")
    {
        string gid = tokens[2], uid = tokens[3], ip_port = tokens[4], path = tokens[1];
        if (user_list[uid].LoginStatus == false)
        {
            return "Login First!";
        }
        else if (group_list.find(gid) != group_list.end())
        {
            if (group_list[gid].members.find(uid) == group_list[gid].members.end())
            {
                return ("You're not a member of this group.");
            }
            else
            {
                //extracting filename
                int i = path.length() - 1;
                while (i >= 0)
                {
                    if (path[i] == '/')
                        break;
                    i--;
                }
                string fileName;
                if (i < path.length() - 1)
                {
                    fileName = path.substr(i + 1, path.length());
                }
                else
                    return "Please provide absolute path.";

                //file size
                ifstream f(path, ios::ate | ios::binary);
                long long fsize = f.tellg();
                f.close();
                struct files newfile;
                newfile.GID = gid;
                newfile.file_size = fsize;
                newfile.file_name = fileName;
                newfile.file_path = path;
                newfile.seeders.insert({uid, ip_port});
                if (file_list.find(fileName) != file_list.end())
                {
                    if (file_list[fileName].seeders.find({uid, ip_port}) != file_list[fileName].seeders.end())
                    {
                        return "U r already a seeder";
                    }
                    else
                    {
                        file_list[fileName].seeders.insert({uid, ip_port});
                        return "One more seeder added for file " + fileName + " in " + gid + ".";
                    }
                }
                else
                {
                    group_list[gid].file_names.insert(fileName);
                    // file_list[fileName].seeders.insert({uid, ip_port});
                    file_list[fileName] = newfile;
                    return "New file uploaded successfully in " + gid + ".";
                }
            }
        }
        else
        {
            return ("Group Doesn't exist.");
        }
    }

    else if (command == "list_files")
    {
        string gid = tokens[1], uid = tokens[2];
        if (user_list[uid].LoginStatus == false)
        {
            return "Login First!";
        }
        else if (group_list.find(gid) != group_list.end())
        {
            if (group_list[tokens[1]].members.find(uid) != group_list[gid].members.end())
            {
                string s = "";
                if (group_list[gid].file_names.empty())
                    return "List is empty";
                for (auto id : group_list[gid].file_names)
                {
                    s += "(";
                    s += id;
                    s += ") ";
                }
                return s;
            }
            else
                return ("You're not present in this group.");
        }
        else
        {
            return ("Group Doesn't exist.");
        }
    }

    else if (command == "download_file")
    {
        string gid = tokens[1], uid = tokens[4], filename = tokens[2];
        if (user_list[uid].LoginStatus == false)
        {
            return "Login First!";
        }
        else if (group_list.find(gid) != group_list.end())
        {
            if (group_list[gid].members.find(uid) == group_list[gid].members.end())
            {
                return ("You're not a member of this group.");
            }
            else
            {
                if (file_list.find(filename) == file_list.end())
                    return "No such file exist!";
                else
                {
                    bool inside = false;
                    string s = to_string(file_list[filename].seeders.size()) + " " + to_string(file_list[filename].file_size) + " " + file_list[filename].file_path;
                    if(file_list[filename].seeders.empty())
                    {
                        return "No seeder available";
                    }
                    for (auto seeder : file_list[filename].seeders)
                    {
                        if (user_list[seeder.first].LoginStatus == false)
                            continue;
                        inside = true;
                        s += " ";
                        s += seeder.second;
                    }
                    if (inside == false)
                        return ("Currently all seeders are offline.");
                    return s;
                }
            }
        }
        else
        {
            return ("Group Doesn't exist.");
        }
    }
    else if (command == "stop_share")
    {
        string gid = tokens[1], uid = tokens[3], fileName = tokens[2],ip_port = tokens[4];
        if (user_list[uid].LoginStatus == false)
        {
            return "Login First!";
        }
        else if (group_list.find(gid) != group_list.end())
        {
            if (group_list[gid].members.find(uid) == group_list[gid].members.end())
            {
                return ("You're not a member of this group.");
            }
            else
            {
                if (file_list.find(fileName) != file_list.end())
                {
                    if (file_list[fileName].seeders.find({uid, ip_port}) == file_list[fileName].seeders.end())
                    {
                        return "U r not a seeder";
                    }
                    else
                    {
                        file_list[fileName].seeders.erase({uid, ip_port});
                        return uid+" stopped sharing";
                    }
                }
                else
                {
                    return "File doesn't exist";
                }
            }
        }
        else
        {
            return ("Group Doesn't exist.");
        }
    }
    else
    {
        return ("Command invalid");
    }
}

void *reader(void *param)
{
    int *newsock = (int *)(param);
    int newsockfd = *newsock;
    char buffer[256];
    int n;
    while (1)
    {
        if (newsockfd < 0)
            error("ERROR on accept");
        bzero(buffer, 256);

        if (n = read(newsockfd, buffer, 255) <= 0)
        {
            close(newsockfd);
            return NULL;
        }
        string outputcmd = extractcmd(buffer);
        char buff[256];
        strcpy(buff, outputcmd.c_str());
        cout << endl
             << outputcmd << endl;
        n = write(newsockfd, buff, strlen(buff));
        if (n < 0)
            error("ERROR writing to socket");
    }
}

void *quit_op(void *param)
{
    string s;
    while(1)
    {
        cin>>s;
        if(s=="quit")
        {
            exit(0);
        }
    }
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    string line;
    ifstream readFile(argv[1],ios::in);
    while (getline(readFile, line));
    readFile.close();
    vector<string> v1;
    stringstream ss(line);
    while (ss.good())
    {
        string sb;
        getline(ss, sb, ':');
        v1.push_back(sb);
    }
    // IP_SERVER = v1[0];
    // TRACKER_PORT = stoi(v1[1]);
    cout << line;
    portno = stoi(v1[1]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd, 5);
    socklen_t clilen;
    clilen = sizeof(cli_addr);
    int i = 0;
    pthread_t tid1;
    if (pthread_create(&tid1, NULL, quit_op, NULL) != 0)
        printf("Failed to create thread\n");
    vector<pthread_t> tids;
    while (1)
    {
        pthread_t tid;
        newsockfd = accept(sockfd,
                           (struct sockaddr *)&cli_addr,
                           &clilen);
        if (pthread_create(&tid, NULL,
                           reader, (void *)(&newsockfd)) != 0)
            printf("Failed to create thread\n");
        // pthread_join(tid[i],NULL);
        tids.push_back(tid);
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}