// #AUTHOR: HET_PATEL  ID:110123875

// importing libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


//defining global variable
#define MAX_BF_SZ 1024

FILE *Output(const char *processId, const char *lnxstring){ // a function to return the output of the linux command

    char cmd[MAX_BF_SZ];// var declration
    char *p_id = processId;
    //printf("inside output buffer");

    snprintf(cmd, sizeof(cmd), lnxstring, p_id);// storing the linux command into cmd var

    FILE *Outputlx = popen(cmd, "r");//getting the output of the linux terminal command using popen file that opens the process 

    if (Outputlx == NULL) {//error handling 

        fprintf(stderr, "Error in executing command %s.\n", cmd);// error message
        exit(EXIT_FAILURE);
    }
    
    return Outputlx;// returning the output
}

void nonDirDesDN(const char *rootPrc, const char *processId)  {// defining the non-direct descendants 

    char cmd[MAX_BF_SZ];// var decl
    char papid[MAX_BF_SZ];// var decl

    char* lnxstring = " ps -eo pid,ppid | grep -w %s | awk '{print $1}' ";// defining the string that is executed in command line

    printf("Non-Direct Descendants of Process ID %s:\n", processId); // msg printing

  
    FILE *psOutput = Output(processId, lnxstring);// calling the output fun and storing its return value inside psoutput
    
    while (fgets(papid, sizeof(papid), psOutput)!= NULL) {// storing the processids received in o/p into papid

        papid[strcspn(papid, "\n")] = 0; // Removing the newline character if present
       // printf("ele - %s\n", papid);
         
            if(strcmp(papid, processId) == 0)// checks if the processid matches with papid it does nothing
            {
                //as we don't want to print the process itself
            }
            else{
               // printf("okk");
                    char cmd[MAX_BF_SZ];// var decl

                    char* lnxstring = "pgrep -P %s";// var def
                    FILE *pgrepOutput = Output(papid, lnxstring);// calling output


                    char buffer[MAX_BF_SZ];//var decl

                    while (fgets(buffer, sizeof(buffer), pgrepOutput) != NULL) {
                        buffer[strcspn(buffer, "\n")] = 0; // Removing the newline character if present
                        printf("%s\n", buffer);// printing the output
                        immDescID(buffer);// calling the immdiate descendants function to get grand child's child
                    }

                    pclose(pgrepOutput);// closing the process
                    
                
            }        
                
        }
    pclose(psOutput);

}

void siblingProLP(const char *processId) { // method to print sibiling 

    char cmd[MAX_BF_SZ];// var decl
    char papid[MAX_BF_SZ];// var decl

    char* lnxstring = "ps -o ppid= -p %s";// lnxstring decl


    FILE *ppidOutput = Output(processId, lnxstring);//getting output via fun output
    fgets(papid, sizeof(papid), ppidOutput);// storing o/p in papid
    pclose(ppidOutput);// closing process

    char* lnxstring1 = "pgrep -P %s";// var def
    FILE *pgrepOutput = Output(papid, lnxstring1);// getting output of another linux cmd

    printf("Sibling Processes of Process ID %s:\n", processId);// printing sibling process

    char buffer[MAX_BF_SZ];// var decl
    //fgets(buffer, sizeof(buffer), pgrepOutput);// storing o/p in buffer
    //rplSubstr(buffer, processId, "\0");// calling rplesubstr to replace the processid with null inside buffer
    //printf("okkkkk%s\n", buffer);

    while (fgets(buffer, sizeof(buffer), pgrepOutput) != NULL) {// looping through output
        
        buffer[strcspn(buffer, "\n")] = 0; // newline character removal
        rplSubstr(buffer, processId, "\0");
        printf("%s\n", buffer);// printing buffer values
    }

    pclose(pgrepOutput);// closing the process
}

void rplSubstr(char *str, const char *substr, const char *replc) {// method to replaced substring in a string
    
    char buff[1024]; //var dec
    char *ptr;// var dec

    if (!(ptr = strstr(str, substr))) {// cheking whether string contains substring or not 
        return;// if contains then ptr will point to its location in string
    }

    strncpy(buff, str, ptr - str);// copies str till pointer location and then adds null charcter to it
    buff[ptr - str] = '\0';

    sprintf(buff + (ptr - str), "%s%s", replc, ptr + strlen(substr));// concating the string

    strcpy(str, buff);// coppying the new string to original variable
}

void immDescID(const char *processId) {// method to print immediate descendants
    
    char cmd[MAX_BF_SZ];// var decl

    char* lnxstring = "pgrep -P %s";// var dec and var initialization
    FILE *pgrepOutput = Output(processId, lnxstring);// getining o/p


    char buffer[MAX_BF_SZ];// var decl
    while (fgets(buffer, sizeof(buffer), pgrepOutput) != NULL) {// storing the output 
        buffer[strcspn(buffer, "\n")] = 0; // newline character removal
        printf("%s\n", buffer);// printing o/p
    }

    pclose(pgrepOutput);// closing the process
}

void grandChildGP(const char *processId) {// method to get grandchildren
    char cmd[MAX_BF_SZ];// var decl
   
    char* lnxstring = "pgrep -P %s | xargs -n1 pgrep -P"; // var initialization
    FILE *pgrepOutput = Output(processId, lnxstring); //getting output
 

    printf("Grand Children of Process ID %s:\n", processId);// msg printing

    char buffer[MAX_BF_SZ];//var decl
    while (fgets(buffer, sizeof(buffer), pgrepOutput) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0; // Removing the newline character if present
        printf("%s\n", buffer);// printing the output
    }

    pclose(pgrepOutput);// closing the process
}

char * t_empty_space(char *string) {// method to trim the empty spaces
    
    char *last;// var decl
   
    while (isspace(*string)) {// checking for whitespace in the begining
        string = string + 1;// if encoutered then moving to next point
    }
   
    last = string + strlen(string) - 1;// getting last non-null char 
    while (last > string && isspace(*last)) {// removing the whitespaces from back
        last = last - 1;
    }

    *(last+1) = '\0';// adding last charcater to null --> truncating the string
    return string;// return the trimmed string
}

void printPpidandSelfid(const char *processId) {//method to print process id and parent process id
    
    char cmd[MAX_BF_SZ];// var decl

    char* lnxstring = "ps -o ppid= -p %s";// var initialization
    FILE *psOutput = Output(processId, lnxstring);// getting output

    printf("Process ID[PID]: %s\n", processId);// pid printing
   
    char buffer[MAX_BF_SZ];// var decl
   
    while (fgets(buffer, sizeof(buffer), psOutput) != NULL) {// storing o/p
        buffer[strcspn(buffer, "\n")] = 0; // newline character removal
        // printf("%s\n", buffer);
    }
    char *nbuff = t_empty_space(buffer);// calling the function to trim the o/p

     printf("Parent Process ID[PPID]: %s\n\n", nbuff);// ppid printing

    pclose(psOutput);// closing the process
}

void processStatZZ(const char *processId) {// method to check process status

    char cmd[MAX_BF_SZ];// var decl


    char* lnxstring = "ps -o state= -p %s";// var initialization
    FILE *psOutput = Output(processId, lnxstring);// getting output

    char buffer[MAX_BF_SZ];// var decl

    if (fgets(buffer, sizeof(buffer), psOutput) != NULL) {// storing the o/p
        buffer[strcspn(buffer, "\n")] = 0; 

        if (strcmp(buffer, "Z") == 0) {// comparing the o/p with Z
            
            printf("Process ID %s is defunct.\n", processId);// if Z then defunct proc
        } 
        else{
            printf("Process ID %s is not defunct.\n", processId);// if not Z then non-defunct proc
        }
    } 
    else {
        
        fprintf(stderr, "Unable to find the process with ID %s.\n", processId);// msg printing
    }

    pclose(psOutput);// closing the process
}

void dirdecProcessStatZC(const char *processId){// method to get the process status of the direct descendants

    char cmd[MAX_BF_SZ];// var decl
 
    char* lnxstring = "pgrep -P %s";//var initiliazation
    FILE *pgrepOutput = Output(processId, lnxstring);//getting o/p

    printf("Process Status of direct descendant of Process ID %s is: \n", processId);// printing msg

    char buffer[MAX_BF_SZ];//var decl
    while (fgets(buffer, sizeof(buffer), pgrepOutput) != NULL) {// storing o/p in buff
        buffer[strcspn(buffer, "\n")] = 0; 
        //printProcessStatus(buffer);

                char cmd[MAX_BF_SZ];// var decl
       
                   char* lnxstring = "ps -o state= -p %s";// passing new linux string to check the status of process
                   FILE *psOutput = Output(buffer, lnxstring);

                char buff[MAX_BF_SZ];// var decl

                if (fgets(buff, sizeof(buff), psOutput) != NULL) {// storing output
                    buff[strcspn(buff, "\n")] = 0; // Removing the newline character if present
                    
                    if (strcmp(buff, "Z") == 0) {// checking for defunct process

                        printf("Process ID %s is defunct.\n", buffer);
                    }
                    // else {
                    //     printf("Process ID %s is not defunct.\n", processId);
                    // }
                } else {
                    fprintf(stderr, "Unable to find the process with ID %s.\n", buffer);// msg printing
                }

                pclose(psOutput);// closing process

    }

    pclose(pgrepOutput);

}

void allProcessdirdecProcessStatZX(const char *processids[]){// method to print the status of decedants of all the processes specified

        for (int p = 1; p <= 5; p++)// looping through the previous func
        {
            if (processids[p] != '\0')// checking for any null values
            {
                dirdecProcessStatZC(processids[p]); // calling the previous method as it returns the process status of the direct descendants
            }
            else{ // if null values do nothing 

            }
                
        }

}

void siblingStatZobSZ(const char *processId){// method to get the status of the sibling process

    char cmd[MAX_BF_SZ];// var decl
    char papid[MAX_BF_SZ];// var decl

    char* lnxstring = "ps -o ppid= -p %s";// var initialization


    FILE *ppidOutput = Output(processId, lnxstring); // getting o/p
    fgets(papid, sizeof(papid), ppidOutput);// storing o/p
    pclose(ppidOutput);// closing the process

    char* lnxstring1 = "pgrep -P %s";// var initialization
    FILE *psOutput = Output(papid, lnxstring1);// getting o/p
    //snprintf(cmd, sizeof(cmd), "pgrep -P %s", papid);// storing o/p in cmd


    printf("Sibling Processes of Process ID %s:\n", processId);// printing msg

    char buffer[MAX_BF_SZ];// var decl
    //fgets(buffer, sizeof(buffer), psOutput);// storing o/p 
    //rplSubstr(buffer, processId, "\0");// calling method to replace substring
  

    while (fgets(buffer, sizeof(buffer), psOutput) != NULL) {// storing o/p in buffer
        
    
        buffer[strcspn(buffer, "\n")] = 0; // newline character removal
    
               char* lnxstring = "ps -o state= -p %s";
                   FILE *psOutput = Output(buffer, lnxstring);

                char buff[MAX_BF_SZ];// var decl
                if (fgets(buff, sizeof(buff), psOutput) != NULL) {// storing output
                    buff[strcspn(buff, "\n")] = 0; 
                    if (strcmp(buff, "Z") == 0) {// checking for the process status

                        printf("Process ID %s is defunct.\n", buffer);
                    }
    
                } else {
                    fprintf(stderr, "Unable to find the process with ID %s.\n", buffer);// msg printing
                }

                pclose(psOutput);// closing the process
    }

    pclose(psOutput);// closing the process


}



int main(int argmc, char *argmv[]) {// main function
    
    if (argmc < 3)// checking the number of arguments if less than 3 , prints synopsis          
    {
        printf("Usage: %s [root_process] [process_id1] ... [process_id(n)]  [OPTION]\n", argmv[0]);   // user will get this message if the format of argument provided is not as per the need
        return 1;
    }
    
    char *processids[6];// array declration to store the proccess ids provided by the user

    processids[0] = argmv[1];// storing the root process id as the 1st element of the processids array
    processids[1] = argmv[2];// stoing the process_id1 in the same array

    if(argmc > 3){ //if cmd arguments are more than 3

        for (int p = 3; p < argmc; p++)// looping through arguments to collect processids
        {
            if (strcmp(argmv[p], "-dn") != 0 && strcmp(argmv[p], "-id") != 0  && strcmp(argmv[p], "-lp") != 0  && strcmp(argmv[p], "-sz") != 0  && strcmp(argmv[p], "-gp") != 0 && strcmp(argmv[p], "-zz") != 0 && strcmp(argmv[p], "-zc") != 0 && strcmp(argmv[p], "-zx") != 0 && strncmp(argmv[p], "-", strlen("-")) != 0)
            // only processids should be valid through this condintion
            {		
                processids[p-1] = argmv[p];   // adding the process ids to the processids element
                
            }
        }
    }

    char *option;// var decl

    for(int o=3; o<= argmc; o++){ // looping through arguments to get option specified
        if(argmv[o] == '\0')// cheking for the null character at the end of the arguments
        {
            option = argmv[o-1];// then moving 1 step backward to get the option
        }  
    }

    for (int p = 1; p <= 5; p++)// lopping through specified processes to print their pids and ppids
        {
            if (processids[p] != '\0'){
                printPpidandSelfid(processids[p]); // calling the function
            }
            else{// if processids null do nothing

            }
                
        }



    if(strcmp(option,"-dn") == 0 )// cheking if the option is dn or not (non-direct descendants)
    {
    nonDirDesDN(processids[0], processids[1]);// calling the respective method 
    }
    else if(strcmp(option,"-id") == 0 )// cheking if the option is id or not (immediate descendants)
    {
    immDescID(processids[1]); // calling the respective method 
    }
    else if(strcmp(option,"-lp") == 0 )// cheking if the option is lp or not (sibling processes)
    {
    siblingProLP(processids[1]);// calling the respective method 
    } 
    else if(strcmp(option,"-sz") == 0 )// cheking if the option is sz or not (sibiling status defunct)
    {
    siblingStatZobSZ(processids[1]);// calling the respective method 
    }
    else if(strcmp(option,"-gp") == 0 )// cheking if the option is gp or not (grandchild)
    {
    grandChildGP(processids[1]);// calling the respective method 
    }
    else if(strcmp(option,"-zz") == 0 )// cheking if the option is zz or not (defunct status)
    {
    processStatZZ(processids[1]);// calling the respective method 
    }
    else if(strcmp(option,"-zc") == 0 )// cheking if the option is zc or not (direct descendatns defunct status)
    {
    dirdecProcessStatZC(processids[1]);// calling the respective method 
    }
    else if(strcmp(option,"-zx") == 0 )// cheking if the option is zx or not (direct descendants of process_id1..process_id[n] defunct status)
    {
    allProcessdirdecProcessStatZX(processids);// calling the respective method 
    }
    else{// if some invalid option is provided 
        
        if(strncmp(option, "-", strlen("-")) == 0)// checking if the invalid option defined began with "-" or not to not discard any pid as invalid option
        {
            printf("please provide valid option");// printing the message       
        }
        else{// if no option is there

            option = "\0";// making sure option stays null
            //printf(" boo yeah");
        }
         
    }

    return 0;
}
