// main.cpp
//
// this program reads a wave file and displays the header info
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wave.h"

WAVEFORMATEX *pHeader;

void hideSecretMessage(BYTE *data) {
    
    
    printf("inside function\n");
}

int writeChunkHeader(FILE *fptr, W_CHUNK *pChunk) {
    int y;

    // size = 1, count = 8 bytes
    y = (int) fwrite(pChunk, 1, 8, fptr);
	if(y != 8) return(FAILURE);

	return(SUCCESS);
}

void writeChunkData(FILE *fptr, BYTE *data, int size) {
    int tmp, x;

	tmp = size%2;	// size MUST be WORD aligned
	if(tmp != 0) size = size + 1;

	x = (int) fwrite(data, 1, size, fptr);
	if(x != size)
	{
		printf("\n\nError writing chunkd data!\n\n");
	}
    	
}

int readChunkHeader(FILE *fptr, W_CHUNK *pChunk)
{
    int x;

	// size = 1, count = 8 bytes
	x = (int) fread(pChunk, 1, 8, fptr);
	if(x != 8) return(FAILURE);

	return(SUCCESS);
} // readChunkHeader

void printData(BYTE *ptr){
    
        printf("This is the data:\n");
        printf("%x\n",*ptr);
    
}

// reads in the data portion of a chunk
BYTE *readChunkData(FILE *fptr, int size)
{
	BYTE *ptr;
	int tmp, x;

	tmp = size%2;	// size MUST be WORD aligned
	if(tmp != 0) size = size + 1;

	ptr = (BYTE *) malloc(size);
	if(ptr == NULL)
	{
		printf("\n\nError, could not allocate %d bytes of memory!\n\n", size);
		return(NULL);
	}

	x = (int) fread(ptr, 1, size, fptr);
	if(x != size)
	{
		printf("\n\nError reading chunkd data!\n\n");
		return(NULL);
	}
    printData(ptr);
	return(ptr);	
} // readChunkData

// prints out wave format info
void printFormat(W_FORMAT fmt)
{
	printf("\n\nWAVE FORMAT INFO\n\n");
	printf("Compression Code:		%d\n", fmt.compCode);
	printf("Number of Channels:		%d\n", fmt.numChannels);
	printf("Sample Rate: 			%d\n", fmt.sampleRate);
	printf("Average Bytes/Second:		%d\n", fmt.avgBytesPerSec);
	printf("Block Align: 			%d\n", fmt.blockAlign);
	printf("Bits per Sample: 		%d\n", fmt.bitsPerSample);
	return;
} // printFormat

int main(int argc, char *argv[])
{
	FILE *fptr, *fptrOut;
	DWORD fileSize;
	int x, cnt, dataFlag, formatFlag, y;
	W_CHUNK chunk[MAX_CHUNKS];		// assuming max of 8 chunks, should only be 3 for you
	BYTE *pChunkData[MAX_CHUNKS];
	W_FORMAT format;		// only 1 format chunk

	if(argc != 3)
	{
		printf("\n\nUSAGE:  wavereader filename.wav outfile.wav\n\n");
		return 1;
	}

	fptr = fopen(argv[1], "rb");
    fptrOut = fopen(argv[2], "wb");
	if(fptr == NULL)
	{
		printf("Could not find file named '%s.'\n\n", argv[1]);
		return 1;
	} else if (fptrOut == NULL) {
        printf("Could not find file named '%s.'\n\n", argv[2]);
        return 1;
	}

	printf("Successfully opened files:\nRead File: %s\nWrite File: %s\n", argv[1], argv[2]);

	// pChunk[0] is the chunk representing the file header
	x = readChunkHeader(fptr, &chunk[0]);
	if(x == FAILURE){
        printf("ReadChunkHeader Failure!\n");   
	    return 1;
	}
    y = writeChunkHeader(fptrOut, &chunk[0]);
    if(y == FAILURE){
        printf("WriteChunkHeader Failure!\n");
        return 1;
    }
    

	// check to make sure it is a RIFF file
	if(memcmp( &(chunk[0].chunkID), "RIFF", 4) != 0)
	{
		printf("\n\nError, file is NOT a RIFF file!\n\n");
		return 1;
	}
	fileSize = chunk[0].chunkSize + 8;

	// check to make sure it is a wave file
	pChunkData[0] = readChunkData(fptr, 4);

	if(memcmp( pChunkData[0], "WAVE", 4) != 0)
	{
		printf("\n\nError, file is not a WAVE file!\n\n");
		return 1;
	}
    
    writeChunkData(fptrOut, pChunkData[0], 4);

	// chunk[1] should be format chunk, but if not, skip
	cnt = 1;
	dataFlag = -1;
	formatFlag = -1;
	while(cnt < MAX_CHUNKS)
	{
		x = readChunkHeader(fptr, &chunk[cnt]);
        y = writeChunkHeader(fptrOut, &chunk[cnt]);
		if(x == FAILURE) {
            return 1;
		} else if(y == FAILURE) {
    	    return 1;   
		}
		// read in chunk data
		pChunkData[cnt] = readChunkData(fptr, chunk[cnt].chunkSize);
		if(pChunkData[cnt] == NULL) return 1;
        writeChunkData(fptrOut, pChunkData[cnt], chunk[cnt].chunkSize);

		if(memcmp( &(chunk[cnt].chunkID), "data", 4) == 0) {
		    dataFlag = cnt;	// if find data chunk, take note
		    printf("Found data before fmt\n");
        }

		if(memcmp( &(chunk[cnt].chunkID), "fmt ", 4) == 0)
		{
			formatFlag = cnt;	//	marks which chunk has format data
			break;	// found format chunk
		}

		cnt++;
	}
	if(cnt == MAX_CHUNKS)
	{
		printf("\n\nError, format chunk not found after 8 tries!\n\n");
		return 1;
	}

	// check format size to make sure this is not a fancy WAVE file
	if(chunk[cnt].chunkSize != 16)
	{
		printf("\n\nError, this WAVE file is not a standard format - we will not use this one!\n\n");
		return 1;
	}

	// put format chunk in our special variable
	// format chunk data already contained in pChunkData[cnt]
    memcpy(&format, pChunkData[cnt], 16);

	// make sure we are working with uncompressed PCM data
	if(format.compCode != 1)
	{
		printf("\n\nError, this file does not contain uncompressed PCM data!\n\n");
		return 1;
	}
	printFormat(format);

	if(dataFlag == -1)	// have not found data chunk yet
	{
		while(cnt < MAX_CHUNKS)
		{
			x = readChunkHeader(fptr, &chunk[cnt]);
            y = writeChunkHeader(fptrOut, &chunk[cnt]);
			if(x == FAILURE) {
                return 1;
			} else if(y == FAILURE) {
    		    return 1;   
			}

			// read in chunk data
			pChunkData[cnt] = readChunkData(fptr, chunk[cnt].chunkSize);
			if(pChunkData[cnt] == NULL) return 1;

			if(memcmp( &(chunk[cnt].chunkID), "data", 4) == 0)
			{
                printf("Found data after fmt\n");
				dataFlag = cnt;	// found data chunk
				break;
			}

			cnt++;
		}
	}

	// pChunkData[dataFlag] is a pointer to the begining of the WAVE data
//	hideSecretMessage(pChunkData[dataFlag]);
    // if 8 bit, then it is unsigned	0 to 255
	// if 16 bit, then it is signed		-32768 to +32767
	// ask me any other questions
	// the web page should answer others

    printf("Initiating Hiding of Message\n");
    hideSecretMessage(pChunkData[dataFlag]);
    printf("Message Hidden, writing out data\n");
    writeChunkData(fptrOut, pChunkData[cnt], chunk[cnt].chunkSize);

	printf("\n");

	fclose(fptr);
	return 0;
} // main
