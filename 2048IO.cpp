#include <iostream>
#include <windows.h>
#include <memory>
#include <thread>
#include <chrono>
#include "2048.h"
#include "2048IO.h"

using namespace std;

void captureScreen(SOLVERSTATE* state){
	HDC hScreen = GetDC(GetDesktopWindow());
    int ScreenX = GetDeviceCaps(hScreen, HORZRES);
    int ScreenY = GetDeviceCaps(hScreen, VERTRES);
	void* ScreenData{0};

    HDC hdcMem = CreateCompatibleDC (hScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, ScreenX, ScreenY);
    HGDIOBJ hOld = SelectObject(hdcMem, hBitmap);
    BitBlt(hdcMem, 0, 0, ScreenX, ScreenY, hScreen, 0, 0, SRCCOPY);
    SelectObject(hdcMem, hOld); // windows quirk, have to return the original object

    BITMAPINFOHEADER bmi = {0};
    bmi.biSize = sizeof(BITMAPINFOHEADER);
    bmi.biPlanes = 1;
    bmi.biBitCount = 32;
    bmi.biWidth = ScreenX;
    bmi.biHeight = -ScreenY;
    bmi.biCompression = BI_RGB;
    bmi.biSizeImage = 0;// 3 * ScreenX * ScreenY;

    if(ScreenData)
        free(ScreenData);
    ScreenData = (BYTE*)malloc(4 * ScreenX * ScreenY);

    GetDIBits(hdcMem, hBitmap, 0, ScreenY, ScreenData, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);

    ReleaseDC(GetDesktopWindow(),hScreen);
    DeleteDC(hdcMem);
    DeleteObject(hBitmap);
	
	state->screenWidth = ScreenX;
	state->screenHeight = ScreenY;
	state->bitmapData = (DWORD*) ScreenData;
	cout << "Screen Capture Success" << endl;
	cout << "Screen Width : " << state->screenWidth << endl;
	cout << "Screen Height : " << state->screenHeight << endl;
	cout << "Scan Size : " << state->screenWidth << endl; 
	cout << "Data Size : " << bmi.biSizeImage << endl;
}

int calibrateScreen(SOLVERSTATE* state){
	captureScreen(state);

	//scanning for determining grid size
	int scanSize{(state->screenWidth)};
	int yscan{state->screenHeight / 2};
	
	state->boxXStart = -1;
	for(int x{0};x<state->screenWidth;++x){
		if(state->bitmapData[x+yscan*scanSize] == state->GRID_COLOR){
			state->boxXStart = x;
			break;
		}
	}
	if(state->boxXStart == -1)
		cout << "WARNING : Failed to find boxXStart" << endl;

	state->boxXEnd = -1;
	for(int x{state->screenWidth};0<x;--x){
		if(state->bitmapData[x+yscan*scanSize] == state->GRID_COLOR){
			state->boxXEnd = x;
			break;
		}
	}
	if(state->boxXEnd == -1)
		cout << "WARNING : Failed to find boxXEnd" << endl;
	
	if(state->boxXStart != -1 && state->boxXEnd != -1){
		int xscan{state->boxXStart + 10};
		
		state->boxYStart = -1;
		for(int y{0};y<state->screenHeight;++y){
			if(state->bitmapData[xscan+y*scanSize] == state->GRID_COLOR){
				state->boxYStart = y;
				break;
			}
		}
		if(state->boxYStart == -1)
			cout << "WARNING : Failed to find boxYStart" << endl;
		
		state->boxYEnd = -1;
		for(int y{state->screenHeight};0<y;--y){
			if(state->bitmapData[xscan+y*scanSize] == state->GRID_COLOR){
				state->boxYEnd = y;
				break;
			}
		}
		if(state->boxYEnd == -1)
			cout << "WARNING : Failed to find boxYEnd" << endl;
			
		if(state->boxYStart != -1 && state->boxYEnd != -1){
			cout << "Screen Calibration Success" << endl;
			cout << "Grid left-top : (" <<state->boxXStart << "," << state->boxYStart << ")" << endl;
			cout << "Grid right-bottom : (" <<state->boxXEnd << "," << state->boxYEnd << ")" << endl; 
			return 0;
		}else{
			return -1;
		}
	}else{
		return -1;
	}
}

bool getSmallTile(SOLVERSTATE* state){
	cout << "Start capturing grid state" << endl;
	HDC hScreen = GetDC(GetDesktopWindow());
	bool changed;
	int xoff{state->boxXStart + 25};
	int yoff{state->boxYStart + 25};
	DWORD pixel =0;
	changed = false;
	//do{
		xoff = state->boxXStart + 25;
		yoff = state->boxYStart + 25;
		for(int y{0};y<4;++y){
			for(int x{0};x<4;++x){
				pixel = GetPixel(hScreen,xoff,yoff);
				cout <<hex<< pixel<<dec<<" ";
				//cout << "(" << xoff << "," << yoff <<") : "<< hex << pixel << dec << endl;
				if(pixel == state->TILE_2_COLOR){
					//if(!(state->gridState[x][y]==2 || state->gridState[x][y]==0)){cout << "IO Failure"<<endl; return false;}
					//if(state->gridState[x][y]==0)changed = true;
					state->gridState[x][y] = 2;
				}
				if(pixel == state->TILE_4_COLOR){
					state->gridState[x][y] = 4;
				}
				if(pixel == 0x00B4C0CD){
					state->gridState[x][y] = 0;
				}
				if(pixel == 0x0079B1F2){
					state->gridState[x][y] = 8;
				}
				if(pixel == 0x005f7CF6){
					state->gridState[x][y] = 32;
				}
				if(pixel == 0x006395f5){
					state->gridState[x][y] = 16;
				}
				if(pixel == 0x003b5ef6){
					state->gridState[x][y] = 64;
				}
				if(pixel == 0x0072cfed){
					state->gridState[x][y] = 128;
				}
				if(pixel == 0x0061cced){
					state->gridState[x][y] = 256;
				}
				if(pixel == 0x0050c8ed){
					state->gridState[x][y] = 512;
				}
				if(pixel == 0x003fc5ED){
					state->gridState[x][y] = 1024;
				}
				xoff = xoff + 106 + 15;
			}
			xoff = state->boxXStart + 25;
			yoff = yoff + 106 +15;
			cout << endl;
		}
		//if(!changed) Sleep(25);
	//}while(!changed);
	cout <<endl;
	
	ReleaseDC(GetDesktopWindow(),hScreen);
	return true;
}

bool gameOver(SOLVERSTATE* state){
	HDC hScreen = GetDC(GetDesktopWindow());
	DWORD pixel;
	pixel = GetPixel(hScreen,state->boxXStart+10,state->boxYStart+10);
	ReleaseDC(GetDesktopWindow(),hScreen);
	cout <<"pixel state : "<<hex<< pixel<<dec<<endl;
	return (pixel == (0x00cad5e1));
}

void actuateMoves(moves_t move){
	HWND hBrowser;
	hBrowser = FindWindow(NULL,"2048 - Mozilla Firefox");
	if(hBrowser==NULL){
		cout << "WARNING : Failed to grab the browser windows" << endl;
	}else{
		DWORD move_key;
		cout << "Actuating Moves (";
		if(move == UP){
			move_key = VK_UP;
			cout << "UP";
		}
		if(move == DOWN){
			move_key = VK_DOWN;
			cout << "DOWN";
		}
		if(move == LEFT){
			move_key = VK_LEFT;
			cout << "LEFT";
		}
		if(move == RIGHT){
			move_key = VK_RIGHT;
			cout << "RIGHT";
		}
		cout << ") : ";
		if(!SendMessage(hBrowser,WM_KEYDOWN,move_key,0)){
			Sleep(10);
			cout << "success" << endl;
			SendMessage(hBrowser,WM_KEYUP,move_key,0);
		}else{
			cout << "failed" <<endl;
		}
	}
	Sleep(50);
}