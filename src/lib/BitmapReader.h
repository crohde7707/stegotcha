// LSB_Steg Header File
//

#define SUCCESS 0
#define FAILURE -1

// the following structure information is taken from wingdi.h

/* constants for the biCompression field
#define BI_RGB        0L	// (no compression)
#define BI_RLE8       1L	// (run-length encoding, 8 bits)
#define BI_RLE4       2L	// (run-length encoding, 4 bits)
#define BI_BITFIELDS  3L
#define BI_JPEG       4L
#define BI_PNG        5L

// this structure defines the file header format for a bitmap file
typedef struct tagBITMAPFILEHEADER // (14 bytes)
{
        WORD    bfType;					// ASCII "BM"
        DWORD   bfSize;					// total length of bitmap file
        WORD    bfReserved1;			// reserved
        WORD    bfReserved2;			// reserved
        DWORD   bfOffBits;				// offset to start of actual pixel data
} BITMAPFILEHEADER;

// this structure defines the header which describes the bitmap itself
typedef struct tagBITMAPINFOHEADER // (40 bytes)
{
        DWORD      biSize;				// size of BITMAPINFOHEADER
        LONG       biWidth;				// width in pixels
        LONG       biHeight;			// height in pixels
        WORD       biPlanes;			// always 1
        WORD       biBitCount;			// color bits per pixel
        DWORD      biCompression;		// BI_RGB, BI_RLE8, BI_RLE4
        DWORD      biSizeImage;			// total bytes in image
        LONG       biXPelsPerMeter;		// 0, or optional horizontal resolution
        LONG       biYPelsPerMeter;		// 0, or optional vertical resolution
        DWORD      biClrUsed;			// colors actually used (normally zero, can be lower than biBitCount)
        DWORD      biClrImportant;		// important colors actualy used (normally zero)
} BITMAPINFOHEADER;	

typedef struct tagRGBQUAD
{
        BYTE    rgbBlue;
        BYTE    rgbGreen;
        BYTE    rgbRed;
        BYTE    rgbReserved;
} RGBQUAD;

//*/

