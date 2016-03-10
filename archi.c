#include <math.h>
#include <stdio.h>
#define add 32
#define addu 33
#define sub 34
#define AND 36
#define XOR 38
#define nor 39
#define nand 40
#define OR 37
FILE *iimg,*dimg,*error,*snapshot;
long dSize,iSize;
char *ibuffer,*dbuffer;
size_t iresult,dresult;
void OpenFile();
void PutInMemory();
void Start();
int iMemory[256]={0},dMemory[256]={0};
unsigned reg[32],inumber,dnumber;
int PC=0,sp=0;
int m[32],OpSum=0,FuncSum=0;
int main(int argc, const char * argv[]) {
    
    return 0;
}
void PutInMemory()
{
    for (int i=0; i<4; i++) {
        PC=PC|ibuffer[i];
        if(i!=3)
        {
            PC=PC<<8;
        }
    }
    for (int i=4; i<8; i++) {
        inumber=inumber|ibuffer[i];
        if (i!=7) {
            inumber=inumber<<8;
        }
    }
    
    int temp=0;
    int count=0;
    for (int i=8;i<inumber*4;i++) {
        iMemory[temp]=iMemory[temp]|ibuffer[i];
        iMemory[temp]=iMemory[temp]<<8;
        if(count==4)
        {
            temp++;
            count=0;
        }
        count++;
    }
    
    for (int i=0; i<4; i++) {
        sp=sp|dbuffer[i];
        if(i!=3)
        {
            sp=sp<<8;
        }
    }
    for (int i=4; i<8; i++) {
        dnumber=dnumber|dbuffer[i];
        if (i!=7) {
            dnumber=dnumber<<8;
        }
    }
    
    int dtemp=0;
    int dcount=0;
    for (int i=8;i<dnumber*4;i++) {
        dMemory[dtemp]=dMemory[dtemp]|dbuffer[i];
        dMemory[dtemp]=dMemory[dtemp]<<8;
        if(dcount==4)
        {
            dtemp++;
            dcount=0;
        }
        dcount++;
    }
}
void decode(int memory)
{
    int r,i=0;
    while(memory!=0)
    {
        r=memory%2;
        memory/=2;
        m[i]=r;
        i++;
    } 
}
void OpenFile()
{
    
    
    iimg = fopen("iimage.bin","rb");
    dimg = fopen("dimage.bin", "rb");
    error = fopen("error_dump.rpt", "wb");
    snapshot = fopen("snapshot.rpt", "wb");
    
    if (iimg==NULL || dimg==NULL)
    {
        fputs ("File error",stderr); exit (1);
    }
    
    fseek(iimg, 0 , SEEK_END);
    fseek(dimg, 0, SEEK_END);
    dSize = ftell(iimg);
    iSize = ftell(dimg);
    rewind(iimg);
    rewind(dimg);
    
    ibuffer = (char*) malloc (sizeof(char)*iSize);
    dbuffer = (char*) malloc (sizeof(char)*dSize);
    if (ibuffer == NULL || dbuffer == NULL)
    {
        fputs ("Memory error",stderr); exit (2);
    }
    
    iresult = fread (ibuffer,1,iSize,iimg);
    if (iresult != iSize || dresult != dSize)
    {
        fputs ("Reading error",stderr); exit (3);
    }
    
    fclose(iimg);
    fclose(dimg);
    
}
int toOpcode(int memory)
{
    decode(memory);
    OpSum=0;
    int j=0;
    for (int i=26; i<=31; i++) {
        OpSum=OpSum+pow(m[i],j);
        j++;
    }
    return OpSum;
    
}
int toFunc(int memory)
{
    decode(memory);
    FuncSum=0;
    int j=0;
    for (int i=26; i<=31; i++) {
        FuncSum=FuncSum+pow(m[i],j);
        j++;
    }
    return FuncSum;
}
void Start()
{
    int k=0,opcode=0,function=0;
    while (1) {
        opcode=toOpcode(iMemory[k]);
        if (opcode==0) {
            function=toFunc(iMemory[k]);
            if (function==add) {
                
            }
            else if (function==addu)
            {
                
            }
            else if(function==sub)
            {
                
            }
            else if(function==OR)
            {
                
            }
            else if (function==AND)
            {
                
            }
            else if(function==XOR)
            {
                
            }
            else if(function==nor)
            {
                
            }
            else if(function==nand)
            {
                
            }
            
        }
        else
        {
            
        }
        k++;
    }
}