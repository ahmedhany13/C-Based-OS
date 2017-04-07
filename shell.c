
void Print(char buff);
void Input(char buffer);
void printChar(char ch);





main(){
	char buffer[80];
	int i = 0;
	Print("Welcome");
	Print("\r\n");
	Print("Shell-> ");

	while(1){

		interrupt(0x21,0,buffer,0,0);
		Print("\r\n");
		Print("Shell-> ");
		interrupt(0x21, 1, buffer, 0, 0);
		Input(buffer);
		for(i=0; i < 80; i++)
		{
			buffer[i]='\0';
		}

	}
}




void Input(char* buff){
       
	

	char fileBuffer[13312];
	char fileName[7];
	int index;
	int i = 0;
	char b[512];



	 if (buff[index]=='v' && buff[index+1]=='i' && buff[index+2]=='e' && buff[index+3]=='w' ){

		index = index + 5;

		for(i=0;i<6;i++){
			fileName[i] = buff[index+i];
		}
		fileName[6] = "\0";

		interrupt(0x21, 3, fileName, fileBuffer, 0); /*read el file fl buffer*/
		interrupt(0x21, 0, fileBuffer, 0, 0); /*print elfile*/

	}

	

else if (buff[index]=='e' && buff[index+1]=='x' && buff[index+2]=='e' &&  buff[index+3]=='c' && buff[index+4]=='u' && buff[index+5]=='t' && buff[index+6]=='e'){

		index = index + 8;

		for(i=0;i<6;i++){
			fileName[i] = buff[index+i];
		}
		fileName[6] = "\0";

		interrupt(0x21, 4, fileName, 0x2000, 0);

}
else{
		Print("Bad Command!");
		Print("\r\n");
		
	}


}
 
void printChar(char ch){
	char* chars[2];
	chars[0] = ch;
	chars[1] = '\0';
	Print(chars);
}

void Print(char* buff){


	interrupt(0x21,0,buff,0,0);

}
