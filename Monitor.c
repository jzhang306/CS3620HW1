#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>


void displayProcessInfo(const char *pid) {
    char statusPath[256], exePath[256], line[256];
    FILE *statusFile;
    uid_t uid = getuid(); // User ID of the current user

    // Construct the path to the status file and the exe link for the process
    snprintf(statusPath, sizeof(statusPath), "/proc/%s/status", pid);
    snprintf(exePath, sizeof(exePath), "/proc/%s/exe", pid);

    statusFile = fopen(statusPath, "r");
    if (!statusFile) return;
    //flag variables
    int ownedByUser = 0;
    long voluntarySwitches = 0, nonvoluntarySwitches = 0;

    // Read information from status file
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

    if (ownedByUser) {
        char exeLink[PATH_MAX + 1];
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
    DIR *d = opendir("/proc");
    if (!d) {
        perror("Failed to open /proc");
        return 1;
    }

    struct dirent *dir;
    while ((dir = readdir(d)) != NULL) {
        if (dir->d_type == DT_DIR && dir->d_name[0] >= '0' && dir->d_name[0] <= '9') {
            displayProcessInfo(dir->d_name);
        }
    }
    closedir(d);
    return 0;
}
