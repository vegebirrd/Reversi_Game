// boardwidget.cpp
#include "boardwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QString>
#include <QHBoxLayout>
#include <QThread>
#include "globals.h"

int Round=1;
int whiteStone=2;
int blackStone=2;
int gridCondition[8][8];
int gameMode;
void BoardWidget::reset(){
    Round=1;
    whiteStone=2;
    blackStone=2;
    for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
            if(i==3&&j==3||i==4&&j==4) gridCondition[i][j]=-1; //initial white condition
            else if(i==3&&j==4||i==4&&j==3) gridCondition[i][j]=1; //initial black condition
            else gridCondition[i][j]=0; //initial empty cells
        }
    }
}
bool BoardWidget::checkBlackLegality(int x,int y){
    if (gridCondition[x][y]!=0){
        return false;
    }//if the cell already has got a stone inside,then you cannot place your stone in it.
    bool flipWhite=false;
    for (int i=x+1;i<8;i++){
        if (gridCondition[i][y]==0) break;
        else if (gridCondition[i][y]==-1) flipWhite=true;
        else if (gridCondition[i][y]==1&&flipWhite){
            return true;
        }
        else if (gridCondition[i][y]==1&&!flipWhite) break;
    }//check the column (up)
    flipWhite=false;
    for (int i=x-1;i>=0;i--){
        if (gridCondition[i][y]==0) break;
        else if (gridCondition[i][y]==-1) flipWhite=true;
        else if (gridCondition[i][y]==1&&flipWhite){
            return true;
        }
        else if (gridCondition[i][y]==1&&!flipWhite) break;
    }//check the column (down)
    flipWhite=false;
    for (int i=y+1;i<8;i++){
        if (gridCondition[x][i]==0) break;
        else if (gridCondition[x][i]==-1) flipWhite=true;
        else if (gridCondition[x][i]==1&&flipWhite){
            return true;
        }
        else if (gridCondition[x][i]==1&&!flipWhite) break;
    }//check the row (right)
    flipWhite=false;
    for (int i=y-1;i>=0;i--){
        if (gridCondition[x][i]==0) break;
        else if (gridCondition[x][i]==-1) flipWhite=true;
        else if (gridCondition[x][i]==1&&flipWhite){
            return true;
        }
        else if (gridCondition[x][i]==1&&!flipWhite) break;
    }//check the row (left)
    flipWhite=false;
    for (int i=1;i<8;i++){
        if (x+i>=8||y+i>=8) break;
        if (gridCondition[x+i][y+i]==0) break;
        else if (gridCondition[x+i][y+i]==-1) flipWhite=true;
        else if (gridCondition[x+i][y+i]==1&&flipWhite){
            return true;
        }
        else if (gridCondition[x+i][y+i]==1&&!flipWhite) break;
    }//check the diagonals
    flipWhite=false;
    for (int i=1;i<8;i++){
        if (x-i<0||y-i<0) break;
        if (gridCondition[x-i][y-i]==0) break;
        else if (gridCondition[x-i][y-i]==-1) flipWhite=true;
        else if (gridCondition[x-i][y-i]==1&&flipWhite){
            return true;
        }
        else if (gridCondition[x-i][y-i]==1&&!flipWhite) break;
    }//check the diagonals
    flipWhite=false;
    for (int i=1;i<8;i++){
        if (x-i<0||y+i>=8) break;
        if (gridCondition[x-i][y+i]==0) break;
        else if (gridCondition[x-i][y+i]==-1) flipWhite=true;
        else if (gridCondition[x-i][y+i]==1&&flipWhite){
            return true;
        }
        else if (gridCondition[x-i][y+i]==1&&!flipWhite) break;
    }//check the diagonals
    flipWhite=false;
    for (int i=1;i<8;i++){
        if (x+i>=8||y-i<0) break;
        if (gridCondition[x+i][y-i]==0) break;
        else if (gridCondition[x+i][y-i]==-1) flipWhite=true;
        else if (gridCondition[x+i][y-i]==1&&flipWhite){
            return true;
        }
        else if (gridCondition[x+i][y-i]==1&&!flipWhite) break;
    }//check the diagonals
    return false;
}
bool BoardWidget::checkWhiteLegality(int x,int y){
    if (gridCondition[x][y]!=0){
        return false;
    }
    bool flipBlack=false;
    for (int i=x+1;i<8;i++){
        if (gridCondition[i][y]==0) break;
        else if (gridCondition[i][y]==1) flipBlack=true;
        else if (gridCondition[i][y]==-1&&flipBlack){
            return true;
        }
        else if (gridCondition[i][y]==-1&&!flipBlack) break;
    }//check the column (up)
    flipBlack=false;
    for (int i=x-1;i>=0;i--){
        if (gridCondition[i][y]==0) break;
        else if (gridCondition[i][y]==1) flipBlack=true;
        else if (gridCondition[i][y]==-1&&flipBlack){
            return true;
        }
        else if (gridCondition[i][y]==-1&&!flipBlack) break;
    }//check the column (down)
    flipBlack=false;
    for (int i=y+1;i<8;i++){
        if (gridCondition[x][i]==0) break;
        else if (gridCondition[x][i]==1) flipBlack=true;
        else if (gridCondition[x][i]==-1&&flipBlack){
            return true;
        }
        else if (gridCondition[x][i]==-1&&!flipBlack) break;
    }//check the row (right)
    flipBlack=false;
    for (int i=y-1;i>=0;i--){
        if (gridCondition[x][i]==0) break;
        else if (gridCondition[x][i]==1) flipBlack=true;
        else if (gridCondition[x][i]==-1&&flipBlack){
            return true;
        }
        else if (gridCondition[x][i]==-1&&!flipBlack) break;
    }//check the row (left)
    flipBlack=false;
    for (int i=1;i<8;i++){
        if (x+i>=8||y+i>=8) break;
        if (gridCondition[x+i][y+i]==0) break;
        else if (gridCondition[x+i][y+i]==1) flipBlack=true;
        else if (gridCondition[x+i][y+i]==-1&&flipBlack){
            return true;
        }
        else if (gridCondition[x+i][y+i]==-1&&!flipBlack) break;
    }//check the diagonals
    flipBlack=false;
    for (int i=1;i<8;i++){
        if (x-i<0||y-i<0) break;
        if (gridCondition[x-i][y-i]==0) break;
        else if (gridCondition[x-i][y-i]==1) flipBlack=true;
        else if (gridCondition[x-i][y-i]==-1&&flipBlack){
            return true;
        }
        else if (gridCondition[x-i][y-i]==-1&&!flipBlack) break;
    }//check the diagonals
    flipBlack=false;
    for (int i=1;i<8;i++){
        if (x-i<0||y+i>=8) break;
        if (gridCondition[x-i][y+i]==0) break;
        else if (gridCondition[x-i][y+i]==1) flipBlack=true;
        else if (gridCondition[x-i][y+i]==-1&&flipBlack){
            return true;
        }
        else if (gridCondition[x-i][y+i]==-1&&!flipBlack) break;
    }//check the diagonals
    flipBlack=false;
    for (int i=1;i<8;i++){
        if (x+i>=8||y-i<0) break;
        if (gridCondition[x+i][y-i]==0) break;
        else if (gridCondition[x+i][y-i]==1) flipBlack=true;
        else if (gridCondition[x+i][y-i]==-1&&flipBlack){
            return true;
        }
        else if (gridCondition[x+i][y-i]==-1&&!flipBlack) break;
    }//check the diagonals
    return false;
}
int BoardWidget::countWhite(){
    int white=0;
    for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
            if (gridCondition[i][j]==-1) white++;
        }
    }
    return white;
}
int BoardWidget::countBlack(){
    int black=0;
    for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
            if (gridCondition[i][j]==1) black++;
        }
    }
    return black;
}
bool BoardWidget::blackNeedSkip(){
    for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
            if (checkBlackLegality(i,j)) return false;
        }
    }
    return true;
}
bool BoardWidget::whiteNeedSkip(){
    for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
            if (checkWhiteLegality(i,j)) return false;
        }
    }
    return true;
}//if you cannot take any legal move,then skip this round.
void BoardWidget::flipWhiteMove(int x,int y){
    bool flipWhite=false,canFlip=false;
    int flipMark=0;
    for (int i=x+1;i<8;i++){
        if (gridCondition[i][y]==0) break;
        else if (gridCondition[i][y]==-1) flipWhite=true;
        else if (gridCondition[i][y]==1&&flipWhite){
            flipMark=i;
            canFlip=true;
            break;
        }
        else if (gridCondition[i][y]==1&&!flipWhite) break;
    }//check the column (up),mark the cells that can be flipped.
    if (canFlip){
        for (int i=x+1;i<flipMark;i++){
            gridCondition[i][y]=1;
        }
    }
    flipWhite=false,canFlip=false;
    for (int i=x-1;i>=0;i--){
        if (gridCondition[i][y]==0) break;
        else if (gridCondition[i][y]==-1) flipWhite=true;
        else if (gridCondition[i][y]==1&&flipWhite){
            flipMark=i;
            canFlip=true;
            break;
        }
        else if (gridCondition[i][y]==1&&!flipWhite) break;
    }//check the column (down)
    if (canFlip){
        for (int i=x-1;i>flipMark;i--){
            gridCondition[i][y]=1;
        }
    }
    flipWhite=false,canFlip=false;
    for (int i=y+1;i<8;i++){
        if (gridCondition[x][i]==0) break;
        else if (gridCondition[x][i]==-1) flipWhite=true;
        else if (gridCondition[x][i]==1&&flipWhite){
            flipMark=i;
            canFlip=true;
            break;
        }
        else if (gridCondition[x][i]==1&&!flipWhite) break;
    }//check the row (right)
    if (canFlip){
        for (int i=y+1;i<flipMark;i++){
            gridCondition[x][i]=1;
        }
    }
    flipWhite=false,canFlip=false;
    for (int i=y-1;i>=0;i--){
        if (gridCondition[x][i]==0) break;
        else if (gridCondition[x][i]==-1) flipWhite=true;
        else if (gridCondition[x][i]==1&&flipWhite){
            flipMark=i;
            canFlip=true;
            break;
        }
        else if (gridCondition[x][i]==1&&!flipWhite) break;
    }//check the row (left)
    if (canFlip){
        for (int i=y-1;i>flipMark;i--){
            gridCondition[x][i]=1;
        }
    }
    flipWhite=false,canFlip=false;
    for (int i=1;i<8;i++){
        if (x+i>=8||y+i>=8) break;
        if (gridCondition[x+i][y+i]==0) break;
        else if (gridCondition[x+i][y+i]==-1) flipWhite=true;
        else if (gridCondition[x+i][y+i]==1&&flipWhite){
            flipMark=i;
            canFlip=true;
            break;
        }
        else if (gridCondition[x+i][y+i]==1&&!flipWhite) break;
    }//check the diagonals
    if (canFlip){
        for (int i=1;i<flipMark;i++){
            gridCondition[x+i][y+i]=1;
        }
    }
    flipWhite=false,canFlip=false;
    for (int i=1;i<8;i++){
        if (x-i<0||y-i<0) break;
        if (gridCondition[x-i][y-i]==0) break;
        else if (gridCondition[x-i][y-i]==-1) flipWhite=true;
        else if (gridCondition[x-i][y-i]==1&&flipWhite){
            flipMark=i;
            canFlip=true;
            break;
        }
        else if (gridCondition[x-i][y-i]==1&&!flipWhite) break;
    }//check the diagonals
    if (canFlip){
        for (int i=1;i<flipMark;i++){
            gridCondition[x-i][y-i]=1;
        }
    }
    flipWhite=false,canFlip=false;
    for (int i=1;i<8;i++){
        if (x-i<0||y+i>=8) break;
        if (gridCondition[x-i][y+i]==0) break;
        else if (gridCondition[x-i][y+i]==-1) flipWhite=true;
        else if (gridCondition[x-i][y+i]==1&&flipWhite){
            flipMark=i;
            canFlip=true;
            break;
        }
        else if (gridCondition[x-i][y+i]==1&&!flipWhite) break;
    }//check the diagonals
    if (canFlip){
        for (int i=1;i<flipMark;i++){
            gridCondition[x-i][y+i]=1;
        }
    }
    flipWhite=false,canFlip=false;
    for (int i=1;i<8;i++){
        if (x+i>=8||y-i<0) break;
        if (gridCondition[x+i][y-i]==0) break;
        else if (gridCondition[x+i][y-i]==-1) flipWhite=true;
        else if (gridCondition[x+i][y-i]==1&&flipWhite){
            flipMark=i;
            canFlip=true;
            break;
        }
        else if (gridCondition[x+i][y-i]==1&&!flipWhite) break;
    }//check the diagonals
    if (canFlip){
        for (int i=1;i<flipMark;i++){
            gridCondition[x+i][y-i]=1;
        }
    }
}
void BoardWidget::flipBlackMove(int x,int y){
    bool flipBlack=false,canFlip=false;
    int flipMark=0;
    for (int i=x+1;i<8;i++){
        if (gridCondition[i][y]==0) break;
        else if (gridCondition[i][y]==1) flipBlack=true;
        else if (gridCondition[i][y]==-1&&flipBlack){
            flipMark=i;
            canFlip=true;
            break;
        }
        else if (gridCondition[i][y]==-1&&!flipBlack) break;
    }//check the column (up),mark the cells that can be flipped.
    if (canFlip){
        for (int i=x+1;i<flipMark;i++){
            gridCondition[i][y]=-1;
        }
    }
    flipBlack=false,canFlip=false;
    for (int i=x-1;i>=0;i--){
        if (gridCondition[i][y]==0) break;
        else if (gridCondition[i][y]==1) flipBlack=true;
        else if (gridCondition[i][y]==-1&&flipBlack){
            flipMark=i;
            canFlip=true;
            break;
        }
        else if (gridCondition[i][y]==-1&&!flipBlack) break;
    }//check the column (down)
    if (canFlip){
        for (int i=x-1;i>flipMark;i--){
            gridCondition[i][y]=-1;
        }
    }
    flipBlack=false,canFlip=false;
    for (int i=y+1;i<8;i++){
        if (gridCondition[x][i]==0) break;
        else if (gridCondition[x][i]==1) flipBlack=true;
        else if (gridCondition[x][i]==-1&&flipBlack){
            flipMark=i;
            canFlip=true;
            break;
        }
        else if (gridCondition[x][i]==-1&&!flipBlack) break;
    }//check the row (right)
    if (canFlip){
        for (int i=y+1;i<flipMark;i++){
            gridCondition[x][i]=-1;
        }
    }
    flipBlack=false,canFlip=false;
    for (int i=y-1;i>=0;i--){
        if (gridCondition[x][i]==0) break;
        else if (gridCondition[x][i]==1) flipBlack=true;
        else if (gridCondition[x][i]==-1&&flipBlack){
            flipMark=i;
            canFlip=true;
            break;
        }
        else if (gridCondition[x][i]==-1&&!flipBlack) break;
    }//check the row (left)
    if (canFlip){
        for (int i=y-1;i>flipMark;i--){
            gridCondition[x][i]=-1;
        }
    }
    flipBlack=false,canFlip=false;
    for (int i=1;i<8;i++){
        if (x+i>=8||y+i>=8) break;
        if (gridCondition[x+i][y+i]==0) break;
        else if (gridCondition[x+i][y+i]==1) flipBlack=true;
        else if (gridCondition[x+i][y+i]==-1&&flipBlack){
            flipMark=i;
            canFlip=true;
            break;
        }
        else if (gridCondition[x+i][y+i]==-1&&!flipBlack) break;
    }//check the diagonals
    if (canFlip){
        for (int i=1;i<flipMark;i++){
            gridCondition[x+i][y+i]=-1;
        }
    }
    flipBlack=false,canFlip=false;
    for (int i=1;i<8;i++){
        if (x-i<0||y-i<0) break;
        if (gridCondition[x-i][y-i]==0) break;
        else if (gridCondition[x-i][y-i]==1) flipBlack=true;
        else if (gridCondition[x-i][y-i]==-1&&flipBlack){
            flipMark=i;
            canFlip=true;
            break;
        }
        else if (gridCondition[x-i][y-i]==-1&&!flipBlack) break;
    }//check the diagonals
    if (canFlip){
        for (int i=1;i<flipMark;i++){
            gridCondition[x-i][y-i]=-1;
        }
    }
    flipBlack=false,canFlip=false;
    for (int i=1;i<8;i++){
        if (x-i<0||y+i>=8) break;
        if (gridCondition[x-i][y+i]==0) break;
        else if (gridCondition[x-i][y+i]==1) flipBlack=true;
        else if (gridCondition[x-i][y+i]==-1&&flipBlack){
            flipMark=i;
            canFlip=true;
            break;
        }
        else if (gridCondition[x-i][y+i]==-1&&!flipBlack) break;
    }//check the diagonals
    if (canFlip){
        for (int i=1;i<flipMark;i++){
            gridCondition[x-i][y+i]=-1;
        }
    }
    flipBlack=false,canFlip=false;
    for (int i=1;i<8;i++){
        if (x+i>=8||y-i<0) break;
        if (gridCondition[x+i][y-i]==0) break;
        else if (gridCondition[x+i][y-i]==1) flipBlack=true;
        else if (gridCondition[x+i][y-i]==-1&&flipBlack){
            flipMark=i;
            canFlip=true;
            break;
        }
        else if (gridCondition[x+i][y-i]==-1&&!flipBlack) break;
    }//check the diagonals
    if (canFlip){
        for (int i=1;i<flipMark;i++){
            gridCondition[x+i][y-i]=-1;
        }
    }
}
bool BoardWidget::endgame(){
    if (blackStone+whiteStone==64) return true;
    else if (blackNeedSkip()&whiteNeedSkip()) return true;
    return false;
}
void BoardWidget::normalBotWhiteMove(){
    bool findLegal=false;
    int markx,marky,maxFlip=0;
    for (int x=0;x<8;x++){
        for (int y=0;y<8;y++){
            if (checkWhiteLegality(x,y)){
                findLegal=true;
                int flipnum=0;
                bool flipBlack=false,canFlip=false;
                int flipMark=0;
                for (int i=x+1;i<8;i++){
                    if (gridCondition[i][y]==0) break;
                    else if (gridCondition[i][y]==1) flipBlack=true;
                    else if (gridCondition[i][y]==-1&&flipBlack){
                        flipMark=i;
                        canFlip=true;
                        break;
                    }
                    else if (gridCondition[i][y]==-1&&!flipBlack) break;
                }//check the column (up),mark the cells that can be flipped.
                if (canFlip){
                    for (int i=x+1;i<flipMark;i++){
                        flipnum++;
                    }
                }
                flipBlack=false,canFlip=false;
                for (int i=x-1;i>=0;i--){
                    if (gridCondition[i][y]==0) break;
                    else if (gridCondition[i][y]==1) flipBlack=true;
                    else if (gridCondition[i][y]==-1&&flipBlack){
                        flipMark=i;
                        canFlip=true;
                        break;
                    }
                    else if (gridCondition[i][y]==-1&&!flipBlack) break;
                }//check the column (down)
                if (canFlip){
                    for (int i=x-1;i>flipMark;i--){
                        flipnum++;
                    }
                }
                flipBlack=false,canFlip=false;
                for (int i=y+1;i<8;i++){
                    if (gridCondition[x][i]==0) break;
                    else if (gridCondition[x][i]==1) flipBlack=true;
                    else if (gridCondition[x][i]==-1&&flipBlack){
                        flipMark=i;
                        canFlip=true;
                        break;
                    }
                    else if (gridCondition[x][i]==-1&&!flipBlack) break;
                }//check the row (right)
                if (canFlip){
                    for (int i=y+1;i<flipMark;i++){
                        flipnum++;
                    }
                }
                flipBlack=false,canFlip=false;
                for (int i=y-1;i>=0;i--){
                    if (gridCondition[x][i]==0) break;
                    else if (gridCondition[x][i]==1) flipBlack=true;
                    else if (gridCondition[x][i]==-1&&flipBlack){
                        flipMark=i;
                        canFlip=true;
                        break;
                    }
                    else if (gridCondition[x][i]==-1&&!flipBlack) break;
                }//check the row (left)
                if (canFlip){
                    for (int i=y-1;i>flipMark;i--){
                        flipnum++;
                    }
                }
                flipBlack=false,canFlip=false;
                for (int i=1;i<8;i++){
                    if (x+i>=8||y+i>=8) break;
                    if (gridCondition[x+i][y+i]==0) break;
                    else if (gridCondition[x+i][y+i]==1) flipBlack=true;
                    else if (gridCondition[x+i][y+i]==-1&&flipBlack){
                        flipMark=i;
                        canFlip=true;
                        break;
                    }
                    else if (gridCondition[x+i][y+i]==-1&&!flipBlack) break;
                }//check the diagonals
                if (canFlip){
                    for (int i=1;i<flipMark;i++){
                        flipnum++;
                    }
                }
                flipBlack=false,canFlip=false;
                for (int i=1;i<8;i++){
                    if (x-i<0||y-i<0) break;
                    if (gridCondition[x-i][y-i]==0) break;
                    else if (gridCondition[x-i][y-i]==1) flipBlack=true;
                    else if (gridCondition[x-i][y-i]==-1&&flipBlack){
                        flipMark=i;
                        canFlip=true;
                        break;
                    }
                    else if (gridCondition[x-i][y-i]==-1&&!flipBlack) break;
                }//check the diagonals
                if (canFlip){
                    for (int i=1;i<flipMark;i++){
                        flipnum++;
                    }
                }
                flipBlack=false,canFlip=false;
                for (int i=1;i<8;i++){
                    if (x-i<0||y+i>=8) break;
                    if (gridCondition[x-i][y+i]==0) break;
                    else if (gridCondition[x-i][y+i]==1) flipBlack=true;
                    else if (gridCondition[x-i][y+i]==-1&&flipBlack){
                        flipMark=i;
                        canFlip=true;
                        break;
                    }
                    else if (gridCondition[x-i][y+i]==-1&&!flipBlack) break;
                }//check the diagonals
                if (canFlip){
                    for (int i=1;i<flipMark;i++){
                        flipnum++;
                    }
                }
                flipBlack=false,canFlip=false;
                for (int i=1;i<8;i++){
                    if (x+i>=8||y-i<0) break;
                    if (gridCondition[x+i][y-i]==0) break;
                    else if (gridCondition[x+i][y-i]==1) flipBlack=true;
                    else if (gridCondition[x+i][y-i]==-1&&flipBlack){
                        flipMark=i;
                        canFlip=true;
                        break;
                    }
                    else if (gridCondition[x+i][y-i]==-1&&!flipBlack) break;
                }//check the diagonals
                if (canFlip){
                    for (int i=1;i<flipMark;i++){
                        flipnum++;
                    }
                }
                if (flipnum>maxFlip){
                    maxFlip=flipnum;
                    markx=x;
                    marky=y;
                }
            }
        }
    }
    if (findLegal) {
        gridCondition[markx][marky]=-1;
        flipBlackMove(markx,marky);
        blackStone=countBlack();
        whiteStone=countWhite();
        Round++;
    }
}
void BoardWidget::normalBotBlackMove(){
    bool findLegal=false;
    int markx,marky,maxFlip=0;
    for (int x=0;x<8;x++){
        for (int y=0;y<8;y++){
            if (checkBlackLegality(x,y)){
                findLegal=true;
                int flipnum=0;
                bool flipWhite=false,canFlip=false;
                int flipMark=0;
                for (int i=x+1;i<8;i++){
                    if (gridCondition[i][y]==0) break;
                    else if (gridCondition[i][y]==-1) flipWhite=true;
                    else if (gridCondition[i][y]==1&&flipWhite){
                        flipMark=i;
                        canFlip=true;
                        break;
                    }
                    else if (gridCondition[i][y]==1&&!flipWhite) break;
                }//check the column (up),mark the cells that can be flipped.
                if (canFlip){
                    for (int i=x+1;i<flipMark;i++){
                        flipnum++;
                    }
                }
                flipWhite=false,canFlip=false;
                for (int i=x-1;i>=0;i--){
                    if (gridCondition[i][y]==0) break;
                    else if (gridCondition[i][y]==-1) flipWhite=true;
                    else if (gridCondition[i][y]==1&&flipWhite){
                        flipMark=i;
                        canFlip=true;
                        break;
                    }
                    else if (gridCondition[i][y]==1&&!flipWhite) break;
                }//check the column (down)
                if (canFlip){
                    for (int i=x-1;i>flipMark;i--){
                        flipnum++;
                    }
                }
                flipWhite=false,canFlip=false;
                for (int i=y+1;i<8;i++){
                    if (gridCondition[x][i]==0) break;
                    else if (gridCondition[x][i]==-1) flipWhite=true;
                    else if (gridCondition[x][i]==1&&flipWhite){
                        flipMark=i;
                        canFlip=true;
                        break;
                    }
                    else if (gridCondition[x][i]==1&&!flipWhite) break;
                }//check the row (right)
                if (canFlip){
                    for (int i=y+1;i<flipMark;i++){
                        flipnum++;
                    }
                }
                flipWhite=false,canFlip=false;
                for (int i=y-1;i>=0;i--){
                    if (gridCondition[x][i]==0) break;
                    else if (gridCondition[x][i]==-1) flipWhite=true;
                    else if (gridCondition[x][i]==1&&flipWhite){
                        flipMark=i;
                        canFlip=true;
                        break;
                    }
                    else if (gridCondition[x][i]==1&&!flipWhite) break;
                }//check the row (left)
                if (canFlip){
                    for (int i=y-1;i>flipMark;i--){
                        flipnum++;
                    }
                }
                flipWhite=false,canFlip=false;
                for (int i=1;i<8;i++){
                    if (x+i>=8||y+i>=8) break;
                    if (gridCondition[x+i][y+i]==0) break;
                    else if (gridCondition[x+i][y+i]==-1) flipWhite=true;
                    else if (gridCondition[x+i][y+i]==1&&flipWhite){
                        flipMark=i;
                        canFlip=true;
                        break;
                    }
                    else if (gridCondition[x+i][y+i]==1&&!flipWhite) break;
                }//check the diagonals
                if (canFlip){
                    for (int i=1;i<flipMark;i++){
                        flipnum++;
                    }
                }
                flipWhite=false,canFlip=false;
                for (int i=1;i<8;i++){
                    if (x-i<0||y-i<0) break;
                    if (gridCondition[x-i][y-i]==0) break;
                    else if (gridCondition[x-i][y-i]==-1) flipWhite=true;
                    else if (gridCondition[x-i][y-i]==1&&flipWhite){
                        flipMark=i;
                        canFlip=true;
                        break;
                    }
                    else if (gridCondition[x-i][y-i]==1&&!flipWhite) break;
                }//check the diagonals
                if (canFlip){
                    for (int i=1;i<flipMark;i++){
                        flipnum++;
                    }
                }
                flipWhite=false,canFlip=false;
                for (int i=1;i<8;i++){
                    if (x-i<0||y+i>=8) break;
                    if (gridCondition[x-i][y+i]==0) break;
                    else if (gridCondition[x-i][y+i]==-1) flipWhite=true;
                    else if (gridCondition[x-i][y+i]==1&&flipWhite){
                        flipMark=i;
                        canFlip=true;
                        break;
                    }
                    else if (gridCondition[x-i][y+i]==1&&!flipWhite) break;
                }//check the diagonals
                if (canFlip){
                    for (int i=1;i<flipMark;i++){
                        flipnum++;
                    }
                }
                flipWhite=false,canFlip=false;
                for (int i=1;i<8;i++){
                    if (x+i>=8||y-i<0) break;
                    if (gridCondition[x+i][y-i]==0) break;
                    else if (gridCondition[x+i][y-i]==-1) flipWhite=true;
                    else if (gridCondition[x+i][y-i]==1&&flipWhite){
                        flipMark=i;
                        canFlip=true;
                        break;
                    }
                    else if (gridCondition[x+i][y-i]==1&&!flipWhite) break;
                }//check the diagonals
                if (canFlip){
                    for (int i=1;i<flipMark;i++){
                        flipnum++;
                    }
                }
                if (flipnum>maxFlip){
                    maxFlip=flipnum;
                    markx=x;
                    marky=y;
                }
            }
        }
    }
    if (findLegal) {
        gridCondition[markx][marky]=1;
        flipWhiteMove(markx,marky);
        blackStone=countBlack();
        whiteStone=countWhite();
        Round++;
    }
}
BoardWidget::BoardWidget(QWidget *parent) : QWidget(parent) {
    memset(gridCondition, 0, sizeof(gridCondition));  // 初始化棋盘状态为0
    cellSize = 60;  // 假设每个格子大小为50像素

    QHBoxLayout *mainLayout = new QHBoxLayout;

    // 创建棋盘部分
    QWidget *boardWidget = new QWidget;
    boardWidget->setMinimumSize(cellSize * 8, cellSize * 8);  // 设置棋盘最小大小

    // 创建 Label 部分
    QVBoxLayout *labelLayout = new QVBoxLayout;  // 创建垂直布局用于 Label
    roundLabel = new QLabel("Round: 1");
    blackCountLabel = new QLabel("Black: 2");
    whiteCountLabel = new QLabel("White: 2");
    currentPlayerLabel = new QLabel("Current Player: Black");
    roundLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");
    blackCountLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");
    whiteCountLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");
    currentPlayerLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");
    labelLayout->addWidget(roundLabel);
    labelLayout->addWidget(blackCountLabel);
    labelLayout->addWidget(whiteCountLabel);
    labelLayout->addWidget(currentPlayerLabel);
    labelLayout->setContentsMargins(0, 0, 0, 0); // 设置边距为0

    // 添加棋盘部分和 Label 部分到主布局
    mainLayout->addWidget(boardWidget);
    mainLayout->addLayout(labelLayout);

    // 设置主布局
    setLayout(mainLayout); // 将主布局设置为窗口的布局
}



void BoardWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 画棋盘
    painter.setBrush(QBrush(QColor(210, 180, 140))); // 棕色背景
    painter.drawRect(0, 0, cellSize * 8, cellSize * 8);  // 绘制棋盘背景

    // 画格子线
    painter.setPen(Qt::black);
    for(int i = 0; i <= 8; ++i) {
        painter.drawLine(i * cellSize, 0, i * cellSize, cellSize * 8);
        painter.drawLine(0, i * cellSize, cellSize * 8, i * cellSize);
    }

    // 画棋子
    for(int row = 0; row < 8; ++row) {
        for(int col = 0; col < 8; ++col) {
            drawPiece(painter, row, col);
        }
    }
}

void BoardWidget::drawPiece(QPainter &painter, int row, int col) {
    if(gridCondition[row][col] != 0) {
        QColor color = (gridCondition[row][col] == 1) ? Qt::black : Qt::white;
        painter.setBrush(QBrush(color));
        painter.drawEllipse((col+0.1) * cellSize, (row+0.1) * cellSize, 0.8*cellSize, 0.8*cellSize);
    }
}

void BoardWidget::mousePressEvent(QMouseEvent *event) {
    QPoint cellPosition = getCellPosition(event->pos());
    int row = cellPosition.y();
    int col = cellPosition.x();
    if (gameMode==1){
    if (Round %2 == 1 && checkBlackLegality(row, col) &&!blackNeedSkip()) {
        setGridValue(row, col, 1);  // 黑棋下
        flipWhiteMove(row,col);
        blackStone=countBlack();
        whiteStone=countWhite();
        Round++;
    }
    else if (Round % 2 == 0 && checkWhiteLegality(row, col) && !whiteNeedSkip()) {
        setGridValue(row, col, -1);  // 白棋下
        flipBlackMove(row,col);
        blackStone=countBlack();
        whiteStone=countWhite();
        Round++;
    }
    else if (Round % 2 == 1 && blackNeedSkip()) {
        QMessageBox::information(this, "Attention", "Dear player (Black), it looks like you don't have any legal moves available this turn. Please press \'OK\' to skip this round.");
        Round++;
    }
    else if (Round % 2 == 1 && whiteNeedSkip()) {
        QMessageBox::information(this, "Attention", "Dear player (White), it looks like you don't have any legal moves available this turn. Please press \'OK\' to skip this round.");
        Round++;
    }
    else {
        QMessageBox::information(this, "Attention", "Invalid move, please have another try!");
    }
    update();  // 重绘棋盘
    // 更新回合数
    roundLabel->setText("Round: " + QString::number(Round));
    roundLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");

    // 更新黑子数和白子数
    blackCountLabel->setText("Black: " + QString::number(blackStone));
    blackCountLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");

    whiteCountLabel->setText("White: " + QString::number(whiteStone));
    whiteCountLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");

    // 更新当前下棋方
    QString currentPlayer = (Round % 2 == 1) ? "Black" : "White";
    currentPlayerLabel->setText("Current Player: " + currentPlayer);
    currentPlayerLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");

    if(endgame()){
        if (blackStone>whiteStone){
            QMessageBox::information(this, "Announcement", "Black Player wins, congrats！");
        }
        else if(blackStone<whiteStone){
            QMessageBox::information(this, "Announcement", "White Player wins, congrats！");
        }
        else{
            QMessageBox::information(this, "Announcement", "Tie！");
        }
    }
    }
    else if (gameMode==2){
        bool havegot=false;
        if(Round%2==1){
            if (checkBlackLegality(row, col) &&!blackNeedSkip()) {
            setGridValue(row, col, 1);  // 黑棋下
            flipWhiteMove(row,col);
            blackStone=countBlack();
            whiteStone=countWhite();
            Round++;
            havegot=true;
            }
            else if (blackNeedSkip()) {
                QMessageBox::information(this, "Attention", "Dear player (Black), it looks like you don't have any legal moves available this turn. Please press \'OK\' to skip this round.");
                Round++;
                havegot=true;
            }
            else {
                QMessageBox::information(this, "Attention", "Invalid move, please have another try!");
            }
        }
        update();  // 重绘棋盘
    // 更新回合数
        roundLabel->setText("Round: " + QString::number(Round));
        roundLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");

    // 更新黑子数和白子数
        blackCountLabel->setText("Black: " + QString::number(blackStone));
        blackCountLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");

        whiteCountLabel->setText("White: " + QString::number(whiteStone));
        whiteCountLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");

    // 更新当前下棋方
        QString currentPlayer = (Round % 2 == 1) ? "Black" : "White";
        currentPlayerLabel->setText("Current Player: " + currentPlayer);
        currentPlayerLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");

        if(endgame()){
            if (blackStone>whiteStone){
                QMessageBox::information(this, "Announcement", "You win, congrats！");
            }
            else if(blackStone<whiteStone){
                QMessageBox::information(this, "Announcement", "Sorry...You lose.");
            }
            else{
                QMessageBox::information(this, "Announcement", "Tie！");
            }
            havegot=false;
        }

        if (havegot){
        if (!whiteNeedSkip()) {
            normalBotWhiteMove();
        }
        else if (whiteNeedSkip()) {
            QMessageBox::information(this, "Attention", "It looks like AI don't have any legal moves available this turn.");
            Round++;
        }
        update();  // 重绘棋盘
    // 更新回合数
        roundLabel->setText("Round: " + QString::number(Round));
        roundLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");

    // 更新黑子数和白子数
        blackCountLabel->setText("Black: " + QString::number(blackStone));
        blackCountLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");

        whiteCountLabel->setText("White: " + QString::number(whiteStone));
        whiteCountLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");

    // 更新当前下棋方
        currentPlayer = (Round % 2 == 1) ? "Black" : "White";
        currentPlayerLabel->setText("Current Player: " + currentPlayer);
        currentPlayerLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");

        if(endgame()){
            if (blackStone>whiteStone){
                QMessageBox::information(this, "Announcement", "You win, congrats！");
            }
            else if(blackStone<whiteStone){
                QMessageBox::information(this, "Announcement", "Sorry...You lose.");
            }
            else{
                QMessageBox::information(this, "Announcement", "Tie！");
            }
        }
        }
    }
    else if (gameMode==3){
        int havegot=false;
        if(Round%2==0){
            if (checkWhiteLegality(row, col) &&!whiteNeedSkip()) {
            setGridValue(row, col, -1);  // 白棋下
            flipBlackMove(row,col);
            blackStone=countBlack();
            whiteStone=countWhite();
            Round++;
            havegot=true;
            }
            else if (whiteNeedSkip()) {
                QMessageBox::information(this, "Attention", "Dear player (White), it looks like you don't have any legal moves available this turn. Please press \'OK\' to skip this round.");
                Round++;
                havegot=true;
            }
            else {
                QMessageBox::information(this, "Attention", "Invalid move, please have another try!");
            }
        }
        update();  // 重绘棋盘
    // 更新回合数
        roundLabel->setText("Round: " + QString::number(Round));
        roundLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");

    // 更新黑子数和白子数
        blackCountLabel->setText("Black: " + QString::number(blackStone));
        blackCountLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");

        whiteCountLabel->setText("White: " + QString::number(whiteStone));
        whiteCountLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");

    // 更新当前下棋方
        QString currentPlayer = (Round % 2 == 1) ? "Black" : "White";
        currentPlayerLabel->setText("Current Player: " + currentPlayer);
        currentPlayerLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");

        if(endgame()){
            if (blackStone>whiteStone){
                QMessageBox::information(this, "Announcement", "Sorry...You lose.");
            }
            else if(blackStone<whiteStone){
                QMessageBox::information(this, "Announcement", "You win, congrats！");
            }
            else{
                QMessageBox::information(this, "Announcement", "Tie！");
            }
        }

        if (havegot){
        if (!blackNeedSkip()) {
            normalBotBlackMove();
        }
        else if (blackNeedSkip()) {
            QMessageBox::information(this, "Attention", "It looks like AI don't have any legal moves available this turn.");
            Round++;
        }
        update();  // 重绘棋盘
    // 更新回合数
        roundLabel->setText("Round: " + QString::number(Round));
        roundLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");

    // 更新黑子数和白子数
        blackCountLabel->setText("Black: " + QString::number(blackStone));
        blackCountLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");

        whiteCountLabel->setText("White: " + QString::number(whiteStone));
        whiteCountLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");

    // 更新当前下棋方
        currentPlayer = (Round % 2 == 1) ? "Black" : "White";
        currentPlayerLabel->setText("Current Player: " + currentPlayer);
        currentPlayerLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");

        if(endgame()){
            if (blackStone>whiteStone){
                QMessageBox::information(this, "Announcement", "Sorry...You lose.");
            }
            else if(blackStone<whiteStone){
                QMessageBox::information(this, "Announcement", "You win, congrats！");
            }
            else{
                QMessageBox::information(this, "Announcement", "Tie！");
            }
        }
    }
    }

}

QPoint BoardWidget::getCellPosition(const QPoint &point) {
    int col = point.x() / cellSize;
    int row = point.y() / cellSize;
    return QPoint(col, row);
}

void BoardWidget::setGridValue(int row, int col, int value) {
    if(row >= 0 && row < 8 && col >= 0 && col < 8) {
        gridCondition[row][col] = value;
    }
}

void BoardWidget::updateLabels(){
    // 更新回合数
    roundLabel->setText("Round: " + QString::number(Round));
    roundLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");

    // 更新黑子数和白子数
    blackCountLabel->setText("Black: " + QString::number(blackStone));
    blackCountLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");

    whiteCountLabel->setText("White: " + QString::number(whiteStone));
    whiteCountLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");

    // 更新当前下棋方
    QString currentPlayer = (Round % 2 == 1) ? "Black" : "White";
    currentPlayerLabel->setText("Current Player: " + currentPlayer);
    currentPlayerLabel->setStyleSheet("QLabel { font-size: 20pt; background-color: rgba(255, 255, 255, 1); border: 1px solid black; padding: 1px; }");
}
