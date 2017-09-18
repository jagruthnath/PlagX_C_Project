#include <stdio.h>
#include <string.h>
#include "Plagiarism.h"

int main(int argc, char *argv[])
{
    //Copying cmdline argument to a char array for further use.
    strcpy(directory,argv[1]);
    int nof=f_count();//Counting no.of files
    char fwp[nof][100];//Path to each file
    char fname[nof][20];
    char content1[1000],content2[1000],last1[50],last2[50];//Fingerprinting variables
    int size_a,size_b,lens1,lens2;
    for (int i = 0; i < nof ; ++i)
        strcpy(fwp[i],"");
    file_read(fwp,fname);//All the file names and path to each file is generated.
    for (int i = 0; i < nof ; ++i)
        fname[i][strlen(fname[i])]='\0';
    float plagx[nof][nof],plagx_lcs[nof][nof],plagx_finger[nof][nof];
    for(int a=0;a<nof-2;a++)
    {
        for(int b=a;b<nof-2;b++)
        {
            if(a!=b)
            {
                // printf("\nFile %s and File %s\n",fname[a],fname[b]);

                // printf("%s\t:\t",fname[a]);
                int flag=1;
                process(fwp[a],t1,w1,flag);         //collecting words and their count in w1 and list1
                size_a=n;
                lens1=lens;
                flag=0;

                // printf("\n%s\t:\t",fname[b]);
                process(fwp[b],t2,w2,flag);         //collecting words and their count in w2 and list2
                size_b=n;
                lens2=lens;
                //printf("\n");

                //Bag of Words start
                float euc1=Euclid(list1,size_a);
                float euc2=Euclid(list2,size_b);
                float dot=(dp(size_a,size_b))/(euc1*euc2);
                dot*=100;
                //printf("Plagiarism \t: %f\n",dot);
                plagx[a][b]=dot;
                plagx[b][a]=dot;

                //Bag of Words end

                //LCS start
                size_t match=0;
                for (int i = 0; i < size_a; ++i)
                    for (int j = 0; j < size_b; ++j)
                        if(strcmp(w1[i],w2[j])==0)
                            match=match<strlen(w1[i])?strlen(w1[i]):match;

                float value=((match*2)*100)/(lens1+lens2);

                plagx_lcs[a][b]=value;
                plagx_lcs[b][a]=value;
                //LCS end

                //Fingerprinting start
                process_finger(fwp[a],content1,last1);
                strrev(last1);
                int len_a=lens;
                process_finger(fwp[b],content2,last2);
                strrev(last2);
                int len_b=lens;
                int small=len_a>len_b?len_b:len_a;
                //  printf("\n%s\n%s\n%s\t%s\t%d",content1,content2,last1,last2,small);

                int hash_table1[100],hash_table2[100],THA,THB;
                THA=generate_hash(content1,last1,hash_table1);
                THB=generate_hash(content2,last2,hash_table2);

                plagx_finger[a][b]=plagx_finger[b][a]=generate_p(hash_table1,hash_table2,THA,THB,small);

                //Fingerprinting end
            }
            else
            {
                plagx[a][b]=plagx[b][a]=100;
                plagx_lcs[a][b]=plagx_lcs[b][a]=100;
                plagx_finger[a][b]=plagx_finger[b][a]=100;
            }
        }
    }

    printf("\nBag of Words\n");
    print_matrix(nof,fname,plagx);

    printf("\nLCS\n");
    print_matrix(nof,fname,plagx_lcs);

    printf("\nFingerprinting\n");
    print_matrix(nof,fname,plagx_finger);

    return 1;
}