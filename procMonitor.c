#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <linux/limits.h>
void displayProcessInfo(const char *pid) {
    char statusPath[256], exePath[256], line[256];//buffers
    FILE *statusFile;
    uid_t uid = getuid(); // User ID of the current user

    // Store the paths to the buffer in a format "proc/pid/..."
    snprintf(statusPath, sizeof(statusPath), "/proc/%s/status", pid);
    snprintf(exePath, sizeof(exePath), "/proc/%s/exe", pid);

    statusFile = fopen(statusPath, "r");//open and read the file by path name buffer. 
    if (!statusFile) return;
    int ownedByUser = 0;    //flag variables
    long voluntarySwitches = 0, nonvoluntarySwitches = 0;//variables for context switches of every process.

    // Read information from status file, use strncmp to compare the string with targeted text in a very brute way: match the same numbers of letters.
    while (fgets(line, sizeof(line), statusFile)) {
        if (strncmp(line, "Uid:", 4) == 0) {
            int ruid;
            sscanf(line, "Uid:\t%d", &ruid);
            ownedByUser = (uid == ruid);//Set the flag to be true if UID matches.
        } else if (strncmp(line, "voluntary_ctxt_switches:", 24) == 0) {
            sscanf(line, "voluntary_ctxt_switches:\t%ld", &voluntarySwitches);
        } else if (strncmp(line, "nonvoluntary_ctxt_switches:", 27) == 0) {
            sscanf(line, "nonvoluntary_ctxt_switches:\t%ld", &nonvoluntarySwitches);
        }
    }
    fclose(statusFile);//Close the status file after getting the information.

    if (ownedByUser) {//Check if the process belongs to the user.
        char exeLink[PATH_MAX + 1];//buffer
        ssize_t len = readlink(exePath, exeLink, sizeof(exeLink) - 1);
        if (len != -1) {
            exeLink[len] = '\0'; // Ensure null-terminated string
        } else {
            strcpy(exeLink, "N/A"); // Executable path not accessible or doesn't exist
        }

        printf("PID: %s, Voluntary Context Switches: %ld, Nonvoluntary Context Switches: %ld, Executable Path: %s\n",
               pid, voluntarySwitches, nonvoluntarySwitches, exeLink);
    }
}

int main() {
    DIR *d = opendir("/proc");//open the /proc directory.
    if (!d) {//if can't open, probably won't get this result.
        perror("Failed to open /proc");
        return 1;
    }
     printf("This is a Process Monitor, all processes info are listed below:\n");
    struct dirent *dir;//recognize and read all processes in the /proc directory by numerics name.
    while ((dir = readdir(d)) != NULL) {//processes are folders that start with number.
        if (dir->d_type == DT_DIR && dir->d_name[0] >= '0' && dir->d_name[0] <= '9') {
            displayProcessInfo(dir->d_name);//get useful information from every process folder.
        }
    }
    closedir(d);//close the directory.
    return 0;
}
