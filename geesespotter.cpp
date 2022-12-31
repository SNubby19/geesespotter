#include "geesespotter_lib.h"
#include "geesespotter.h"

// Declarations
char *createBoard(std::size_t xdim, std::size_t ydim);
void cleanBoard(char *board);
void printBoard(char *board, std::size_t xdim, std::size_t ydim);
void computeNeighbors(char *board, std::size_t xdim, std::size_t ydim);
void hideBoard(char *board, std::size_t xdim, std::size_t ydim);
int reveal(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc);
int mark(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc);
bool isGameWon(char *board, std::size_t xdim, std::size_t ydim);

// Definitions
char *createBoard(std::size_t xdim, std::size_t ydim)
{
    std::size_t *numElems{new std::size_t{xdim * ydim}};
    char *board{new char[*numElems]{0}};

    delete numElems;
    numElems = nullptr;
    
    return board;
}

void cleanBoard(char *board)
{
    delete[] board;
    board = nullptr;
}

void printBoard(char *board, std::size_t xdim, std::size_t ydim)
{

    for (std::size_t i{0}; i < ydim; ++i)
    {
        
        for (std::size_t j{0}; j < xdim; ++j)
        {
            char &node{board[((i) * xdim) + j]};
            if (bool(node & markedBit()))
            {
                std::cout << 'M';
            }
            else if (bool(node & hiddenBit()))
            {
                std::cout << '*';
            }
            else
            {
                std::cout << (node & valueMask());
            }
        }
        std::cout << '\n';
    }
}

void hideBoard(char *board, std::size_t xdim, std::size_t ydim)
{
    for (std::size_t i{0}; i < xdim; ++i)
    {
        for (std::size_t j{0}; j < ydim; ++j)
        {
            char &node{board[(i * ydim) + j]};
            
            node = (node | hiddenBit());   
        }
    }
}

int mark(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc)
{
    char &node{board[(xloc * xdim) +  yloc ]};

    if (node & hiddenBit())
    {
        node = (node ^ markedBit());
        return 0;
    }
    else
    {
        return 2;
    }
}

void computeNeighbors(char *board, std::size_t xdim, std::size_t ydim) {
    for (std::size_t i{0}; i < ydim; ++i)
    {
        for (std::size_t j{0}; j < xdim; ++j)
        {
            char &node{board[((i) * xdim) + j]};

            if ((node & valueMask()) != 9) {
                for (int k{-1}; k <= 1; ++k) {
                    int x_coord{j + k};
                    int y_coord1{i-1};
                    int y_coord2{i + 1};
                    int y_coord3{i};

                    char &node_u{board[y_coord1*xdim + x_coord]};
                    char &node_m{board[y_coord3*xdim + x_coord]};
                    char &node_l{board[y_coord2*xdim + x_coord]};

                    if (x_coord >= 0 && y_coord1 >= 0 && x_coord < xdim && y_coord1 < ydim) {
                        if ((node_u & valueMask()) == 9) {
                            ++node;
                        }
                    }
                    if (x_coord >= 0 && y_coord2 >= 0 && x_coord < xdim && y_coord2 < ydim) {
                        if ((node_l & valueMask()) == 9) {
                            ++node;
                        }
                    }
                    if(x_coord != j && x_coord >= 0 && y_coord3 >= 0 && x_coord < xdim && y_coord3 < ydim) {
                        if ((node_m & valueMask()) == 9) {
                            ++node;
                        }
                    }

                }
            }
             
        }
    }

    return;
}

int reveal(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc) {
    char &node{board[(yloc * xdim) + xloc]};

    if (node & markedBit()) {
        return 1;
    } else if (!(node & hiddenBit())) {
        return 2;
    } else {
        if ((node & valueMask()) == 9) {
            node = node ^ hiddenBit();
            return 9;
        } else {
            if ((node & valueMask()) != 0) {
                node = node ^ hiddenBit();
            } else {

                for (int k{-1}; k <= 1; ++k) {
                    int x_coord{xloc + k};
                    int y_coord1{yloc-1};
                    int y_coord2{yloc + 1};
                    int y_coord3{yloc};

                    char &node_u{board[y_coord1*xdim + x_coord]};
                    char &node_m{board[y_coord3*xdim + x_coord]};
                    char &node_l{board[y_coord2*xdim + x_coord]};

                    if (x_coord >= 0 && y_coord1 >= 0 && x_coord < xdim && y_coord1 < ydim) {
                        if (!(node_u & markedBit()) && (node_u & hiddenBit())) {
                            std::cout << "Reveals top row";
                            node_u = node_u ^ hiddenBit();
                        }
                    }
                    if (x_coord >= 0 && y_coord2 >= 0 && x_coord < xdim && y_coord2 < ydim) {
                        if (!(node_m & markedBit()) && (node_m & hiddenBit())) {
                            std::cout << "Reveals middle row";
                            node_m = node_m ^ hiddenBit();
                        }
                    }
                    if(x_coord >= 0 && y_coord3 >= 0 && x_coord < xdim && y_coord3 < ydim) {
                        if (!(node_l & markedBit()) && (node_l & hiddenBit())) {
                            std::cout << "Reveals bottom row";
                            node_l = node_l ^ hiddenBit();
                        }
                    }
                }
            }
        }
    }
    

    return 0;
}

bool isGameWon(char *board, std::size_t xdim, std::size_t ydim)
{
    for (std::size_t i{0}; i < xdim * ydim; ++i) {
        char node = board[i];

        if (bool(node & hiddenBit()) && ((node & valueMask()) != 9)) {
            return false;
        }
    }
    
    return true;
}
