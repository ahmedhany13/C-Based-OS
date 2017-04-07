void printString(char *);
void readString(char *);
int mod(int, int);
int div(int, int);
void readSector(char *, int);
void readFile(char *, char *);
void executeProgram(char *, int);
void terminate();
void handleInterrupt21(int, int, int, int);

int main()
{
    makeInterrupt21();
    printString("sada");
    interrupt(0x21, 4, "shell\0", 0x2000, 0);
    while (1)
        ;
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
{
    int a;
    int k = 0;

    //AX, BX, CX,  DX

    while (1)
    {
        a = interrupt(0x16, 0, 0, 0, 0);
        if (k > 0 && a == 0x8)
        {                   // 0x8 da elbackspace when its pressed
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

void readFile(char *fileName, char *buffer)
{
    int fileChanger = 0;
    int startingSector;
    int i;
    int j;
    int sec;
    int notSameChar = 0;
    int result = 20;
    int n;
    int incrementerOfAddress;
    int dirSectors[27];
    char directory[512];
    readSector(directory, 2);

    //finding the file location
    for (i = 0; i < 16; i++)
    {

        for (j = 0; j < 6; j++)
        {
            if (directory[j + fileChanger] == fileName[j])
            {
                notSameChar = 0;
            }
            else
            {
                notSameChar = 1;
            }
            if (notSameChar == 1)
            {
                break;
            }
        }
        //go to the next file
        fileChanger += 32;

        if (notSameChar == 0)
        {
            //i is the no. of file .
            result = i;
        }

        if (result != 20)
        {
            //we found the file then go out of the loop
            break;
        }
    }

    //if didn't found the file
    if (result == 20)
    {
        return;
    }

    else
    {
        //load file sector by sector
        startingSector = result * 32 + 6;
        //(+6 , 6 for the next file name )
        for (n = 0; n < 26; n++)
        {
            dirSectors[n] = directory[startingSector + n];
        }
        dirSectors[26] = 0;
        incrementerOfAddress = 0;
        for (sec = 0; dirSectors[sec] != 0x0; sec++)
        {
            readSector(buffer + incrementerOfAddress, dirSectors[sec]);
            incrementerOfAddress += 512;
        }
    }
}

void executeProgram(char *name, int segment)
{
    //step 2
    int currentFromBuffer = 0;

    char buffer[13312]; /*this is the maximum size of a file*/

    //step 1
    readFile(name, buffer);
    for (currentFromBuffer = 0; currentFromBuffer < 13312; currentFromBuffer++)
    {
        putInMemory(segment, currentFromBuffer, buffer[currentFromBuffer]);
    }
    //step 3
    launchProgram(segment);
}

void terminate()
{
    char shell[6];
    shell[0] = 's';
    shell[1] = 'h';
    shell[2] = 'e';
    shell[3] = 'l';
    shell[4] = 'l';
    shell[5] = 0x0;
    interrupt(0x21, 4, shell, 0x2000, 0);
}

void writeSector(char *buffer, int sector)
{
    //relative sector number
    int reasector = mod(sector, 18) + 1;
    //head number
    int h = mod(div(sector, 18), 2);
    //track number
    int t = div(sector, 36);

    //AX, BX, CX,  DX
    int ax = 3 * 256 + 1;
    int bx = buffer;
    int cx = t * 256 + reasector;
    int dx = h * 256 + 0;
    interrupt(0x13, ax, bx, cx, dx);
}

void deleteFile(char *name)
{
    
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
    else if (ax == 3)
    {
        readFile(bx, cx);
    }

    else if (ax == 4)
    {
        executeProgram(bx, cx);
    }
    else if (ax == 5)
    {
        terminate();
    }
    else if (ax == 6)
    {
        writeSector(bx, cx);
    }
    else if (ax > 6)
    {
        printString("Interrupt error\0");
    }
}
