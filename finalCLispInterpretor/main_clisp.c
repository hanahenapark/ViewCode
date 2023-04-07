
    /*
    Hana Park
    CS403 cLisp Interpretor

    Format
    1. To run files, compile c code with make file 
    2. Format to run files: ./lisp <file.lisp>
        --> ./lisp.exe testMe.lisp
    3. Format to run script: ./lisp
    */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define CLISP
#define LINE_MAX 4096
#include "clisp.c"
#include "EVALenv.c"

static Lisp cLISPstart(Lisp args, cLispError* e, clispPtrENV clsp);

int main(int argc, const char* argv[])
{

    const char* file = NULL; 
    clispPtrENV clsp = loadLisp();
    defineVM(cLISPcdr(cLISPenv(clsp)), createSymbol("LOAD", clsp), cLISPmake_func(cLISPstart), clsp);
    cLISPtable_set(cLISPmacro_table(clsp),createSymbol("INCLUDE", clsp),cLISPmake_func(cLISPstart),clsp);

//RUNNING script
  if(argc==1){ //"./lisp" => running script 
    // REPL
        char line[LINE_MAX];
        printf("Press CTRL + C to quit the program\n");
        while (!feof(stdin))
        {
            printf("lisp> ");
           
            if (!fgets(line, LINE_MAX, stdin)) break;
            printf("==> ");  
            cLispError error;
            Lisp code = cLISPread(line, &error, clsp);
            
            if (error != ERROR_NONE)
            {
                fprintf(stderr, "%s\n", cLISPerror_string(error));
                continue;
            }

            Lisp l = cLISPeval(code, &error, clsp);
          

            if (error != ERROR_NONE)
            {
                fprintf(stderr, "%s\n", cLISPerror_string(error));
            }

            cLISPprintf(stdout, l);
            printf("\n");
            
            cLISPcollect(cLISPnull(), clsp);
            
       }
    
   }
   else if(argc==2) //Running File 
   {
    FILE* fp = fopen(argv[1], "r");

    //const char* file = NULL; 
    if(fp){
        cLispError error;
        printf("\n-------------------------------------------------\n");
        Lisp l = cLISPread_file(fp, &error, clsp);//cLISPread_path

        if (error != ERROR_NONE)
        {
            fprintf(stderr, "%s. %s\n", file, cLISPerror_string(error));
        }
        Lisp code = cLISPmacroexpand(l, &error, clsp);

        if (error != ERROR_NONE)
        {
            fprintf(stderr, "%s\n", cLISPerror_string(error));
            exit(1);
        }

        cLISPeval(code, &error, clsp);  
       

        if (error != ERROR_NONE)
        {
            fprintf(stderr, "%s\n", cLISPerror_string(error));
            exit(1);
        }

        cLISPcollect(cLISPnull(), clsp);
    }
    else{
        printf("Invalid File");
      
    }

   }
   else{
    printf("FORMAT Option 1: ./lisp <file.lisp> \n");
    printf("FORMAT Option 2: ./lisp\n");
    printf("Please try again \n");
   }    

    cLISPshutdown(clsp);

    return 0;
}

static Lisp cLISPstart(Lisp args, cLispError* e, clispPtrENV clsp)
{
    Lisp path = cLISPcar(args);
    Lisp result = cLISPread_path(cLISPstring(path), e, clsp);
    if (*e != ERROR_NONE) return cLISPnull();
    return cLISPeval(result, e, clsp);
}