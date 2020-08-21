
#include <citro2d.h>
#include <3ds.h>

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define TOP_SCREEN_WIDTH  400
#define TOP_SCREEN_HEIGHT 240

#define BOTTOM_SCREEN_WIDTH 320
#define BOTTOM_SCREEN_HEIGHT 240

#define RED_COLOR C2D_Color32(0xAD, 0x68, 0x5A, 255)

#define TEXT_DEFAULT C2D_AtBaseline | C2D_WithColor

C2D_TextBuf textBuf;

char inputtedText[150];
char morseOutput[150];
char asciiOutput[150];

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int init()
{	
	textBuf = C2D_TextBufNew(256);

	printf("\x1b[14;1HPress A to open keyboard.");
	printf("\x1b[16;1HPress Start to exit.");

	return 0;
}

void drawUIText()
{
	u32 clrRed = C2D_Color32(0xFF, 0x00, 0x00, 0xFF);

	C2D_TextBufClear(textBuf);
	C2D_Text input;
	C2D_TextParse(&input, textBuf, "INPUT");
    C2D_TextOptimize(&input);
    C2D_DrawText(&input, TEXT_DEFAULT, C2D_AlignCenter, TOP_SCREEN_HEIGHT / 4, 0.0f, 0.7f, 0.7f, clrRed);

	C2D_TextBufClear(textBuf);
	C2D_Text output;
	C2D_TextParse(&output, textBuf, "OUTPUT");
    C2D_TextOptimize(&output);
    C2D_DrawText(&output, TEXT_DEFAULT, C2D_AlignCenter, TOP_SCREEN_HEIGHT - TOP_SCREEN_HEIGHT / 4, 0.0f, 0.7f, 0.7f, clrRed);

	//Check if there are any text to be printed
	if(!(*inputtedText == 0)) 
	{
		C2D_TextBufClear(textBuf);
		C2D_Text inputOut;
		C2D_TextParse(&inputOut, textBuf, inputtedText);
    	C2D_TextOptimize(&inputOut);
    	C2D_DrawText(&inputOut, TEXT_DEFAULT, 5, TOP_SCREEN_HEIGHT / 4 + 17, 0.5f, 0.8f, 0.8f, clrRed);
	}

	if(!(*asciiOutput == 0)) 
	{
		C2D_TextBufClear(textBuf);
		C2D_Text asciiOut;
		C2D_TextParse(&asciiOut, textBuf, asciiOutput);
    	C2D_TextOptimize(&asciiOut);
    	C2D_DrawText(&asciiOut, TEXT_DEFAULT, 5, TOP_SCREEN_HEIGHT - TOP_SCREEN_HEIGHT / 4 + 17, 0.5f, 1.0f, 1.0f, clrRed);
	}

	if(!(*morseOutput == 0)) 
	{
		C2D_TextBufClear(textBuf);
		C2D_Text morseOut;
		C2D_TextParse(&morseOut, textBuf, morseOutput);
    	C2D_TextOptimize(&morseOut);
    	C2D_DrawText(&morseOut, TEXT_DEFAULT, 5, TOP_SCREEN_HEIGHT - TOP_SCREEN_HEIGHT / 4 + 17, 0.5f, 1.0f, 1.0f, clrRed);
	}
}

void textUpdates(touchPosition touch, u32 kHeld, u32 kDown)
{
	printf("\x1b[29;1HX Coordinate: %i",touch.px);
	printf("\x1b[30;1HY Coordinate: %i",touch.py);

	if (kHeld & KEY_TOUCH)
        printf("\x1b[2;0HIs Active: TRUE ");
    else
        printf("\x1b[2;0HIs Active: FALSE");
}

typedef struct
{
    char* morse;
    char ascii;
} morse_table_t;

int translateToMorse(char input[])
{
	morse_table_t table[52] = {
                          {".-  ", 'A'},
                          {"-...  ", 'B'},
                          {"-.-.  ", 'C'},
						  {"-..  ", 'D'},
						  {".  ", 'E'},
						  {"..-.  ", 'F'},
						  {"--.  ", 'G'},
						  {"....  ", 'H'},
						  {"..  ", 'I'},
						  {".---  ", 'J'},
						  {"-.-  ", 'K'},
						  {".-..  ", 'L'},
						  {"--  ", 'M'},
						  {"-.  ", 'N'},
						  {"---  ", 'O'},
						  {".--.  ", 'P'},
						  {"--.-  ", 'Q'},
						  {".-.  ", 'R'},
						  {"...  ", 'S'},
						  {"-  ", 'T'},
						  {"..-  ", 'U'},
						  {"...-  ", 'V'},
						  {".--  ", 'W'},
						  {"-..-  ", 'X'},
						  {"-.--  ", 'Y'},
						  {"--..  ", 'Z'},
						  
						  {".-  ", 'a'},
                          {"-...  ", 'b'},
                          {"-.-.  ", 'c'},
						  {"-..  ", 'd'},
						  {".  ", 'e'},
						  {"..-.  ", 'f'},
						  {"--.  ", 'g'},
						  {"....  ", 'h'},
						  {"..  ", 'i'},
						  {".---  ", 'j'},
						  {"-.-  ", 'k'},
						  {".-..  ", 'l'},
						  {"--  ", 'm'},
						  {"-.  ", 'n'},
						  {"---  ", 'o'},
						  {".--.  ", 'p'},
						  {"--.-  ", 'q'},
						  {".-.  ", 'r'},
						  {"...  ", 's'},
						  {"-  ", 't'},
						  {"..-  ", 'u'},
						  {"...-  ", 'v'},
						  {".--  ", 'w'},
						  {"-..-  ", 'x'},
						  {"-.--  ", 'y'},
						  {"--..  ", 'z'},
};

    char output[300];

    char* segment;
    segment = strtok(input, " ");
    memset(output, 0, 300);

    while(segment != NULL)
    {
        for (int s = 0; strlen(segment) > s; s++) {
            for(int i = 0; i < 52; i++)
            {
                if (!strncmp(&segment[s], &table[i].ascii, 1))
                {
                    strncat(output, table[i].morse, 150);
                }
            }
        }
        segment = strtok(NULL, " ");
    }   
	strcpy(morseOutput, output);

    return 0;
}

int translateFromMorse(char input[])
{
	morse_table_t table[26] = {
                          {".-", 'A'},
                          {"-...", 'B'},
                          {"-.-.", 'C'},
						  {"-..", 'D'},
						  {".", 'E'},
						  {"..-.", 'F'},
						  {"--.", 'G'},
						  {"....", 'H'},
						  {"..", 'I'},
						  {".---", 'J'},
						  {"-.-", 'K'},
						  {".-..", 'L'},
						  {"--", 'M'},
						  {"-.", 'N'},
						  {"---", 'O'},
						  {".--.", 'P'},
						  {"--.-", 'Q'},
						  {".-.", 'R'},
						  {"...", 'S'},
						  {"-", 'T'},
						  {"..-", 'U'},
						  {"...-", 'V'},
						  {".--", 'W'},
						  {"-..-", 'X'},
						  {"-.--", 'Y'},
						  {"--..", 'Z'},
};

    char* segment;
	char output[300];

    segment = strtok(input, "/");
	memset(output, 0, 300);

    while(segment != NULL)
    {
        for(int i = 0; i < 26; i++)
        {
            if (!strncmp(segment, table[i].morse, 4))
            {
               strncat(output, &table[i].ascii, 150);
            }
        }
        segment = strtok(NULL, "/");
    }
	strcpy(asciiOutput, output);

    return 0;
}

int main(int argc, char **argv)
{
	romfsInit();
	gfxInitDefault();

	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	//gfxSetWide(true); // Enable wide mode
	consoleInit(GFX_BOTTOM, NULL);

	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

 	init();//Init text in console

	// Create colors
	u32 clrWhite = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
	u32 clrGreen = C2D_Color32(0x00, 0xFF, 0x00, 0xFF);
	u32 clrRed   = C2D_Color32(0xFF, 0x00, 0x00, 0xFF);
	u32 clrBlue  = C2D_Color32(0x00, 0x00, 0xFF, 0xFF);
	u32 clrBlack = C2D_Color32(0x00, 0x00, 0x00, 0xFF);
	u32 clrClear = C2D_Color32(0x88, 0x88, 0x88, 0x7F); //Clear - Gray
	u32 clrCyan = C2D_Color32(0x00, 0x80, 0x80, 0xFF);

	u32 clrCircle1 = C2D_Color32(0xFF, 0x00, 0xFF, 0xFF);
	u32 clrCircle2 = C2D_Color32(0xFF, 0xFF, 0x00, 0xFF);
	u32 clrCircle3 = C2D_Color32(0x00, 0xFF, 0xFF, 0xFF);

	u32 clrSolidCircle = C2D_Color32(0x68, 0xB0, 0xD8, 0xFF);

	u32 clrTri1 = C2D_Color32(0xFF, 0x15, 0x00, 0xFF);
	u32 clrTri2 = C2D_Color32(0x27, 0x69, 0xE5, 0xFF);

	u32 clrRec1 = C2D_Color32(0x9A, 0x6C, 0xB9, 0xFF);
	u32 clrRec2 = C2D_Color32(0xFF, 0xFF, 0x2C, 0xFF);
	u32 clrRec3 = C2D_Color32(0xD8, 0xF6, 0x0F, 0xFF);
	u32 clrRec4 = C2D_Color32(0x40, 0xEA, 0x87, 0xFF);
	
	//"\x1b[r;cH"
	//The top screen has 30 rows and 50 columns
	//The bottom screen has 30 rows and 40 columns

	// Main loop
	while (aptMainLoop())
	{
		//Scan all the inputs. This should be done once for each frame
		hidScanInput();
		touchPosition touch;
		hidTouchRead(&touch);
		u32 kHeld = hidKeysHeld();
		u32 kDown = hidKeysDown();

		textUpdates(touch, kHeld, kDown);

		static SwkbdState swkbd;
		static char inputText[100];
		static SwkbdStatusData swkbdStatus;
		static SwkbdLearningData swkbdLearning;
		SwkbdButton button = SWKBD_BUTTON_NONE;
		bool didit = false;

		if (kDown & KEY_START) 
			break; // break in order to return to hbmenu

		if (kDown & KEY_A)
		{
			didit = true;
			swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 3, -1);
			swkbdSetInitialText(&swkbd, inputText);
			swkbdSetHintText(&swkbd, "Enter text to translate");
			swkbdSetButton(&swkbd, SWKBD_BUTTON_LEFT, "Cancel", false);
			swkbdSetButton(&swkbd, SWKBD_BUTTON_MIDDLE, "From Morse", true);
			swkbdSetButton(&swkbd, SWKBD_BUTTON_RIGHT, "To Morse", true);
			swkbdSetFeatures(&swkbd, SWKBD_PREDICTIVE_INPUT);
			SwkbdDictWord words[2];
			swkbdSetDictionary(&swkbd, words, sizeof(words)/sizeof(SwkbdDictWord));
			static bool reload = false;
			swkbdSetStatusData(&swkbd, &swkbdStatus, reload, true);
			swkbdSetLearningData(&swkbd, &swkbdLearning, reload, true);
			reload = true;
			button = swkbdInputText(&swkbd, inputText, sizeof inputText);
		}

		if (didit == true)
		{
			//InputText is the text to be displayed
			strcpy(inputtedText, inputText);

			if (button == SWKBD_BUTTON_LEFT)
			{
				didit = false;
			} 
			else if(button == SWKBD_BUTTON_MIDDLE)
			{
				translateFromMorse(inputText);
			}
			else if(button == SWKBD_BUTTON_RIGHT)
			{
				translateToMorse(inputText);
			}
		}

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, clrCyan);
		C2D_SceneBegin(top);

		drawUIText();	
		
		//input and output rectangles
		//C2D_DrawRectSolid(0, TOP_SCREEN_HEIGHT / 4, 0.0f, TOP_SCREEN_WIDTH, 25, clrBlack);
		//C2D_DrawRectSolid(0, TOP_SCREEN_HEIGHT - TOP_SCREEN_HEIGHT / 4, 0.0f, TOP_SCREEN_WIDTH, 25, clrBlack);

		//TOP LINES
		C2D_DrawLine(0, TOP_SCREEN_HEIGHT / 4, clrBlack, TOP_SCREEN_WIDTH, TOP_SCREEN_HEIGHT / 4, clrBlack, 2.0f, 1.0f); 
		C2D_DrawLine(0, TOP_SCREEN_HEIGHT / 4 + 20, clrBlack, TOP_SCREEN_WIDTH, TOP_SCREEN_HEIGHT / 4 + 20, clrBlack, 2.0f, 1.0f); 

		//BOTTOM LINES
		C2D_DrawLine(0, TOP_SCREEN_HEIGHT - TOP_SCREEN_HEIGHT / 4, clrBlack, TOP_SCREEN_WIDTH, TOP_SCREEN_HEIGHT - TOP_SCREEN_HEIGHT / 4, clrBlack, 2.0f, 1.0f); 
		C2D_DrawLine(0, TOP_SCREEN_HEIGHT - TOP_SCREEN_HEIGHT / 4 + 20, clrBlack, TOP_SCREEN_WIDTH, TOP_SCREEN_HEIGHT - TOP_SCREEN_HEIGHT / 4 + 20, clrBlack, 2.0f, 1.0f); 

		//float scaledPx = map(touch.px, 0, BOTTOM_SCREEN_WIDTH, 0, TOP_SCREEN_WIDTH);
		//C2D_DrawCircle(touch.px, touch.py, 0, 30, clrCircle3, clrWhite, clrCircle1, clrCircle2);

		C3D_FrameEnd(0);
	}
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	return 0;
}