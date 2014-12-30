#include<stdio.h>
#pragma pack(2)

typedef struct {
    char signature[2];
    unsigned int fileSize;
    unsigned int reserved;
    unsigned int offset;
} BmpHeader;

typedef struct {
    unsigned int headerSize;
    unsigned int width;
    unsigned int height;
    unsigned short planeCount;
    unsigned short bitDepth;
    unsigned int compression;
    unsigned int compressedImageSize;
    unsigned int horizontalResolution;
    unsigned int verticalResolution;
    unsigned int numColors;
    unsigned int importantColors;

} BmpImageInfo;

typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} Rgb;


int main( void ) {

	FILE *inputImageFile; // pointer to input image file
	FILE *outputPixelDataFile; // pointer to output pixel data file
	BmpHeader header;     // variable of structure BmpHeader
	BmpImageInfo info;    // variable of structure BmpImageInfo
	Rgb pixelData;        // variable of structure Rgb


	int imageArea = 0, read = 0;
	int i = 0, j = 0;     // counter variables for loops
	int red = 0, green = 0, blue = 0;   // flag variables
	float redCost = 0, greenCost = 0, blueCost = 0, totalCost = 0;
	float redPercentage = 0, greenPercentage = 0, bluePercentage = 0;
	char fileName[50];
	char pixelInfo[50];
	char padding[50];

	printf("Enter the file name : ");
	gets(fileName);
	printf("Opening file %s for reading.\n", fileName);

	inputImageFile = fopen(fileName, "rb");
	outputPixelDataFile = fopen("PixelDataFile.txt", "w");

	// Exception handling

	if ( !inputImageFile ) {
		printf("Error opening file %s.\n", inputImageFile);
	    return -1;
	}

	if ( !outputPixelDataFile ) {
        printf("Error opening file %s.\n", outputPixelDataFile);
        return -1;
	}

	if( fread(&header, 1, sizeof(BmpHeader), inputImageFile) != sizeof(BmpHeader) ) {
		printf("Error reading Bmp header.\n");
	    return -1;
	}

	if( fread(&info, 1, sizeof(BmpImageInfo), inputImageFile) != sizeof(BmpImageInfo) ) {
		printf("Error reading image info.\n");
	    return -1;
	}

	if(info.bitDepth != 24) {
        printf("Please input a 24 BIT Bmp image");
        return -1;
	}

    // End exception handling

	for(i = 0; i < info.height; i++) {
		read = 0; // initializing read to zero every time the loop runs

        for(j = 0; j < info.width; j++) {

			if( fread(&pixelData, 1, sizeof(Rgb), inputImageFile) != sizeof(Rgb) ) {
				printf("Error reading pixel!\n");
				return -1;
			}

			read += sizeof(Rgb);
			sprintf(pixelInfo, "Pixel %d: %3u %3u %3u\n", j+1, pixelData.red, pixelData.green, pixelData.blue);
            fwrite(pixelInfo, 1, sizeof(pixelInfo), outputPixelDataFile);

			if(pixelData.red != 0) {
                red++;
			}

			if(pixelData.green != 0) {
                green++;
			}

			if(pixelData.blue != 0) {
                blue++;
			}



		}

		if( read % 4 != 0 ) {
			read = 4 - (read % 4);
			sprintf(padding, "Padding: %d bytes\n", read);
			fread(&pixelData, read, 1, inputImageFile);
		}

	}

	printf( "Done.\n" );

	fclose(inputImageFile);
	fclose(outputPixelDataFile);

	printf("\nBMP-Info:\n");
	printf("Width x Height: %u x %u\n", info.width, info.height);
	printf("Depth: %u\n", info.bitDepth);

	imageArea = info.width * info.height;

	redPercentage = (red * 100)/ imageArea;
	greenPercentage = (green * 100)/ imageArea;
	bluePercentage = (blue * 100)/ imageArea;

	printf("\nThe percentages are :\n\t\t    red=\t%f \n\t\t    green=\t%f\n\t\t    blue=\t%f\n", redPercentage, greenPercentage, bluePercentage);



	printf("\n\nEnter the price of red color : ");scanf("%f", &redCost);
	printf("Enter the price of green color : ");scanf("%f", &greenCost);
	printf("Enter the price of blue color : ");scanf("%f", &blueCost);

    totalCost = (redPercentage * redCost) + (greenPercentage * greenCost) + (bluePercentage * blueCost);

	printf("\nThe cost is Rs %.2f", totalCost);




    getchar();
	getchar();
	return 0;

}


