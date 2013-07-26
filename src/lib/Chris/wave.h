// wave.h
// 
// contains wave structures
//

// built-in windows types
//typedef unsigned char		BYTE;
//typedef unsigned short	WORD;
//typedef unsigned int		DWORD;

#define SUCCESS 0
#define FAILURE -1
#define MAX_CHUNKS 8
typedef unsigned int DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;

typedef struct
{
	DWORD	chunkID;
	DWORD	chunkSize;
} W_CHUNK;

typedef struct
{
	WORD	compCode;				
    WORD    numChannels;         
    unsigned int   sampleRate;       //  Defined in windows...essentially is unsigned int from my understanding -Joe
    DWORD   avgBytesPerSec;   //  avgBytesPerSec = sampleRate * blockAlign 
    WORD    blockAlign;       //  blockAlign = bitsPerSample / 8 * numChannels
    WORD    bitsPerSample;    
	// WORD extraFormat;		// do not worry about this
} W_FORMAT;

typedef struct
{

} W_DATA;

typedef struct {
  WORD  wFormatTag;
  WORD  nChannels;
  DWORD nSamplesPerSec;
  DWORD nAvgBytesPerSec;
  WORD  nBlockAlign;
  WORD  wBitsPerSample;
  WORD  cbSize;
} WAVEFORMATEX;


