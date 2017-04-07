
void Print(char buff);
void Input(char buffer);
void printChar(char ch);
int isEqual(char input1[], char input2[]);
int GetSec(int x, int y);
void clear(char *, int);

main()
{
    char buffer[80];
    int i = 0;
    Print("Welcome");
    Print("\r\n");
    Print("Shell-> ");

    while (1)
    {

	interrupt(0x21, 0, buffer, 0, 0);
	Print("\r\n");
	Print("Shell-> ");
	interrupt(0x21, 1, buffer, 0, 0);
	Input(buffer);
	for (i = 0; i < 80; i++)
	{
	    buffer[i] = '\0';
	}
    }
}

void Input(char *buff)
{

    char fileBuffer[13312];
    char fileName[7];
    int index;
    int i = 0;
    char b[512];
    int indexnew;
    char fileNameone[7];
    char fileNametwo[7];
    int FileL = 0;
    int NoOfsectors;

    char BufferInput[80];
    char BufferData[512];
    int LineCount = 0;
    int BufferLength = 0;

    int input = 1;
    int Countinput = 0;

    if (isEqual("view\0", buff))
    {

	index = index + 5;

	for (i = 0; i < 6; i++)
	{
	    fileName[i] = buff[index + i];
	}
	fileName[6] = "\0";

	interrupt(0x21, 3, fileName, fileBuffer, 0); /*read el file fl buffer*/
	interrupt(0x21, 0, fileBuffer, 0, 0);	/*print elfile*/
    }

    else if (isEqual("execute\0", buff))
    {

	index = index + 8;

	for (i = 0; i < 6; i++)
	{
	    fileName[i] = buff[index + i];
	}
	fileName[6] = "\0";

	interrupt(0x21, 4, fileName, 0x2000, 0);
    }

    else if (isEqual("delete\0", buff))
    {

	index = index + 7;

	for (i = 0; i < 6; i++)
	{
	    fileName[i] = buff[index + i];
	}
	fileName[6] = "\0";

	interrupt(0x21, 7, fileName, 0, 0); /*deletefile */
	interrupt(0x21, 0, " deletion is done \0", 0, 0);
    }

    else if (isEqual("dir\0", buff))
    {
	interrupt(0x21, 9, 0, 0, 0);
	interrupt(0x21, 0, 0, 0, 0);	
	Print("Shell-> ");
    }

    else if (isEqual("copy\0", buff))
    {
	/* Get the file names Split at space char */

	indexnew = index + 5;
	i = 0;
	while (buff[indexnew] != 0x20 && buff[indexnew] != 0x0)
	{
	    fileNameone[i] = buff[indexnew];
	    indexnew++;
	    i++;
	}
	fileNameone[6] = '\0';

	i = 0;
	indexnew++;

	while (buff[indexnew] != 0x20 && buff[indexnew] != 0x0)
	{
	    fileNametwo[i] = buff[indexnew];
	    indexnew++;
	    i++;
	}

	fileNametwo[6] = '\0';
	/* get the file names. and read in filenameone */
	interrupt(0x21, 3, fileNameone, fileBuffer, 0);
	/* to know how many sectors do we need */

	while (fileBuffer[FileL] != 0x0)
	{
	    FileL++;
	}
	NoOfsectors = GetSec(FileL, 512);
	/* Write fileBuff with fileNameone inside of it to fileNametwo */

	interrupt(0x21, 8, fileNametwo, fileBuffer, NoOfsectors + 1);
    }

    else if (isEqual("create\0", buff))
    {
	index = index + 7;
	Print("Enter the Data: \r\n");
	Print("\t-:");
	for (i = 0; i < 6; i++)
	{
	    fileName[i] = buff[index + i];
	}
	fileName[6] = "\0";

	while (input || Countinput < 13312)
	{
	    /*to get input */
	    clear(BufferInput, 80);

	    interrupt(0x21, 1, BufferInput, 0, 0);
	    Print("\r\n");
	    Print("\t-:");

	    if (BufferInput[2] == 0x0)
	    {
		Print("END OF FILE");
		Print("\r\n");
		Print("Shell-> ");
		input = 0;
		break;
	    }
	    else
	    {

		for (i = 0; i < 80; i++)
		{
		    if (BufferInput[i] == 0x0 || BufferInput[i] == '\0' || BufferInput[i] == '\r' || BufferInput[i] == '\n' || BufferInput[i] == '\t')
		    {
			BufferInput[i] = 0x20;
		    }
		    BufferData[i + 80 * LineCount] = BufferInput[i];
		    Countinput++;
		}
		BufferLength++;
		LineCount++;
	    }
	}
	NoOfsectors = GetSec(BufferLength / 512);
	/* hacreate the file */
	interrupt(0x21, 8, fileName, BufferInput, BufferLength);
    }

    else
    {
	Print("Bad Command!");
	Print("\r\n");
    }
}

void printChar(char ch)
{
    char *chars[2];
    chars[0] = ch;
    chars[1] = '\0';
    Print(chars);
}

void Print(char *buff)
{

    interrupt(0x21, 0, buff, 0, 0);
}
void clear(char *buff, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
	buff[i] = 0x0;
    }
}

int GetSec(int x, int y)
{
    int a = 0;
    while (a * y <= x)
    {
	a = a + 1;
    }
    return a - 1;
}

int isEqual(char input1[], char input2[])
{
    int i = 1;
    int result = 1;
    for (i = 0; input1[i] != '\0' && input2[i] != '\0'; i++)
	if (input1[i] != input2[i])
	    return 0;

    return result;
}