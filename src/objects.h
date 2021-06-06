#include <vector>

void createGlider(std::vector<std::vector<int>> &board, int posX, int posY, int rotation) {
    int gliderArr[3][3] = {
        0,1,0,
        0,0,1,
        1,1,1
    };

    int n = 3;

    for(int k = 0; k < rotation; k++) {
        for (int i = 0; i < n / 2; i++)  { 
            for (int j = i; j < n-i-1; j++)  {
                int temp = gliderArr[i][j];
                gliderArr[i][j] = gliderArr[n-1-j][i];
                gliderArr[n-1-j][i] = gliderArr[n-1-i][n-1-j];
                gliderArr[n-1-i][n-1-j] = gliderArr[j][n-1-i];
                gliderArr[j][n-1-i] = temp;
            }
        }
    }

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            board[posX + i][posY + j] = gliderArr[i][j];
        }
    }
}

void createLWSS(std::vector<std::vector<int>> &board, int posX, int posY, int rotation) {
    int LWSSArr[7][7] = {
        0,1,0,0,1,0,0,
        0,0,0,0,0,1,0,
        0,1,0,0,0,1,0,
        0,0,1,1,1,1,0,
        0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,
    };

    int n = 7;

    for(int k = 0; k < rotation; k++) {
        for (int i = 0; i < n / 2; i++)  { 
            for (int j = i; j < n-i-1; j++)  {
                int temp = LWSSArr[i][j];
                LWSSArr[i][j] = LWSSArr[n-1-j][i];
                LWSSArr[n-1-j][i] = LWSSArr[n-1-i][n-1-j];
                LWSSArr[n-1-i][n-1-j] = LWSSArr[j][n-1-i];
                LWSSArr[j][n-1-i] = temp;
            }
        }
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            board[posX + i][posY + j] = LWSSArr[i][j];
        }
    }
}

void createLine(std::vector<std::vector<int>> &board, int posX, int posY, int length, int rotation) {
    if(rotation == 0 || rotation == 2) {
        for(int i = 0; i < length; i++) {
            board[posX + i][posY] = 1;
        }
    } else {
        for(int i = 0; i < length; i++) {
            board[posX][posY + i] = 1;
        }
    }
}