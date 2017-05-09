int getSecNum(char*);
int mod(int,int);
int DIV(int,int);
main(){
  while(1){
    int secNum;
    char line[80];
    int i;
    int j=0;
    int k=0;
    int tens=0;
    int units=0;
    char directory[512];
    char buffer[13312];
    char buffer2[13312];
    char fileName [20];
    char fileName2 [20];
    interrupt(0x21,0,"SHELL>\0",0,0);
    interrupt(0x21,1,line,0,0);
    if(line[0] == 'v' && line[1] == 'i' && line[2] == 'e' && line[3] == 'w'){
            for(i = 5;(i<11) && (line[i]!='\0');i++)
            fileName[i-5]=line[i];
            fileName[i]='\0';
            interrupt(0x21,3,fileName,buffer,0);
            interrupt(0x21,0,buffer,0,0);
            interrupt(0x21,0,"\r\n\0",0,0);
            for(i=0;i<13312;i++)
            buffer[i]=0;
          } else if(line[0] == 'e' && line[1] == 'x' && line[2] == 'e' && line[3] == 'c' && line[4] == 'u' && line[5] == 't' && line[6] == 'e'){
            for(i = 8;(i<14) && (line[i] != '\0');i++)
            fileName[i-8]=line[i];
            fileName[i]='\0';
            interrupt(0x21,4,fileName,0,0);
          } else if(line[0] == 'd' && line[1] == 'e' && line[2] == 'l' && line[3] == 'e' && line[4] == 't' && line[5] == 'e'){
            for(i = 7;i<13 && line[i] != '\0';i++)
            fileName[i-7] = line[i];
            fileName[i] = '\0';
            interrupt(0x21,7,fileName,0,0);
          }else if(line[0] == 'c' && line[1] == 'o' && line[2] == 'p' && line[3] == 'y'){
            for(i = 5;i<11 && line[i]!=' ';i++)
            fileName[i-5] = line[i];
            fileName[i]='\0';
            for(j=i+1;j<i+6 && line[j]!='\0';j++)
            fileName2[j-(i+1)] = line[j];
            fileName2[j] = '\0';
            interrupt(0x21,3,fileName,buffer,0);
            secNum = getSecNum(fileName);
            interrupt(0x21,8,fileName2,buffer,secNum);
          }else if(line[0] == 'd' && line[1] == 'i' && line[2] == 'r'){
            interrupt(0x21,2,directory,2,0);
            for(i = 0;i<16;i++)
              if(directory[i*32] != 0x00){
                for(j = 0;j<6 && directory[(i*32)+j]!='\0';j++)
                  fileName[j] = directory[(i*32)+j];


                  secNum = getSecNum(fileName);
                  if(secNum>9){
                  units = mod(secNum,10);
                  tens = DIV(secNum,10);
                  fileName[j]=' ';
                  fileName[j+1]=tens+'0';
                  fileName[j+2]=units+'0';
                  fileName[j+3]='\n';
                  fileName[j+4]='\r';
                  }else{
                  fileName[j]=' ';
                  fileName[j+1]=secNum+'0';
                  fileName[j+2]='\n';
                  fileName[j+3]='\r';
                }
                  interrupt(0x21,0,fileName,0,0);

                  for(k = 0;k<20;k++)
                  fileName[k] = 0;
            }
          }else if(line[0] == 'c' && line[1] == 'r' && line[2] == 'e' && line[3] == 'a' && line[4] == 't' && line[5] == 'e'){
            for(i = 7;i<13 && line[i] != '\0';i++)
            fileName[i-7] = line[i];
            fileName[i] = '\0';
            while(1){
            interrupt(0x21,0,"Enter a line to be written to the file: ",0,0);
            interrupt(0x21,1,buffer,0,0);
            if(buffer[0]==0)
            break;
            else
            {
            for(k=0;buffer[k]!='\0';k++){
              buffer2[j] = buffer[k];
              j++;
            }
            buffer2[j]='\r';
            buffer2[j++]='\n';
            buffer2[j++]='\r';
              }
              for(k=0;k<13312;k++)
              buffer[k]=0;
          }
          while(j>0){
            j-=512;
            secNum++;
          }
          interrupt(0x21,8,fileName,buffer2,secNum);
          }
          else if(line[0] == 'k' && line[1] == 'i' && line[2] == 'l' && line[3] == 'l'){
            interrupt(0x21,9,line[5] - 48,0,0);
          }
          else{
    interrupt(0x21,0,"Bad Command!\r\n\0",0,0);
  }
  }
}

int getSecNum(char* fileName){
  char* directory;
  int c = 0;
  int i;
  char* sectorFiles;
  int flag = 0;
  int j;
  int k = 0;
  interrupt(0x21,2,sectorFiles,2,0);
  for(i=0;i<16;i++){
		for(j=0;j<6;j++){
			if(fileName[j] == sectorFiles[(i*32)+j]){
			flag = 1;
		}
			else{
			flag = 0;
			break;
		}
		}
		if(flag == 1)
		break;
	}
	if(flag == 1){
		while(sectorFiles[(i*32)+j] != 0x00 && k<26){
		c++;
    k++;
    j++;
	}
}
return c;
}
int mod(int x, int y){
	if(x<y)
		return x;
	else
		return x-(DIV(x,y)*y);
}
int DIV(int x, int y){
	int result = 0;
	while(x>=y){
	x -= y;
	result ++;
}
return result;
}
