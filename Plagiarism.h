#ifndef PLAGX_C_PROJECT_PLAGIARISM_H
#define PLAGX_C_PROJECT_PLAGIARISM_H
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include <ctype.h>
//w1,w2 for storing unique words, t1,t2 for storing individual words in a file.
char w1[50][50],w2[50][50],t1[50][50],t2[50][50];
int list1[50],list2[50],lens=0;
int n=0,kg=5;
char directory[100];//="C:\\Testfiles\\";
int f_count()
{
    int nof=0;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (directory)) != NULL)
        while ((ent = readdir (dir)) != NULL)
            nof++;
    return nof;
}
void file_read(char fwp[f_count()][100],char fname[f_count()+1][20])
{
    int nof=0,i=0;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (directory)) != NULL) {
        /* Print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            nof++;
            char file[strlen(directory)];
            strcpy(file,directory);
            strcat(file,ent->d_name);
            if(nof>2) {
                strcpy (fname[i], ent->d_name);
                strcpy(fwp[i],directory);
                strcat(fwp[i],fname[i]);
                i++;
            }
        }
        closedir (dir);
    } else {
        perror ("Cannot open folder");
    }
}

//Bag of Words methods start

float Euclid(const int list[],int size)
{
    int sum=0;
    for(int i=0;i<=size;i++)
        sum=sum+(list[i]*list[i]);
    return (float)sqrt(sum);
}

int dp(int x,int y)
{
    int sum=0;
    for(int i=0;i<=x;i++)
        for(int j=0;j<=y;j++)
            if(strcmp(w1[i],w2[j])==0)
                sum=sum+(list1[i]*list2[j]);
    return sum;
}

//Bag of Words methods end

//Common method to store words for Bag of Words and LCS
void process(char filename[],char t[50][50],char w[50][50],int flag)
{
    FILE *fp;
    char text[50],word[50];
    fp=fopen(filename,"r");
    int a=0,b=0,count=0;
    lens=0;
    while(!feof(fp))
    {
        fscanf(fp,"%s",text);
        strlwr(text);
        lens+=strlen(text);
        // printf("%s  ",text);
        //Loop for parsing each character and storing words in a 2-D Array of type:char
        for(int i=0;i<strlen(text);i++)
        {
            if(isdigit(text[i]) || isalpha(text[i]) || (int)text==95)
                word[i]=text[i];
            else
            {
                word[0]='\0';
                break;
            }
            if(i == strlen(text) - 1)
            {
                word[i+1]='\0';
                strcpy(t[a++],word);
                word[0]='\0';
            }
        }
        count=0;
        for(int i=0;i<a;i++)
            if(strcmp(t[i],text)==0)
                count++;
        if(count==1)
            strcpy(w[b++],text);
    }
    int size=b-1;
    int list[size];
    n=size;
    for(int i=0;i<b;i++)
    {
        count=0;
        for(int m=0;m<a;m++)
            if(strcmp(w[i],t[m])==0)
                count++;
        list[i]=count;
    }
    if(flag)
        for (int j = 0; j < b; ++j)
            list1[j]=list[j];
    else
        for (int j = 0; j < b; ++j)
            list2[j]=list[j];

    fclose(fp);
}

void print_matrix(int nof,char fname[nof][20], float p[nof][nof])
{
    printf("\nFILE\t");
    for(int i=0;i<nof-2;i++)
        printf("%s\t",fname[i]);
    printf("\n");
    for(int i=0;i<nof-2;i++)
    {
        printf("%s\t",fname[i]);
        for(int j=0;j<nof-2;j++)
            printf("%.2f\t",p[i][j]);
        printf("\n");
    }
}

//Fingerprinting methods start
void process_finger(char filename[],char content[],char last[])
{
    FILE *fp;
    fp=fopen(filename,"r");
    char text[50];
    lens=0;
    strcpy(content,"");
    while(!feof(fp))
    {
        fscanf(fp,"%s",text);
        strlwr(text);
        lens+=strlen(text);
        strcat(content,text);
    }
    int k=lens-1;
    int i;
    for (i = 0; i < kg ; i++)
        last[i]=content[k--];
    last[i]='\0';
}

int generate_hash(const char content[],char last[],int ht[])
{
    char hash[kg];
    int j;
    long h=0;
    for (j = 0; strcmp(hash,last)!=0; ++j)
    {
        int i;
        for (i = 0; i < kg ; i++)
            hash[i]=content[j+i];
        hash[i]='\0';
        for (int k = 0; k < kg; ++k)
            h+=(int)hash[k]*powf(10,k);
        h%=10007;
        ht[j]=h;
    }
    return j;
}

int generate_p(const int hash_table1[100],const int hash_table2[100],int THA,int THB,int small)
{
    int c=0;
    for (int i = 0; i < small; ++i)
        if ( hash_table1[i] == hash_table2[i] )
            c++;
    return (2*c*100)/(THA+THB);
}
//Fingerprinting methods end

#endif //PLAGX_C_PROJECT_PLAGIARISM_H