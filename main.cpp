#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>

// has to be lower than image_... and pair
#define IMAGE_WIDTH 500
#define IMAGE_HEIGHT 500

#define RATIO 4

#define WIDTH IMAGE_WIDTH/RATIO
#define HEIGHT IMAGE_HEIGHT/RATIO

uint8_t pixels[IMAGE_WIDTH *IMAGE_HEIGHT*3];

void writePixels(uint8_t *image) {
    std::ofstream ofs("./output/block.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << WIDTH << " " << HEIGHT << "\n255\n";
    for (unsigned i = 0; i < WIDTH * HEIGHT; ++i) {
        ofs << image[i*3+0] <<
            image[i*3+1] <<
            image[i*3+2];
    }
    ofs.close();
}

std::vector<std::vector<int>> initEmptyBoard() {
    std::vector<std::vector<int>> board;

    for(int i = 0; i < WIDTH; i++) {
        std::vector<int> subboard;
        board.push_back(subboard);
        for(int j = 0; j < HEIGHT-1; j++) {
            board[i].push_back(0);
        }
    }

    return  board;
}

std::vector<std::vector<int>> initFullBoard() {
    std::vector<std::vector<int>> board;

    for(int i = 0; i < WIDTH; i++) {
        std::vector<int> subboard;
        board.push_back(subboard);
        for(int j = 0; j < HEIGHT-1; j++) {
            board[i].push_back(0);
        }
    }

    for(int i = 1; i < WIDTH-2; i++) {
        for(int j = 1; j < HEIGHT-2; j++) {
            if (rand() % 8 == 1) {
                board[i][j] = 1;
            }
        }
    }

    return  board;
}

int getState(
    int i1, int i2, int i3,
    int i4, int i5, int i6,
    int i7, int i8, int i9,
    int state
) {
    // These are the rules of conway's game of life, but you can customize of course.
    int sum = i1+i2+i3+i4+i6+i7+i8+i9;
    if(sum == 3) {
        return 1;
    } else if (sum == 2 && state == 1) {
        return 1;
    }
    return 0;
}

void drawOnBoard(std::vector<std::vector<int>> &board, float mX, float mY, int draw) {
    board[std::floor(mY/RATIO)][std::floor(mX/RATIO)] = draw;
};

void updateBoard(std::vector<std::vector<int>> &oldBoard, std::vector<std::vector<int>> &newBoard) {
    for(int i = 1; i < WIDTH-1; i++) {
        for(int j = 1; j < HEIGHT-1; j++) {
            int state = 0;
            
            int i1 = oldBoard[i-1][j-1];
            int i2 = oldBoard[i-0][j-1];
            int i3 = oldBoard[i+1][j-1];

            int i4 = oldBoard[i-1][j-0];
            int i5 = oldBoard[i-0][j-0];
            int i6 = oldBoard[i+1][j-0];

            int i7 = oldBoard[i-1][j+1];
            int i8 = oldBoard[i-0][j+1];
            int i9 = oldBoard[i+1][j+1];

            state = getState(i1, i2, i3, i4, i5, i6, i7, i8, i9, oldBoard[i][j]);
            newBoard[i][j] = state;
        }
    }
};

void getPixels(std::vector<std::vector<int>> board) {
    int basecolor[3] = {219, 105, 39};
    int endcolor[3] = {24, 50, 64};

    int change[3] = {(basecolor[0]-endcolor[0])/10, (basecolor[1]-endcolor[1])/10, (basecolor[2]-endcolor[2])/10};

    for(int i = 0; i < WIDTH; i++) {
        for(int j = 0; j < HEIGHT; j++) {
            for(int k = 0; k < RATIO; k++) {
                for(int z = 0; z < RATIO; z++) {
                    if (board[i][j] == 1) {
                        pixels[((i*RATIO+k)*IMAGE_WIDTH+(j*RATIO+z))*3 + 0] = basecolor[0];
                        pixels[((i*RATIO+k)*IMAGE_WIDTH+(j*RATIO+z))*3 + 1] = basecolor[1];
                        pixels[((i*RATIO+k)*IMAGE_WIDTH+(j*RATIO+z))*3 + 2] = basecolor[2];
                    } else if (
                        (pixels[((i*RATIO+k)*IMAGE_WIDTH+(j*RATIO+z))*3 + 0] > change[0]) && 
                        (pixels[((i*RATIO+k)*IMAGE_WIDTH+(j*RATIO+z))*3 + 1] > change[1]) && 
                        (pixels[((i*RATIO+k)*IMAGE_WIDTH+(j*RATIO+z))*3 + 2] > change[2])
                    ) {
                        pixels[((i*RATIO+k)*IMAGE_WIDTH+(j*RATIO+z))*3 + 0] -= change[0];
                        pixels[((i*RATIO+k)*IMAGE_WIDTH+(j*RATIO+z))*3 + 1] -= change[1];
                        pixels[((i*RATIO+k)*IMAGE_WIDTH+(j*RATIO+z))*3 + 2] -= change[1];
                    } else {
                        pixels[((i*RATIO+k)*IMAGE_WIDTH+(j*RATIO+z))*3 + 0] = 0;
                        pixels[((i*RATIO+k)*IMAGE_WIDTH+(j*RATIO+z))*3 + 1] = 0;
                        pixels[((i*RATIO+k)*IMAGE_WIDTH+(j*RATIO+z))*3 + 2] = 0;
                    }
                }
            }
        }
    }
};

int main(int argc, char **argv) {
    auto oldBoard = initFullBoard();
    auto newBoard = initEmptyBoard();
    bool quit = false;
    bool update = false;

    SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );}
	else {
		//Create window
		window = SDL_CreateWindow("Cellular Automata", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, IMAGE_WIDTH, IMAGE_HEIGHT, SDL_WINDOW_SHOWN);
		if( window == NULL ) { 
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError()); 
        } else {
			screenSurface = SDL_GetWindowSurface(window);
			SDL_Event e;
            float deltaX = 0;
            float deltaY = 0;

            while(!quit) {
				while(SDL_PollEvent( &e ) != 0) {
					if(e.type == SDL_QUIT) {
						quit = true;

                        /* writePixels(getPixels(board)); */
					}

                    if(e.type == SDL_KEYDOWN) {
                        update = !update;
                    }

                    deltaX = 0;
                    deltaY = 0;
                    
                    if(e.button.button == SDL_BUTTON(SDL_BUTTON_LEFT)) {
                        deltaX += e.motion.x;
                        deltaY += e.motion.y;
                
                        drawOnBoard(oldBoard, deltaX, deltaY, 1);
                    }

                    if(e.button.button == SDL_BUTTON(SDL_BUTTON_RIGHT)) {
                        deltaX += e.motion.x;
                        deltaY += e.motion.y;
                
                        drawOnBoard(oldBoard, deltaX, deltaY, 0);
                    }
				}

                if(update) {
                    updateBoard(oldBoard, newBoard);
                    getPixels(newBoard);
                    oldBoard = newBoard;
                    auto newBoard = initEmptyBoard(); 
                } else {
                    getPixels(oldBoard);
                }
                
                SDL_Surface *surf = SDL_CreateRGBSurfaceFrom((void*)pixels,
                    IMAGE_WIDTH,
                    IMAGE_HEIGHT,
                    3 * 8,
                    IMAGE_WIDTH * 3,
                    0x0000FF,
                    0x00FF00,
                    0xFF0000,
                    0
                );
                SDL_BlitSurface(surf, NULL, screenSurface, NULL);
                SDL_UpdateWindowSurface(window);
                SDL_Delay(50);
            }
			SDL_Delay(200);
		}
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
    /* writePixels(getPixels(board)); */
    return 0;
}