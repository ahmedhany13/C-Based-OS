void printString(char*);
void readString (char []);
int div(int,int);
int mod(int,int);
void readSector(char*,int);
void handleInterrupt21(int, int,int,int);
void readFile(char[],char*);
void executeProgram(char*,int);
void terminate();
void writeSector(char*,int);
void deleteFile(char*);
void writeFile(char*,char*,int);
void handleTimerInterrupt(int, int);
void killProcess(int);

int activeProcess[8];
int processSP[8];
int currentProcess;
int timeUnit = 0;
int firstExecution = 0;


main()
{
	int i;
	for(i = 0;i<8;i++){
		activeProcess[i] = 0;
		processSP[i] = 0xFF00;
}
currentProcess = 0;
makeInterrupt21();
interrupt(0x21,4, "shell\0", 0x2000, 0);
//interrupt(0x21, 4, "hello1\0", 0, 0);
//interrupt(0x21, 4, "hello2\0", 0, 0);
makeTimerInterrupt();
while(1);
}
void printString (char* chars){
	char *ptr = chars;
	for(;*ptr !='\0';ptr++){
		interrupt(0x10, 0xE*256+*ptr, 0, 0, 0);
	}
	}
void readString (char line []){
	char c = 0x0;
	int i = 0;
	while (c != 0xd){
		c = interrupt(0x16, 0x0, 0x0, 0x0, 0x0);
		if (c == 0xd) {
			interrupt(0x10, 0xE*256+'\r', 0, 0);
			break;
		}
		else if(c == 0x8 && i != 0){
			interrupt(0x10, 0xE*256+0x8, 0, 0, 0);
			interrupt(0x10, 0xE*256+0x0, 0, 0, 0);
			interrupt(0x10, 0xE*256+0x8, 0, 0, 0);
			i--;
		}
				else {
					line[i]=c;
					interrupt(0x10, 0xE*256+line[i], 0, 0, 0);
					i++;
				}
	}

				line[i] = 0xa;
				interrupt(0x10, 0xE*256+line[i],0,0,0);
				line[i]=0x0;

}




int div(int x, int y){
	int result = 0;
	while(x>=y){
	x -= y;
	result ++;
}
return result;
}
int mod(int x, int y){
	if(x<y)
		return x;
	else
		return x-(div(x,y)*y);
}
void readSector(char* buffer, int sector){
	int rs = (mod(sector,18)) + 1;
	int head = mod(div(sector,18),2);
	int track = div(sector,36);
	interrupt(0x13,513,buffer,(track*256)+rs, head*256);


}
void handleInterrupt21(int ax, int bx,int cx,int dx){
	if(ax==0)
		printString(bx);
	else if(ax == 1)
		readString(bx);
	else if(ax == 2)
		readSector(bx,cx);
  else if(ax == 3)
  	readFile(bx,cx);
	else if(ax ==  4)
		executeProgram(bx);
	else if(ax == 5)
		terminate();
	else if(ax == 6)
		writeSector(bx,cx);
	else if(ax == 7)
		deleteFile(bx);
	else if(ax == 8)
		writeFile(bx,cx,dx);
	else if(ax == 9)
		killProcess(bx);
	else
		printString("Error");

}
void readFile(char fileName[],char buffer[]){
	char sectorFiles[512];
	int flag = 0;
	int k = 0;
	int i;
	int j;
  readSector(sectorFiles,2);
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
		while(sectorFiles[(i*32)+j] != 0x0 && k<26){
		readSector(buffer+(k*512),sectorFiles[(i*32)+j]);
		j++;
		k++;
	}
}else{
	return;
}

}

void executeProgram(char* name){
	char buffer[13312];
	int i;
	int segment;
	int j;
	setKernelDataSegment();
	for(j = 0;j<8;j++)
		if(activeProcess[j] == 0)
		break;

	if(j == 8)
	restoreDataSegment();
		else{
		activeProcess[j] = 1;
		processSP[j] = 0xFF00;
		restoreDataSegment();
		segment = (j+2)*0x1000;



	readFile(name,buffer);
	for(i=0;i<13312;i++){
		putInMemory(segment,i,buffer[i]);
	}
	for(i = 0;i<13312;i++){
		buffer[i]=0;
	}

	initializeProgram(segment);
}
}
void terminate(){
	setKernelDataSegment();
	activeProcess[currentProcess] = 0;
	processSP[currentProcess] = 0xFF00;
	while(1);
}
void writeSector(char* buffer,int sector){
	int rs = (mod(sector,18)) + 1;
	int head = mod(div(sector,18),2);
	int track = div(sector,36);
	interrupt(0x13,769,buffer,(track*256)+rs, head*256);
}
void deleteFile(char* name){
	char directory [512];
	char map [512];
	int flag = 0;
	int i;
	int j;
	int k;
	readSector(map,1);
	readSector(directory,2);
	for(i=0;i<16;i++){
		for(j=0;j<6;j++){
			if(name[j] == directory[(i*32)+j]){
			flag = 1;
		}
			else{
			flag = 0;
			break;
		}
		}
		if(flag == 1){
		directory[(i*32)] = 0x00;
		break;
	}
	}
	if(flag == 1){
	while(directory[(i*32)+j] != 0x00 && k<26){
	map[directory[(i*32)+j]] = 0x00;
	j++;
	k++;
}
}else{
	interrupt(0x10, 0xE*256+'F', 0, 0, 0);
	interrupt(0x10, 0xE*256+'i', 0, 0, 0);
	interrupt(0x10, 0xE*256+'l', 0, 0, 0);
	interrupt(0x10, 0xE*256+'e', 0, 0, 0);
	interrupt(0x10, 0xE*256+' ', 0, 0, 0);
	interrupt(0x10, 0xE*256+'n', 0, 0, 0);
	interrupt(0x10, 0xE*256+'o', 0, 0, 0);
	interrupt(0x10, 0xE*256+'t', 0, 0, 0);
	interrupt(0x10, 0xE*256+' ', 0, 0, 0);
	interrupt(0x10, 0xE*256+'f', 0, 0, 0);
	interrupt(0x10, 0xE*256+'o', 0, 0, 0);
	interrupt(0x10, 0xE*256+'u', 0, 0, 0);
	interrupt(0x10, 0xE*256+'n', 0, 0, 0);
	interrupt(0x10, 0xE*256+'d', 0, 0, 0);
	interrupt(0x10, 0xE*256+'!', 0, 0, 0);
	interrupt(0x10, 0xE*256+'\r', 0, 0, 0);
	interrupt(0x10, 0xE*256+'\n', 0, 0, 0);
	return;
}
writeSector(map,1);
writeSector(directory,2);
}
void writeFile(char* name,char* buffer,int secNum){
	char map [512];
	char directory [512];
	int i;
	int j=0;
	int n=0;
	int x=0;
	int flag =0;
	int k;
	readSector(map,1);
	readSector(directory,2);
	for(i = 0;i<16;i++){
		if(directory[i*32] == 0x00){
		while(j<6){
			if(name[n] != '\0'){
			directory[(i*32)+j] = name[n];
			j++;
			n++;
			flag = 1;
		}else{
			directory[(i*32)+j] = 0x00;
			j++;
		}
		}
		if(flag == 1)
		break;
	}
	}
	if(flag == 0 ) return;
	for(k = 3;k<512 && secNum!=0;k++){
		if(map[k]==0x00){
		map[k] = 0xFF;
		directory[(i*32)+j] = k;
		writeSector(buffer+(x*512),k);
		x++;
		j++;
		secNum--;
	}
	}

while(j<32){
directory[(i*32)+j]=0x00;
j++;
}

writeSector(map,1);
writeSector(directory,2);
}
void handleTimerInterrupt(int segment, int sp){
	if(firstExecution == 0){
		firstExecution = 1;
		timeUnit++;
		returnFromTimer(0x2000,processSP[0]);
	}
	else{
	int i = currentProcess + 1;
		if(timeUnit == 100){
			timeUnit = 0;
			processSP[currentProcess] = sp;
			while(1){
				if(i == 8)
					i=0;
				if(i == currentProcess)
				break;

				if(activeProcess[i] == 1){
					segment = (i+2) * 0x1000;
					sp = processSP[i];
					currentProcess = i;
					break;
				}
			i++;
				}
		}
		else{
		timeUnit++;
	}
	returnFromTimer(segment,sp);
}
}

void killProcess(int index){
	setKernelDataSegment();
	activeProcess[index] = 0;
	processSP[index] = 0xFF00;
	restoreDataSegment();
}
