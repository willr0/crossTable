//
//by willr0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define SIZE 80

struct use {
    int c;
    struct use *next;
};

struct Node {
    char id[11];
    int def;
    struct use *use;
    struct Node *next;
};

void listing(FILE *i,FILE *o);
void crossTable(FILE *i,FILE *o);

int main(int argc, char *argv[])
{
    FILE *fi;
    FILE *fo;
    if(argc!=4){
        printf("Error, Requires: ' flag, input.file, output.file '\n");
        exit(1);
    }
    fi=fopen(argv[2],"r");
    fo=fopen(argv[3],"w");
    
    if(fi==NULL)
    {
        fprintf(stderr,"Error opening input file");
        exit(1);
    }
    if(fo==NULL)
    {
        fprintf(stderr,"Error opening output file\n");
        exit(1);
    }
    if(strcmp(argv[1],"-l")==0)
        listing(fi,fo);
    else if(strcmp(argv[1],"-c")==0)
        crossTable(fi,fo);
    else if(strcmp(argv[1],"-b")==0)
    {
        listing(fi,fo);
        crossTable(fi,fo);
    }
    fclose(fi);
    fclose(fo);
    return 0;
}

void listing(FILE *i,FILE *o)
{
    char line[SIZE];
    int count=1;
    
    while((fgets(line,SIZE,i))!=NULL)
    {
        //If blank line, skip
        if(strcmp(line,"\n")==0)
        {
            fprintf(o,"%s",line);
            continue;
        }
        //print line with line number
        fprintf(o,"%d %s",count,line);
        //add to count
        count++;
    }
    fprintf(o, "\n\n");
    rewind(i);
}

void crossTable(FILE *i,FILE *o)
{
    char line[SIZE];
    char *tok;
    int j;
    int count = 1;
    struct Node *table=NULL;
    struct Node *curr;
    struct Node *temp;
    struct use *t, *u = NULL;
    
    fprintf(o, "Identifier     Defined     Used\n");
    //Store current line in line variable
    while(fgets(line,SIZE,i)!=NULL){
        tok=strtok(line, "#");
        //if the line is blank - skip
        if(isspace(*line)!=0){
            continue;
        }
        
        //iterate through file
        for(j=0;j<SIZE;j++){
            //Find where defined
            if(line[j]==':'&&line[0]!='#'&&tok[0]!='#'){
                temp = (struct Node *)malloc(sizeof(struct Node));
                //Copy identifier to temp variable
                strncpy(temp->id,line,j);
                temp->def=count;

                if(table==NULL){
                    table=temp;
                    table->next=NULL;
                    break;
                }
                
                curr=table;
                while(curr->next!=NULL){
                    curr=curr->next;
                }
                curr->next=temp;
                break;
            }
        }
        tok=strtok(NULL, "#");
        count++;
    }
    rewind(i);
    count = 1;
    while(fgets(line,SIZE,i)!=NULL){
        tok=strtok(line, "#");
        
        //if blank line
        if(line[j]==':'&&line[0]!='#'&&tok[0]!='#'){
            continue;
        }
        //iterate through file
        curr=table;
        while(curr!=NULL&&line[0]!='#'&&tok[0]!='#'){
            t = (struct use *)malloc(sizeof(struct use));
            t->c=count;
            u=curr->use;
            if(strstr(line, curr->id)!=NULL){
                if(curr->def == count){ break; }
                if(curr->use==NULL){
                    curr->use=t;
                    break;
                }
                while(u->next!=NULL){
                    u=u->next;
                }
                u->next=t;
            }
            curr=curr->next;
        }
        tok=strtok(NULL, "#");
        count++;
    }
    curr=table;
    while(curr!=NULL){
        fprintf(o, "%7s%11d       ", curr->id, curr->def);
        t = curr->use;
        while(t!=NULL){
            fprintf(o, "%3d", t->c);
            t=t->next;
        }
        fprintf(o, "\n");
        curr=curr->next;
    }
}
