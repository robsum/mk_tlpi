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

void buildProcessTree(const std::list<Process>& allProcesses, Process& root) {
    for (const auto& process : allProcesses) {
        if (process.ppid == root.pid) {
            root.children.push_back(process);
            buildProcessTree(allProcesses, root.children.back());
        }
    }
}

std::list<Process> findRootProcesses(const std::list<Process>& allProcesses) {
    std::list<Process> rootProcesses;

    for (const auto& process : allProcesses) {
        bool isRoot = true;
        for (const auto& otherProcess : allProcesses) {
            if (process.ppid == otherProcess.pid) {
                isRoot = false;
                break;
            }
        }

        if (isRoot) {
            rootProcesses.push_back(process);
            buildProcessTree(allProcesses, rootProcesses.back());
        }
    }

    return rootProcesses;
}

void displayProcessTree(const Process& process, int depth = 0) {
    for (int i = 0; i < depth; ++i) {
        std::cout << "  "; // Indentation for better visualization
    }
    std::cout << process.pid << std::endl;

    for (const auto& child : process.children) {
        displayProcessTree(child, depth + 1);
    }
}

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

    std::list<Process> rootProcesses = findRootProcesses(list);

    // Display process tree for each root process
    for (const auto& rootProcess : rootProcesses) {
        displayProcessTree(rootProcess);
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
