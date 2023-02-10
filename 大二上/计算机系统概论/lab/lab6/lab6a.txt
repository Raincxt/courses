#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
//
//变量定义
// 
struct table{
	char label[20];
	int addr;
}sym_t[20]; 
int table_line=0;  		//symble table 最新行 
char inst[300][50];		//保存指令 
int inst_add[300];      //保存指令地址 
int count;				//计算地址时，保存.BLKW/.STRINGZ长度 
char saved_sy[35][9]={	//定义保留字符
		{'A','D','D'},{'A','N','D'},{'N','O','T'},
		{'L','D'},{'L','D','I'},{'L','E','A'},{'S','T'},{'S','T','I'},
		{'S','T','R'},{'L','D','R'},
		{'T','R','A','P'},
		{'B','R'},{'B','R','n'},{'B','R','z'},{'B','R','p'},
		{'B','R','n','z'},{'B','R','z','p'},{'B','R','n','p'},{'B','R','n','z','p'},
		{'J','M','P'},{'J','S','R','R'},{'J','S','R'},
		{'R','T','I'},{'R','E','T'},
		{'.','O','R','I','G'},{'.','F','I','L','L'},{'.','B','L','K','W'},
		{'.','S','T','R','I','N','G','Z'},{'.','E','N','D'},{'H','A','L','T'},
		{'G','E','T','C'},{'O','U','T'},{'P','U','T','S'},{'I','N'},{'P','U','T','S','P'},
	}; 	
	
//					 
//函数声明 
//
int judge(char*ptr,int i); 			//判断读入指令类型 
int input();						//输入代码，并保存在inst[][]中 ,返回行数 
void set_table(char*ptr,int i); 	//建立symbol_table 
void out_binary(int a,int length); 	//整数a转换成二进制以length长度输出 
char *myitoa(int num,char*str);

int main()
 {
	int result; 		//返回值   -1:LABEL;  
						/*0：ADD;   1: AND;   2: NOT;  
					      3: LD;   4: LDI;	5: LEA;   6: ST; 7:STI
						  8: STR;  9: LDR;
						 10:TRAP;  
						 11: BR ; 12:BRn; 13:BRz; 14:BRp;
						 15:BRnz; 16:BRzp; 17:BRnp; 18: BRnzp;
						 19: JMP; 20: JSRR; 21: JSR;
						 22: RTI;  23: RET;
						 24:.ORIG; 25: .FILL; 26:.BLKW; 27:.STRINGZ; 28: .END;
						 29:HALT 30:GETC 31:OUT 32:PUTS 33:IN 34:PUTSP*/
	int i=0;
	int imm,k,value;
	int j;
	int flag2=1;
	char labelA[20];
	int length;			//instuction 行数 	
	length=input(); 
	inst_add[0]=0;
	for(i=0;i<length;i++){				//第一遍遍历，建立symbol table 
		set_table(inst[i],i);
	} 	
/*	for(i=0;i<length;i++){				//第一遍遍历，建立symbol table 
		printf("%s\n",inst[i]);
		printf("%d\n",inst_add[i]);
	} */
	
	for(i=0;i<length;i++){				//第二遍遍历，翻译机器码
		j=0;
		result=judge(inst[i],i);
	
		//ADD AND
		if(result==1||result==0){
			if(result)printf("0101");		//输出opcode 
			else printf("0001");
			while(inst[i][j]!=',')	j++;	//输出DR
			j--;
			while(inst[i][j]<'0'||inst[i][j]>'9')	j--;
			out_binary(inst[i][j]-'0',3);
			while(inst[i][j]!=',')	j++;	//输出SR1
			while(inst[i][j]!='R')	j++;	
			j++;
			while(inst[i][j]==32)	j++;
			out_binary(inst[i][j]-'0',3);	
			j++;
	
			while(inst[i][j]!='R'&&inst[i][j]!='#'&&inst[i][j]!='x') j++;
			if(inst[i][j]=='#'){			//输出 1 imm5 
				printf("1");
				j++;
				while(inst[i][j]==32) j++;
				imm=0;
				if(inst[i][j]=='-'){
					j++;
				while(inst[i][j]==32) j++;
				while(inst[i][j]!=32&&inst[i][j]!=0){
					imm=imm*10+inst[i][j]-'0';
					j++;
				}
				out_binary(-imm,5);
				}
				else{
				while(inst[i][j]!=32&&inst[i][j]!=0){
					imm=imm*10+inst[i][j]-'0';
					j++;
				}
				out_binary(imm,5);	
				}
			}
			else if(inst[i][j]=='x'){			//debug1
				printf("1");
				j++;
				imm=0;
				value=0;
				
				while(inst[i][j]==32) j++;
				
				if(inst[i][j]=='-'){
					j++;
					while(inst[i][j]==32) j++;
					while(inst[i][j]!=0&&inst[i][j]!=32){
					if(inst[i][j]>='0'&&inst[i][j]<='9')
					value=inst[i][j]-'0';
					else if(inst[i][j]>='A'&&inst[i][j]<='F')
					value=inst[i][j]-'A'+10;
					else if(inst[i][j]>='a'&&inst[i][j]<='f')
					value=inst[i][j]-'a'+10;
					j++;
					imm=imm*16+value;
				}	
				out_binary(-imm,5);
				}
				else{
				 	while(inst[i][j]!=0&&inst[i][j]!=32){
					if(inst[i][j]>='0'&&inst[i][j]<='9')
					value=inst[i][j]-'0';
					else if(inst[i][j]>='A'&&inst[i][j]<='F')
					value=inst[i][j]-'A'+10;
					else if(inst[i][j]>='a'&&inst[i][j]<='f')
					value=inst[i][j]-'a'+10;
					j++;
					imm=imm*16+value;
				}	
				out_binary(imm,5);	
				}			
			}
			else if(inst[i][j]=='R'){
				printf("000");			//输出SR2 
			    j++;
			    while(inst[i][j]==32) j++;
				out_binary(inst[i][j]-'0',3);
			}
		printf("\n");
		}
		//NOT
		else if(result==2){
			printf("1001");
			
			while(inst[i][j]!=',')	j++;	//输出DR
			j--;
			while(inst[i][j]<'0'||inst[i][j]>'9') j--;
			out_binary(inst[i][j]-'0',3);
			
			while(inst[i][j]!=',')	j++;	//输出SR
			while(inst[i][j]!='R')	j++;	
			j++;
			while(inst[i][j]==32) j++;
			out_binary(inst[i][j]-'0',3);
			printf("111111\n");
		}
		//LD,LDI,LEA,ST,STI
		else if(result>=3&&result<=7){
			j=2;
			if(result==3) printf("0010");			//输出opcode 
			else if(result==4) printf("1010");
			else if(result==5) printf("1110");
			else if(result==6) printf("0011");
			else if(result==7) printf("1011");
			else;
			
			while(inst[i][j]!=',')	j++;	//输出DR/SR
			j--;
			while(inst[i][j]<'0'||inst[i][j]>'9') j--;
			out_binary(inst[i][j]-'0',3);
			
			j++;
			while(inst[i][j]==','||inst[i][j]==32) j++;
			
			if(inst[i][j]=='#'){			//输出 PCoffset9 
				j++;
				while(inst[i][j]==32) j++;
				imm=0;
				if(inst[i][j]=='-'){
					j++;
				while(inst[i][j]==32) j++;
				while(inst[i][j]!=32&&inst[i][j]!=0){
					imm=imm*10+inst[i][j]-'0';
					j++;
				}
				out_binary(-imm,9);
				}
				else{
				while(inst[i][j]!=32&&inst[i][j]!=0){
					imm=imm*10+inst[i][j]-'0';
					j++;
				}
				out_binary(imm,9);	
				}
			}
			else{
				k=0;
				while(inst[i][j]!=32&&inst[i][j]!=0){
					labelA[k]=inst[i][j];
					k++;
					j++;
				}
				labelA[k]='\0';
				for(k=0;k<table_line;k++){  
				flag2=strcmp(labelA,sym_t[k].label);
				if(flag2==0)
				out_binary(sym_t[k].addr-inst_add[i]-1,9);
				}	
			}
			printf("\n");	
		}
		//STR,LDR
		else if(result==8||result==9){
			j=0;
			if(result==8) printf("0111");
			else printf("0110");
			
			while(inst[i][j]!=',')	j++;	//输出DR/SR 
			j--;
			while(inst[i][j]<'0'||inst[i][j]>'9')	j--;
			out_binary(inst[i][j]-'0',3);
			while(inst[i][j]!=',')	j++;
			
			while(inst[i][j]!='R')	j++;	//输出BaseR
			j++;
			while(inst[i][j]==32)	j++;
			out_binary(inst[i][j]-'0',3);	
			j++;
		
			while(inst[i][j]==','||inst[i][j]==32) j++;		//输出 PCoffset6 
			if(inst[i][j]=='#'){			
				j++;
				while(inst[i][j]==32) j++;
				imm=0;
				if(inst[i][j]=='-'){
					j++;
				while(inst[i][j]!=32&&inst[i][j]!=0){
					imm=imm*10+inst[i][j]-'0';
					j++;
				}
				out_binary(-imm,6);
				}
				else{
				while(inst[i][j]!=32&&inst[i][j]!=0){
					imm=imm*10+inst[i][j]-'0';
					j++;
				}
				out_binary(imm,6);	
				}
			}
			else if(inst[i][j]=='x'){			//debug1
				j++;
				imm=0;
				value=0;
				
				while(inst[i][j]==32) j++;
				
				if(inst[i][j]=='-'){
					j++;
					while(inst[i][j]==32) j++;
					while(inst[i][j]!=0&&inst[i][j]!=32){
					if(inst[i][j]>='0'&&inst[i][j]<='9')
					value=inst[i][j]-'0';
					else if(inst[i][j]>='A'&&inst[i][j]<='F')
					value=inst[i][j]-'A'+10;
					else if(inst[i][j]>='a'&&inst[i][j]<='f')
					value=inst[i][j]-'a'+10;
					j++;
					imm=imm*16+value;
				}	
				out_binary(-imm,6);
				}
				else{
				 	while(inst[i][j]!=0&&inst[i][j]!=32){
					if(inst[i][j]>='0'&&inst[i][j]<='9')
					value=inst[i][j]-'0';
					else if(inst[i][j]>='A'&&inst[i][j]<='F')
					value=inst[i][j]-'A'+10;
					else if(inst[i][j]>='a'&&inst[i][j]<='f')
					value=inst[i][j]-'a'+10;
					j++;
					imm=imm*16+value;
				}	
				out_binary(imm,6);	
				}			
			}
			else{
				k=0;
				while(inst[i][j]!=32&&inst[i][j]!=0){
					labelA[k]=inst[i][j];
					k++;
					j++;
				}
				labelA[k]='\0';
				for(k=0;k<table_line;k++){  
				flag2=strcmp(labelA,sym_t[k].label);
				if(flag2==0)
				out_binary(sym_t[k].addr-inst_add[i]-1,6);
				}		
			}
			printf("\n"); 	
		}
		//TRAP
		else if(result==10){
			printf("11110000");
			j=0;
			imm=0;
			while(inst[i][j]!='T'||inst[i][j+1]!='R') j++;
			j+=4;
			while(inst[i][j]!='x')	j++;
			j++;
			while(inst[i][j]==32) j++;
			while(inst[i][j]!=0&&inst[i][j]!=32){
					imm=imm*16+inst[i][j]-'0';
					j++;
			}
			out_binary(imm,8);
			printf("\n");		
		}
		//BR 8种情况 
		else if(result>=11&&result<=18){
			printf("0000");
			j=0;
			while(inst[i][j]!='B'||inst[i][j+1]!='R') j++;
			j+=2;
			if(inst[i][j]==32) printf("111");
			else if(inst[i][j]=='n'){
				if(inst[i][j+1]=='z'&&inst[i][j+2]=='p') printf("111");
				else if(inst[i][j+1]=='z'&&inst[i][j+2]==32) printf("110");
				else if(inst[i][j+1]=='p') printf("101");
				else if(inst[i][j+1]==32) printf("100");
				else ; 
			}
			else if(inst[i][j]=='z'){
				if(inst[i][j+1]=='p') printf("011");
				else  printf("010");
			}
			else {
				printf("001");
			}
			while(inst[i][j]!=32) j++;
			while(inst[i][j]==32) j++;
			if(inst[i][j]=='#'){			//输出 PCoffset9 
				j++;
				while(inst[i][j]==32) j++;
				imm=0;
				if(inst[i][j]=='-'){
					j++;
				while(inst[i][j]==32) j++;
				while(inst[i][j]!=32&&inst[i][j]!=0){
					imm=imm*10+inst[i][j]-'0';
					j++;
				}
				out_binary(-imm,9);
				}
				else{
				while(inst[i][j]==32) j++;
				while(inst[i][j]!=32&&inst[i][j]!=0){
					imm=imm*10+inst[i][j]-'0';
					j++;
				}
				out_binary(imm,9);	
				}
			}
			else if(inst[i][j]=='x'){			//debug1
				j++;
				imm=0;
				value=0;
				
				while(inst[i][j]==32) j++;
				
				if(inst[i][j]=='-'){
					j++;
					while(inst[i][j]==32) j++;
					while(inst[i][j]!=0&&inst[i][j]!=32){
					if(inst[i][j]>='0'&&inst[i][j]<='9')
					value=inst[i][j]-'0';
					else if(inst[i][j]>='A'&&inst[i][j]<='F')
					value=inst[i][j]-'A'+10;
					else if(inst[i][j]>='a'&&inst[i][j]<='f')
					value=inst[i][j]-'a'+10;
					j++;
					imm=imm*16+value;
				}	
				out_binary(-imm,9);
				}
				else{
				 	while(inst[i][j]!=0&&inst[i][j]!=32){
					if(inst[i][j]>='0'&&inst[i][j]<='9')
					value=inst[i][j]-'0';
					else if(inst[i][j]>='A'&&inst[i][j]<='F')
					value=inst[i][j]-'A'+10;
					else if(inst[i][j]>='a'&&inst[i][j]<='f')
					value=inst[i][j]-'a'+10;
					j++;
					imm=imm*16+value;
				}	
				out_binary(imm,9);	
				}			
			}
			else{
				k=0;
				while(inst[i][j]!=32&&inst[i][j]!=0){
					labelA[k]=inst[i][j];
					k++;
					j++;
				}
				labelA[k]='\0';
				for(k=0;k<table_line;k++){  
				flag2=strcmp(labelA,sym_t[k].label);
				if(flag2==0)
				out_binary(sym_t[k].addr-inst_add[i]-1,9);
				}
				
			}
			printf("\n");
		}
		//JMP JSRR
		else if(result==19||result==20){
			if(result==19) printf("1100");
			else printf("0100");
			printf("000");
			while(inst[i][j]<'0'||inst[i][j]>'9') j++;
			out_binary(inst[i][j]-'0',3);	
			printf("000000\n");
		}
		// JSR
		else if(result==21){
			printf("01001");
			j=0;
			while(inst[i][j]!='J'||(inst[i][j+1]!='S'||inst[i][j+2]!='R')) j++;
			j+=3;
			while(inst[i][j]==32) j++;
			if(inst[i][j]=='#'){			//输出 PCoffset11 
				j++;
				while(inst[i][j]==32) j++;
				imm=0;
				if(inst[i][j]=='-'){
					j++;
				while(inst[i][j]==32) j++;
				while(inst[i][j]!=32&&inst[i][j]!=0){
					imm=imm*10+inst[i][j]-'0';
					j++;
				}
				out_binary(-imm,11);
				}
				else{
				while(inst[i][j]!=32&&inst[i][j]!=0){
					imm=imm*10+inst[i][j]-'0';
					j++;
				}
				out_binary(imm,11);	
				}
			}
			else{
				k=0;
				while(inst[i][j]!=32&&inst[i][j]!=0){
					labelA[k]=inst[i][j];
					k++;
					j++;
				}
				labelA[k]='\0';
				for(k=0;k<table_line;k++){  
				flag2=strcmp(labelA,sym_t[k].label);
				if(flag2==0)
				out_binary(sym_t[k].addr-inst_add[i]-1,11);
				}
			}
			printf("\n");
		}
		//RTI RET
		else if(result==22||result==23){
			if(result==22) printf("1000000000000000\n");
			else printf("1100000111000000\n");
		}
		//24:.ORIG;  
		else if(result==24){
		    j=0;
			imm=0;
			while(inst[i][j]!='x')	j++;
			j++;
			while(inst[i][j]!=0&&inst[i][j]!=32){
					if(inst[i][j]>='0'&&inst[i][j]<='9')
					imm=inst[i][j]-'0';
					else if(inst[i][j]>='A'&&inst[i][j]<='F')
					imm=inst[i][j]-'A'+10;
					else if(inst[i][j]>='a'&&inst[i][j]<='f')
					imm=inst[i][j]-'a'+10;
					j++;
					out_binary(imm,4);
			}
			printf("\n");
			
		}
		//25: .FILL;
		else if(result==25){ //.FILL
			j=0;
			imm=0;
			while(inst[i][j]!='#'&&inst[i][j]!='x')	j++;		//输出 16位imm 
		
			if(inst[i][j]=='#'){
				j++;
				while(inst[i][j]==32) j++;
				if(inst[i][j]=='-'){
				j++;
				while(inst[i][j]==32) j++;
				while(inst[i][j]!=32&&inst[i][j]!=0){
					imm=imm*10+inst[i][j]-'0';
					j++;
				}
				out_binary(-imm,16);
				}
				else{
					while(inst[i][j]!=32&&inst[i][j]!=0){
					imm=imm*10+inst[i][j]-'0';
					j++;
					}
					out_binary(imm,16);	
				}
			}
			else if(inst[i][j]=='x'){
				j++;
				imm=0;
				value=0;
				while(inst[i][j]!=0&&inst[i][j]!=32){
					if(inst[i][j]>='0'&&inst[i][j]<='9')
					value=inst[i][j]-'0';
					else if(inst[i][j]>='A'&&inst[i][j]<='F')
					value=inst[i][j]-'A'+10;
					else if(inst[i][j]>='a'&&inst[i][j]<='f')
					value=inst[i][j]-'a'+10;
					j++;
					imm=imm*16+value;
				}	
				out_binary(imm,16);		
			}
			else;
			printf("\n");	
		}
		//26:.BLKW;  
		else if(result==26){
			j=0;
			imm=0;
			while(inst[i][j]!='#')j++;		//输出 16位imm 
			j++;
			while(inst[i][j]==32) j++;
			while(inst[i][j]!=32&&inst[i][j]!=0){
					imm=imm*10+inst[i][j]-'0';
					j++;
			}
			for(j=0;j<imm;j++){
				printf("0111011101110111\n");
			}
		}
		//27:.STRINGZ;
		else if(result==27){
			j=0;
			while(inst[i][j]!=34) j++;
			j++;
			while(inst[i][j]==32) j++;
			while(inst[i][j]!=34){
				out_binary(inst[i][j],16);
				printf("\n");
				j++;
			}
			printf("0000000000000000\n");
		}
		//29:HALT 30:GETC 31:OUT 32:PUTS 33:IN 34:PUTSP
		if(result>=29&&result<=34){
    		switch (result){
      	  	case 29:printf("1111000000100101\n");	break;
        	case 30:printf("1111000000100000\n");	break;
        	case 31:printf("1111000000100001\n");	break;
      	  	case 32:printf("1111000000100010\n");	break;
        	case 33:printf("1111000000100011\n");	break;
      	  	case 34:printf("1111000000100100\n");
      	 	}
   		}
    
    }      
	return 0;
}
//输入指令，删除空行，保存在inst[][]中，返回行数 
int input()
{	
	int i=0;
	int j=0;
	char ch;
	while(gets(inst[i])!=NULL){ 
		ch=*inst[i];
		j=0;
		while(ch==32){ 		//跳过空格 
		j++;
		ch=*(inst[i]+j);
		}
		if(ch==0) continue;
		i++;	
    }
    return i;
}

int judge(char*ptr,int i){
	char ch;
	char str[20];
	int j=0;
	int k=0;
	int p=0;
	int flag;
	ch=*ptr;
	
	while(ch==32) { 		//跳过空格 
		j++;
		ch=*(ptr+j);
	}
	while(ch!=32&&ch!=0){	//获取最前面的连续字符 
		str[k]=ch;
		k++;
		j++;
		ch=*(ptr+j);
	}
	str[k]='\0';
	while(k){				//35种保留字 
		for(p=0;p<35;p++){
		 	flag=strcmp(str,saved_sy[p]);
			if(flag==0) return p;
		} 
		k=0;				//出现标签时，获取真正的保留命令
			while(ch==32) { 		
			  	j++;
				ch=*(ptr+j);
			}
			while(ch!=32&&ch!=0){
				str[k]=ch;
				k++;
				j++;
				ch=*(ptr+j);
			}
			str[k]='\0';
				for(p=0;p<35;p++){
		 	 		flag=strcmp(str,saved_sy[p]);
					if(flag==0) return p;
				}
		return -1;	 	
	}	
}

void set_table(char*ptr,int i)
{
	char ch;
	int j=0;
	int k=0;
	int p=0;
	int q=0;
	int flag;
	int match=0;
	char str[20];
	ch=*ptr;
	
	inst_add[i]=inst_add[i-1]+count+1;	//更新地址 
	count=0;
	//计算count 
	while(ch==32) { 		//跳过空格 
		j++;
		ch=*(ptr+j);	}
	while(ch!=32&&ch!=0){	//获取最前面的连续字符 
		str[k]=ch;
		k++;
		j++;
		ch=*(ptr+j);
	}
	str[k]='\0';
	q=j;						 
		for(p=0;p<35;p++){	//判断指令类型，与35种保留字进行比较，并判断有无label 
		 	flag=strcmp(str,saved_sy[p]);
		 	if(flag==0){	//没有label 
		 	if(p==26){		//.BLKW 
		 	j=0;
			while(inst[i][j]!='#')j++;		
			j++;
			while(inst[i][j]==32) j++;
			while(inst[i][j]!=32&&inst[i][j]!=0){
					count=count*10+inst[i][j]-'0';
					j++;
			}
			count--;		
			}
			else if(p==27){	//.SRTINGZ 
				j=0;
				while(inst[i][j]!=34) j++;
				j++;
				while(inst[i][j]==32) j++;
				while(inst[i][j]!=34){
					count++;
					j++;
				}	
			}	
			match++; 
			break;
			}	 	
		}
		
		if(match==0){		//有LABEL 
			memcpy(sym_t[table_line].label,str,sizeof(char)*20); //建立symbol_table 
		    sym_t[table_line].addr=inst_add[i];
	    	table_line++;
	    	//再次读取连续字符，计算count 
	    	k=0;	
	    	while(ch==32){ 		
			  	q++;
				ch=*(ptr+q);
			}
			while(ch!=32&&ch!=0){
				str[k]=ch;
				k++;
				q++;
				ch=*(ptr+q);
			}
			str[k]='\0';	
	    	
	    	for(p=0;p<35;p++){	//35种保留字
		 	flag=strcmp(str,saved_sy[p]);
		 	if(flag==0){	
		 	if(p==26){	
		 	j=0;
			while(inst[i][j]!='#')j++;		
			j++;
			while(inst[i][j]==32) j++;
			while(inst[i][j]!=32&&inst[i][j]!=0){
					count=count*10+inst[i][j]-'0';
					j++;
				}
			count--;		
			}
			else if(p==27){
				j=0;
				while(inst[i][j]!=34) j++;
				j++;
				while(inst[i][j]==32) j++;
				while(inst[i][j]!=34){
					count++;
					j++;
				}	
				}	
			}	 	
		}	
		}
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
