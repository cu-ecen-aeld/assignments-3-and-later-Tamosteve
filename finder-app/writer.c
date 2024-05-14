#include <stdio.h>
#include <syslog.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <file> <string>\n", argv[0]);
        syslog(LOG_ERR,"Usage: %s <file> <string>\n", argv[0]);
        return 1;
    }

    openlog("Assignment 2", LOG_PID, LOG_USER);

    char *writefile = argv[1];
    char *writestr = argv[2];
    
    printf("Writing %s to %s", writestr, writefile);
    syslog(LOG_DEBUG, "Writing %s to %s", writestr, writefile);

    FILE* fichier = fopen(writefile, "w"); // Utiliser "w" au lieu de "r+" pour écrire dans le fichier

    if (fichier != NULL) {
        fputs(writestr, fichier);
        fclose(fichier);
        printf("Write successful!\n");
        syslog(LOG_DEBUG, "Writing %s to %s", writestr, writefile);
        
    } else {
        printf("Unable to open file '%s' for writing\n", writefile);
        syslog(LOG_ERR, "Error writing to file");
    }

    return 0;
}
