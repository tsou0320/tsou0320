#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define add 32
#define addu 33
#define sub 34
#define AND 36
#define XOR 38
#define nor 39
#define nand 40
#define OR 37

#define slt 42
#define sll 0
#define srl 2
#define sra 3
#define jr 8
#define addi 8
#define addiu 9
#define  halt 63

#define lw 35
#define lh 33
#define lhu 37
#define lb 32
#define lbu 36
#define sw 49
#define sh 41
#define sb 40
FILE *iimg,*dimg,*error,*snapshot;
long dSize,iSize;
char *ibuffer,*dbuffer;

void OpenFile();
void PutInMemory();
void Start();
void decode(int memory);
int toOpcode();
int toFunc();
int toRs();
int toRt();
int toRd();
int toShamt();
int toimmediate();
void Start();
void print();

int iMemory[256]={0},dMemory[256]={0};
unsigned reg[32],inumber,dnumber;
int PC=0,sp=0,k=0;
int m[32],OpSum=0,FuncSum=0,pos=0;
int main(int argc, const char * argv[]) {
    OpenFile();
    return 0;
}
void PutInMemory()
{
    for (int i=0; i<4; i++) {
        PC=PC|ibuffer[i];
        if(i!=4)
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
    for (int i=8;i<8+inumber*4;i++) {
        if(count==4)
        {
            temp++;
            count=0;
        }
        iMemory[temp]=iMemory[temp]|ibuffer[i];
        iMemory[temp]=iMemory[temp]<<8;
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
    for (int i=8;i<8+dnumber*4;i++) {
        if(dcount==4)
        {
            dtemp++;
            dcount=0;
        }
        dMemory[dtemp]=dMemory[dtemp]|dbuffer[i];
        dMemory[dtemp]=dMemory[dtemp]<<8;
        dcount++;
    }
}
void decode(int memory)
{
    
    for (int i=0; i<32; i++) {
        m[i]=0;
    }
    int r,i=0;
    while(memory!=0)
    {
        m[i]=0;
        r=memory%2;
        memory/=2;
        m[i]=r;
        i++;
    } 
}
void OpenFile()
{
    size_t iresult,dresult;

    
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
int toOpcode()
{
    OpSum=0;
    int j=0;
    for (int i=26; i<=31; i++) {
        OpSum=OpSum+pow(m[i],j);
        j++;
    }
    return OpSum;
    
}
int toFunc()
{
    FuncSum=0;
    int j=0;
    for (int i=26; i<=31; i++) {
        FuncSum=FuncSum+pow(m[i],j);
        j++;
    }
    return FuncSum;
}
int toRs()
{
    int rs = 0;
    int j=0;
    for (int i=21; i<=25; i++) {
        rs=rs+pow(m[i],j);
        j++;
    }
    return rs;
}
int toRt()
{
    int rt=0,k=0;
    for (int i=16; i<=20; i++) {
        rt=rt+pow(m[i],k);
        k++;
    }
    return rt;
}

int toRd()
{
    int rd=0,l=0;
    for (int i=11; i<=15; i++) {
        rd=rd+pow(m[i],l);
        l++;
    }
    return rd;
}
int toShamt()
{
    int C=0,p=0;
    for (int i=6; i<=10; i++) {
        C=C+pow(m[i],p);
        p++;
    }
    return C;
}
int toimmediate()
{
    int immediate=0,r=0;
    for (int i=0; i<=15; i++) {
        immediate=immediate+pow(m[i],r);
        r++;
    }
    return immediate;
}
void Start()
{
    int opcode=0,function=0,rs=0,rt=0,rd=0,C=0,immediate=0;
    while (1) {
        decode(iMemory[pos]);
        opcode=toOpcode();
        if (opcode==0) {
            function=toFunc();
            rd=toRd();
            rt=toRt();
            rs=toRs();
            if (function==add) {
                
                reg[rd] = reg[rs] + reg[rt];
            }
            else if (function==addu)
            {
                reg[rd] = reg[rs] + reg[rt];
            }
            else if(function==sub)
            {
                reg[rd] = reg[rs] - reg[rt];
            }
            else if(function==OR)
            {
                reg[rd] = reg[rs] | reg[rt];
            }
            else if (function==AND)
            {
                reg[rd] = reg[rs] & reg[rt];
            }
            else if(function==XOR)
            {
                reg[rd] = reg[rs] ^ reg[rt];
            }
            else if(function==nor)
            {
                reg[rd] = ~(reg[rs] | reg[rt]);
            }
            else if(function==nand)
            {
                reg[rd] = ~(reg[rs] & reg[rt]);
            }
            else if(function==slt)
            {
                reg[rd]=reg[rs]<reg[rt];
            }
            else if(function==sll)
            {
                C=toShamt();
                reg[rd]=reg[rt]<<C;
                
            }
            else if(function==srl)
            {
                C=toShamt();
                reg[rd]=reg[rt]>>C;
            }
            else if(function==sra)
            {
                C=toShamt();
                reg[rd]=reg[rt]>>C;
            }
            else if(function==jr)
            {
                PC=reg[rs];
            }
        }
        else if(opcode==halt)
        {
            break;
        }
        else if(opcode==addi)
        {
            int SignedImmediate=0,d=0;
            if (m[15]==0) {
                for (int i=0; i<=15; i++) {
                    SignedImmediate=SignedImmediate+pow(m[i],d);
                    d++;
                }
                reg[rt]=reg[rs]+SignedImmediate;
            }
            else
            {
                for (int i=0; i<=15; i++) {
                    if (m[i]==0) {
                        m[i]=1;
                    }
                    else
                    {
                        m[i]=0;
                    }
                }
                for (int i=0; i<=15; i++) {
                    SignedImmediate=SignedImmediate+pow(m[i],d);
                    d++;
                }
                reg[rt]=reg[rs]-SignedImmediate;
             }
            
        }
        else if(opcode==addu)
        {
            immediate=toimmediate();
            reg[rt]=reg[rs]+immediate;
        }
        else if (opcode==lw)
        {
            
        }
        else if (opcode==lh)
        {
            
        }
        else if (opcode==lhu)
        {
            
        }
        else if (opcode==lb)
        {
            
        }
        else if (opcode==lbu)
        {
            
        }
        else if (opcode==sw)
        {
            
        }
        else if (opcode==sh)
        {
            
        }
        else if (opcode==sb)
        {
            
        }
        pos++;
    }
}
void print()
{
    fprintf(snapshot,"cycle%d : ",k++);
    unsigned i;
    for (i = 0; i < 32; ++i) {
        fprintf(snapshot, "$%02u: 0x", i);
        fprintf(snapshot, "%08X\n", reg[i]);
    }
    fprintf(snapshot, "PC: 0x");
    fprintf(snapshot, "%08X\n\n\n", PC + pos);
}