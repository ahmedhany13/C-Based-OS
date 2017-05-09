void printString(char *);
int mod(int, int);
int div(int, int);
void readString(char *);
void readSector(char *, int);
void handleInterrupt21(int , int , int , int);

int main()
{
    //char* line[80];

    // printString("Hello World\0" );

    //printString("Enter a line:\0");
    //readString(line);
    //printString(line);
    // char buffer[512];
    // readSector(buffer, 30);
    // printString(buffer);
    //makeInterrupt21();
    //interrupt(0x21,0,0,0,0);
    char line[80];
    makeInterrupt21();	
    interrupt(0x21, 1, line, 0, 0);
    interrupt(0x21, 0, line, 0, 0);
    while (1)
    {
    }
    return 0;
}

void printString(char *chars)
{
    int k = 0;
    while (chars[k] != '\0')
    {
	interrupt(0x10, 0xE * 256 + chars[k], 0, 0, 0);
	k++;
    }
    interrupt(0x10, 0xE * 256 + '\n', 0, 0, 0);
    interrupt(0x10, 0xE * 256 + '\r', 0, 0, 0);
}
 

void readString(char *chars)
{   int a;
    int k = 0;
   

    while (1)
    {
	a = interrupt(0x16, 0, 0, 0, 0);
	if (k > 0 && a == 0x8 )
	{		    // 0x8 da elbackspace when its pressed
	    chars[k] = 0x0; // end of string
	    interrupt(0x10, 0xE * 256 + 0x8, 0, 0, 0);
	    interrupt(0x10, 0xE * 256 + 0x0, 0, 0, 0);
	    interrupt(0x10, 0xE * 256 + 0x8, 0, 0, 0);
	    k--;
	}
	else if (a == 0xd)
	{ //enter key pressed
	    chars[k] = 0xd;
	    chars[k + 1] = 0xa;
	    chars[k + 2] = 0x0;
	    interrupt(0x10, 0xE * 256 + '\n', 0, 0, 0);
	    interrupt(0x10, 0xE * 256 + '\r', 0, 0, 0);
	    return;
	}
	else
	{
	    chars[k] = a;
	    interrupt(0x10, 0xE * 256 + chars[k], 0, 0, 0);
	    k++;
	}
    }
}

int mod(int a, int b)
{
    int res = a;
    while (res >= b)
    {
	res = res - b;
    }
    return res;
}

int div(int a, int b)
{
    int res = 0;
    while (a >= b)
    {
	a = a - b;
	res++;
    }
    return res;
}

void readSector(char *buffer, int sector)
{

    int reasector = mod(sector, 18) + 1;
    int h = mod(div(sector, 18), 2);
    int t = div(sector, 36);
    interrupt(0x13, 2 * 256 + 1, buffer, t * 256 + reasector, h * 256 + 0);
}

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
    if (ax == 0)
    {
	printString(bx);
    }
    else if (ax == 1)
    {
	readString(bx);
    }
    else if (ax == 2)
    {
	readSector(bx, cx);
    }
    else if (ax >= 3)
    {
	printString("an error\0");
    }
}
