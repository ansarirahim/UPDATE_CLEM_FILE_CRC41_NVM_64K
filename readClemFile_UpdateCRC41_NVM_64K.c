
// NVMpage NVMpages[NVM_SIZE];
//A. R. Ansari listed code to read line by line
//gcc readFileLinebyLine.c -o readFileLinebyLine.c
//Ver 1.00 Rev1.00
//For read clemfile nvm 64K.
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
/*
struct pageData{
//    unsigned int pageNumber;
    unsigned char page[256];

};*/
typedef unsigned char BYTE;
//pageData nPageData[256];
#define NVM_SIZE (0x100UL)
//65535
#define _BLANK_ 0
#define _NOT_BLANK_ 1
typedef struct NVMpage
{
    BYTE dataProperty;
    
    unsigned char page[256];
    unsigned int pageNo;

} nvmPg;
// NVMpage NVMpages[NVM_SIZE];
//(64*1024)
//NVMpage NVMpages[NVM_SIZE];//65535];
nvmPg npages[NVM_SIZE]; //256];//NVM_SIZE]; //65535];



BYTE getNVM_CRC(BYTE *CKBlock, int Length, BYTE Seed)
{
    BYTE val, y, crc, bbit1;
    int k;
    crc = Seed;
    for (k = 0; k < Length; k++)
    {
        val = CKBlock[k];
        crc = crc ^ ((val << 4));
        for (y = 0; y < 8; y++)
        {
            bbit1 = crc & 0x80;
            if (bbit1)
            {
                crc = crc ^ 74;
                bbit1 = 1;
            }
            else
                bbit1 = 0;
            crc = ((crc << 1) + bbit1);
        }
        crc = crc ^ 82;
        crc = crc ^ (val >> 4);
    }

    return (crc);
}

BYTE CharToHex(char ch)
{
    // Convert to upper case
    ch = toupper(ch);

    // Parse nibble
    if (ch >= '0' && ch <= '9')
    {
        return (ch - '0');
    }
    else if (ch >= 'A' && ch <= 'F')
    {
        return (ch - 'A' + 10);
    }
    else
    {
        // Bad input character
        //    assert(false);
        return 0; // to suppress warning C4715
    }
}
BYTE ParseByte(const char *str) //const char *str)
{

    char highNibbleChar = str[0];
    char lowNibbleChar = str[1];

    BYTE highNibble = CharToHex(highNibbleChar);
    BYTE lowNibble = CharToHex(lowNibbleChar);

    return (lowNibble + (highNibble << 4));
}
char hexASCII_addr[2];
char hexASCII_index[2];
char hexASCII_data[64];
#define ADDRESS_LENGTH 12

unsigned long clemFullAddress = 0;
BYTE hexASCII_full_address[ADDRESS_LENGTH];
#define START_ADDRESS_INDEX 0
typedef struct addressSturture
{
    unsigned int address;
    int addresslength;
} memoryAddress;
memoryAddress clemFileMemoryAddress;
#define MINMUM_ADDRESS_LENGTH 4

// static inline void swapObjects(char *o1, char *o2, int size)
// {
//     for (int k = 0; k < size; k++, o1++, o2++)
//     {
//         char tmp = *o1;
//         *o1 = *o2;
//         *o2 = tmp;
//     }
// }
void rvereseArray(BYTE arr[], int start, int end)
{
    int temp;
    while (start < end)
    {
        temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}
#define MAX_DATA_BYTES 64
typedef struct dataBytes
{
    BYTE data[MAX_DATA_BYTES];
    int data_len;
} dataBytes;
dataBytes dBytes;

int asciiHexDataToBytes(char *s)
{
    int i = 0;
    if (*s == '\0')
        return -1;
    dBytes.data_len = strlen(s);
    BYTE add[2];
    memset(dBytes.data, 0xff, MAX_DATA_BYTES);
    for (i = 0; i < dBytes.data_len / 2; i++)
    {

        add[0] = *s;
        *s++;
        add[1] = *s;

        dBytes.data[i] = ParseByte(add);

        *s++;

        if (*s == '\0')
            break;
    }
    dBytes.data_len /= 2;
    return 0;
}
int asciitoFullAddress(char *s)
{
    ///unsigned long fullAddress;
    unsigned int i = 0;
    ///unsigned int b = 0;
    BYTE add[2];
    BYTE address;
    clemFileMemoryAddress.address = 0;
    clemFileMemoryAddress.addresslength = strlen(s);
    if (clemFileMemoryAddress.addresslength < MINMUM_ADDRESS_LENGTH)
    {
        printf("\nNACK");//INVALID ADDRESS LENGTH");
        return 0;
    }
    //while (*s != '\0')
    memset(hexASCII_full_address, '\0', ADDRESS_LENGTH);
    for (i = 0; i < clemFileMemoryAddress.addresslength / 2; i++)
    {

        add[0] = *s;
        *s++;
        add[1] = *s;
        address = ParseByte(add);
        //hexASCII_full_address[(clemFileMemoryAddress.addresslength/2)-1-i]=ParseByte(add);///address;
        hexASCII_full_address[i] = ParseByte(add);
        *s++;

       
    ///    printf("\nParse byte ad=%.4X", hexASCII_full_address[i]); ///
        if (*s == '\0')
            break;
        ///clemFileMemoryAddress.address|=((unsigned long)address)<<(i*(8/2);

        //     hexASCII_data[i] = *s;
        //     printf("%c", *s);
        //    i++;
        //     *s++;
    }
  ////  printf("\nnormal=");
    clemFileMemoryAddress.address = 0;
//unsigned int i=0;
    for (i = 0; i < clemFileMemoryAddress.addresslength / 2; i++)
    {
        clemFileMemoryAddress.address |= hexASCII_full_address[i] << (8 * ((clemFileMemoryAddress.addresslength / 2 - 1) - i));
      ///  printf("%.2X", hexASCII_full_address[i]);
    }
    
    switch (clemFileMemoryAddress.addresslength)
    {
    case 4:
       // printf("\nLength address=%.4X", clemFileMemoryAddress.address);
        break;
    case 6:
      //  printf("\nLength address=%.6X", clemFileMemoryAddress.address);
        break;
    case 8:
       // printf("\nLength address=%.8X", clemFileMemoryAddress.address);
        break;
    case 10:
    ;
       // printf("\nLength address=%.10X", clemFileMemoryAddress.address);
    }

    
    return 1;
}
void splitLine(char *str, int init_size) /// nvmPg *nvmpage,char *str,int init_size)
{
    int k = 0;
    BYTE currentPage;
    BYTE currentByte;
    int length = 0;
    int length1 = 0;
    int i = 0;
    unsigned char pageIndex = 0;
    unsigned int higherByte;
    unsigned int lowerByte;
    unsigned int address;

    //int init_size = strlen(str);
    char delim[] = " ";
    //hexASCII_adress[8];
    //har hexAscc
    char *ptr = strtok(str, delim);
    ///currentPage=ParseByte(ptr+2);
    while (ptr != NULL)
    {
        switch (k)
        {
        case 0:
            // ///	printf("\n****\n");
            // //		*ptr++;
            // //			 *ptr++;
            // hexASCII_addr[0] = ptr[0];
            // hexASCII_addr[1] = ptr[1];
            // // hexASCII_addr[2]=ptr[2];
            // //hexASCII_addr[3]=ptr[3];
            // hexASCII_index[0] = ptr[2]; //6];
            // hexASCII_index[1] = ptr[3]; //7];
                                        //ParseByte(hexASCII_addr);
                                        //size_t arraylen= strlen(ptr);
                                        ///printf("\nlen of ptr=%u",arraylen);
                                        //clemFullAddress=
                                         if (asciitoFullAddress(ptr))
                                          {
                                              ;///printf("\n&&&&&&&\n");
                                          }  
                                         else
                                            {printf("\nNACK");//
                                            exit(0);}
            break;
        case 1:
          ///  printf("\n*****\n");
            if (asciiHexDataToBytes(ptr) < 0)
            {
                printf("\nNACK");
                exit(0);
            }
            else
            {
               
             currentPage= clemFileMemoryAddress.address>>8;
             pageIndex=clemFileMemoryAddress.address;
             k=dBytes.data_len+pageIndex;
 ///////////printf("\nAddress=%.4X,Cp=%.2X, pageIndex=%.2X,total index=%d\n",clemFileMemoryAddress.address,currentPage,pageIndex,k);
                for (i = 0; i < dBytes.data_len; i++) //
                {
                  
                    if((i+pageIndex)<256)
                   { ///printf("%.2X", dBytes.data[i]);
                   npages[currentPage].page[(pageIndex+i)%256]=dBytes.data[i%256];
              
                   }
                    else
                    {
                        ///printf("\nPI=%.2X=>data=%.2X\n", currentPage+1,dBytes.data[i]);
     npages[currentPage+1].page[(pageIndex+i)%256]=dBytes.data[i];
   
                    }
                }
                k=0;
            }
          
            
        }
        //	 printf("\nlength2=%d\n",length1);
        ptr = strtok(NULL, delim);
        k++;
    }
   
}
void splitLineBasic(char *str, int init_size)
{
    //int init_size = strlen(str);
    char delim[] = " ";

    char *ptr = strtok(str, delim);

    while (ptr != NULL)
    {
        ///printf("'%s'\n", ptr);
        ptr = strtok(NULL, delim);
    }
}
char *readline(FILE *fp, char *buffer)
{
    int ch;
    int i = 0;
    size_t buff_len = 0;

    buffer = malloc(buff_len + 1);
    if (!buffer)
        return NULL; // Out of memory

    while ((ch = fgetc(fp)) != '\n' && ch != EOF)
    {
        buff_len++;
        void *tmp = realloc(buffer, buff_len + 1);
        if (tmp == NULL)
        {
            free(buffer);
            return NULL; // Out of memory
        }
        buffer = tmp;

        buffer[i] = (char)ch;
        i++;
    }
    buffer[i] = '\0';

    // Detect end
    if (ch == EOF && (i == 0 || ferror(fp)))
    {
        free(buffer);
        return NULL;
    }
    if ((buffer[0] == ';') || (buffer[0] == '#'))
    {
        //	buffer++;//=buffer+1;
        //splitLine(buffer,i);
    }
    else
        splitLine(buffer, i);
    return buffer;
}

void lineByline(FILE *file)
{
   
   //// int linenumber = 0;
    char *s;
    //int j=0;
    while ((s = readline(file, 0)) != NULL)
    {
        ///puts(s);
        free(s);
       ////// linenumber++
      ///  printf("\nline number completed=%d", linenumber++);
     
    }
}

void Init()
{
    int i = 0;
    int j = 0;
    //char cmd_buffer[80];char cmd_buffer[80];
    for (j = 0; j < 256; j++)
    {npages[j].dataProperty=_BLANK_;
        for (i = 0; i < 256; i++)
            npages[j].page[i] = 0xff;
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int checkFileExist(char *s)
{

    {

        int i = 1;
        //char filename[20];
        //filename=argv[1];
        //size_t len = strlen(argv[1]);
        /*unsigned int len=0;
  while(*(msg+len) ) len++;*/
        unsigned int len = 0;
        {
            while (*(s + len))
                len++;
            printf("\nlen of finame=%u", len); //
            if (len > 0)
            {
                char buff[100] = "find /root/PROGRAM/CLemFile/003812.j19  -type f -exec grep -l 'searchstring' {} \\;";

                sprintf(buff, "find /root/PROGRAM/CLemFile/%s  -type f -exec grep -l 'searchstring' {} \\; > /dev/null 2>&1", s); //
                i = system(buff);
            }
            else
                i = 1;
        }
        switch (i)
        {

        case 0:
            printf("ACK\r\n");
            break;
        default:
        {
            printf("NACK\r\n");
            exit(0);
        }
        }
        //printf("system()=%d\n",i);//(system(buff));//
        return 0;
    }
}
char buff1[100];
int checkScript(char *s)
{
    sprintf(buff1, "CHECK_CLEM_FILE_NAME_FULL_PATH %s > /dev/null 2>&1", s);

    ////printf("\ncmd=%s\n\n",buff1);
    return (system(buff1));
}
char debugFilePath[100];                                                            ///="/root/PROGRAM/CLemFile/TEST_files/ClemFile_nvm_64k";
void printTFile(int newline, unsigned char *buff, unsigned int pageNUMBER, int crc) //,int len)
{
    int i = 0;
    /*
unsigned char x[256];

for(i=0;i<256;i++)
{
sprintf(x,"%.2X",buff[i];
}*/
    /* File pointer to hold reference of input file */

    FILE *fPtr;
    //char filePath[100];

    //    char dataToAppend[BUFFER_SIZE];
    /*  Open all file in append mode. */
    fPtr = fopen(debugFilePath, "a");
 
    if (fPtr == NULL)
    {
        /* Unable to open file hence exit */
        printf("\nUnable to open '%s' file.\n", debugFilePath);
        printf("Please check whether file exists and you have write privilege.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(fPtr, "\nPageNo(Hex)=%.8X", pageNUMBER); ///fprintf(fPtr, "\n",i);
    for (i = 0; i < 256; i++)
    {

        if (i % (newline) == 0)
            fprintf(fPtr, "\n", "");

        fprintf(fPtr, "%.2X", buff[i]);
    }
    fprintf(fPtr, "\nCRC(Decimal)=%d", crc);
    fclose(fPtr);
}
int main(int argc, char **argv)
{
    //char *fileName ="/root/CLEM_Files/test.txt";// "input-1.txt";
    int x = 0;
    int page = 0;
    int crc64K;
    char ch = 0;
    char cmd_buffer[80];
    //////int newl = atoi(argv[3]);
    /*if(checkScript(argv[1]))
{
printf("NACK\n");
return 0;
}*/

/*    for (ch = 0; ch < 100; ch++)
    {
        if (argv[2][ch] == '\0')
            break;
        debugFilePath[ch] = argv[2][ch]; //
    }
    ch = 0;
    // fp=freopen(NULL,"w",fp);
    fclose(fopen(debugFilePath, "w"));*/
    // int fdd= open(debugFilePath,O_RDONLY | O_WRONLY | O_TRUNC);
    memset(hexASCII_full_address, '\0', ADDRESS_LENGTH);
    //checkFileExist(argv[1]);
    /*
unsigned int len=0;
  while(*(argv[1]+len) ) len++;
///printf("len=%d\n",len);
if((len<24)||(argv[1][0]=='\0'))
{
printf("NACK\n");
return 0;
}*/
    Init();
    ////  system("/root/CLEM_Files/clearRearranged");
    ////  sleep(1);
    ///nvmPg  NVMpages[NVM_SIZE];
    FILE *file = fopen(argv[1], "r");

    lineByline(file);
    /*
for(x=0;x<256;x++)
{
if(x%16==0)
printf("\n");
printf("%.2X",npages[5].page[x]);
	}
*/
// int numberOfPagesHavingData=0;
//     for (page = 0; page < 256; page++)
//     {

//         for(x=0;x<256;x++)
//         {
//            // crc64K = getNVM_CRC(npages[page].page, 256, crc64K);
//             if(npages[page].page[x]<0xff)
//             {
//                 printf("\nDataPAge=%.8X",page);
//                 npages[page].dataProperty=_NOT_BLANK_;
//             numberOfPagesHavingData++;
//                 break;
//             }
        
//         }

//     }
//      printf("\nNumber of Pages having Data=%d",numberOfPagesHavingData);
//     return 0;
    crc64K = 80;
    x=0;
    for (page = 0; page < 256; page++)
    {

        //for(x=0;x<256;x++)
        {
            crc64K = getNVM_CRC(npages[page].page, 256, crc64K);
            
        ///   if(npages[page].dataProperty==_NOT_BLANK_)
          { 
         ///z      printTFile(newl, npages[page].page, page, crc64K);
         // x++;
        }
        }
    }



//      FILE *fPtr;
//     fPtr = fopen(debugFilePath, "a");
//     if (fPtr == NULL)
//     {
//         /* Unable to open file hence exit */
//         printf("\nUnable to open '%s' file.\n", debugFilePath);
//         printf("Please check whether file exists and you have write privilege.\n");
//         exit(EXIT_FAILURE);
//     }
//  fprintf(fPtr, "\nNumber of Pages Having Data (Decimal)=%d", x);
//     fclose(fPtr);
    sprintf(cmd_buffer, "CLEM_FILE_CRC41_NVM_64K_SAVE %d", crc64K); //

     system(cmd_buffer); //
    ///printf("%d\n",crc64K);
    /*for(page=0;page<256;page++)
{
printf("Enter any character: ");
		//read a single character
		ch=fgetc(stdin);
		
		if(ch==0x0A)
		{
///			printf("ENTER KEY is pressed.\n");
for(x=0;x<256;x++)

{
if(x%32==0)
printf("\n");
printf("%.2X",npages[page].page[x]);
}



//			break;
		}
		else
		{
			/////printf("%c is pressed.\n",ch);
		}
		//read dummy character to clear
		//input buffer, which inserts after character input
		ch=getchar();

}
*/

    return 0;
}

