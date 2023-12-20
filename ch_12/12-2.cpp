#include <iostream>
#include <filesystem>
#include <string>
#include <algorithm>
#include <map>
#include <list>
#include <mk_linux_programming_interface/tlpi-dist/lib/tlpi_hdr.h>

class Process {
    public:
        int pid;
        int ppid;
        std::list<Process> children;
        Process(int _pid, int _ppid) {
            pid = _pid;
            ppid = _ppid;
            children = std::list<Process>();
        };
        bool operator==(const Process& other) const {
            return pid == other.pid;
        };
};

char* getprocessname(const char* pid);
char* getprocessppid(const char* pid);

int main(void) {
    std::list<Process> list;

    std::string procpath = "/proc";

    for (const auto& entry : std::filesystem::directory_iterator(procpath)) {
        const std::string& filename = entry.path().filename();
        if (!filename.empty() && std::all_of(filename.begin(), filename.end(), ::isdigit)) {
            //std::cout << filename << " is a process" << std::endl;
            int pid = atoi(filename.c_str());
            int ppid = atoi(getprocessppid(filename.c_str()));
            //std::cout << filename << " ppid is " << ppid << std::endl;
            Process process(pid, ppid);
            list.push_back(process);
        }
    }


    std::list<Process> listWithChildren(list);
    for (const auto& process : list) {
        for (auto& processWithChildren : listWithChildren) {
            if (process == processWithChildren)
                continue;
            else if (process.ppid == processWithChildren.pid)
                processWithChildren.children.push_back(process);
                continue;
        }
    }

    for (const auto& process : listWithChildren) {
        if (!process.children.empty()) {    
            std::cout << process.pid << " has children: ";

            for (const auto& child : process.children) {
                std::cout << child.pid << " ";
            }

            std::cout << std::endl;
        }
    }

    return EXIT_SUCCESS;
}


char* getprocessname(const char *pid) {
    FILE *file;
    char line[256];
    char *name = (char*) malloc(256);
    char * filename = strdup("/proc/");
    strcat(filename, pid);
    strcat(filename, "/status");
    file = fopen(filename,"r");
    while((fgets(line, sizeof line, file) !=NULL))
    {
        if(strncmp(line, "Name:", 5) ==0)
	{
	    sscanf(line,"%*5s %s", name);
	}
    }
    fclose(file);
    return name;
}

char* getprocessppid(const char *pid) {
    FILE *file;
    char line[256];
    char *name = (char*) malloc(256);
    char * filename = strdup("/proc/");
    strcat(filename, pid);
    strcat(filename, "/status");
    file = fopen(filename,"r");
    while((fgets(line, sizeof line, file) !=NULL))
    {
        if(strncmp(line, "PPid:", 5) ==0)
	{
	    sscanf(line,"%*5s %s", name);
	}
    }
    return name;
}
