#include<stdio.h>
#include<string.h>

char *myitoa(int num,char*str);
void out_binary(int a,int length);

int main(){
	int a=-7;
	int length=9;
	out_binary(a,length);
	return 0;
}

void out_binary(int a,int length){
	
    int len1=0;
    int i;
    char buffer[16];
    myitoa (a,buffer);  
    if(a>=0){
    	len1=strlen(buffer);
		for(i=0;i<length-len1;i++){
			printf("0");
		}
		printf ("%s",buffer);   
	}
	
	if(a<0){
		for(i=length;i>0;i--)
		printf("%c",buffer[16-i]);
	}
}

/* 实现itoa函数的源代码 */   
char *myitoa(int num,char*str)   
{   
	  
    char index[]="01";   
    unsigned unum; /* 中间变量 */   
    int i=0,j,k;     
    unum=(unsigned short)num;  
    do {   
        str[i++]=index[unum%2];   
        unum/=2;   
    }while(unum);   
    str[i]='\0';     
    char temp;   
    for(j=0;j<=(i-1)/2.0;j++)   
    {   
        temp=str[j];   
        str[j]=str[i-j-1];   
        str[i-j-1]=temp;   
    }   
    return str;   
}  
