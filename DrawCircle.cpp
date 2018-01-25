// DrawCircle.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream> 
#include "DrawCircle.h"
#include <windows.h>

CDrawCircle::CDrawCircle(unsigned int DimX, unsigned int DimY) : m_DimX(DimX), m_DimY(DimY)
{    
}

bool CDrawCircle::DrawCircle()
{
    if (m_DimX == 0 || m_DimY == 0)
        return false;
    bool bNeedRotate = false;
    if (m_DimX < m_DimY){   //Make sure m_DimX is always greater than m_DimY so we can process it easily
        unsigned int temp = m_DimY;
        m_DimY = m_DimX;
        m_DimX = temp;
        bNeedRotate = true;
    }
    m_RadiusSquare = m_DimY * m_DimY;
    
    
    m_Raster.resize(m_DimY * 2 + 1, vector<unsigned char>(m_DimX * 2 + 1, ' '));
    unsigned int X = 0;
    unsigned int Y = m_DimY;
    unsigned int newX = 0;   //This is the adjusted X dimension value based on ratio m_DimY / m_DimX;
    
    while (Y >= X * m_DimY / m_DimX){
        set(X, Y);
        GetNextCandidateByIncreaseX(X, Y);
    }    
    
    X = m_DimX;
    Y = 0;
    while (X * m_DimY / m_DimX >= Y){
        set(X, Y);
        GetNextCandidateByDecreaseX(X, Y);
    }    

    return PrintToConsole(bNeedRotate);     
}

//For the next point(location of '*'), There will 3 possible candidats: (X+1, Y), (X, Y-1) and (X+1, Y-1)
void CDrawCircle::GetNextCandidateByIncreaseX(unsigned int &x, unsigned int &y)
{
    unsigned int newX = x * m_DimY / m_DimX;
    unsigned int newXPlus1 = (x+1) * m_DimY / m_DimX;
    
    unsigned int XPlus1_Y = abs(int(newXPlus1 * newXPlus1 + y * y - m_RadiusSquare));
    unsigned int X_YMinus1 = abs((int)(newX*newX + (y - 1) * (y - 1) - m_RadiusSquare));
    unsigned int XPlus1_YMinus1 = abs((int)(newXPlus1*newXPlus1 + (y - 1) * (y - 1) - m_RadiusSquare));

    unsigned int minVal = min(X_YMinus1, XPlus1_Y);
    
    if (XPlus1_YMinus1 <= minVal){
        x++;
        y--;
    }
    else if (minVal == XPlus1_Y){
        x++;
    }
    else
        y--;

    return;    
}

//For the next point(location of '*'), There will 3 possible candidats: (X-1, Y), (X, Y+1) and (X-1, Y+1)
void CDrawCircle::GetNextCandidateByDecreaseX(unsigned int &x, unsigned int &y)
{
    unsigned int newX = x * m_DimY / m_DimX;
    unsigned int newXMinus1 = (x - 1) * m_DimY / m_DimX;
    unsigned int X_YPlus1 = abs((int)(newX*newX + (y + 1) * (y + 1) - m_RadiusSquare));
    unsigned int XMinus1_Y = abs(int(newXMinus1 * newXMinus1 + y * y - m_RadiusSquare));
    unsigned int XMinus1_YPlus1 = abs((int)(newXMinus1*newXMinus1 + (y + 1) * (y + 1) - m_RadiusSquare));

    unsigned int minVal = min(X_YPlus1, XMinus1_Y);

    if (XMinus1_YPlus1 <= minVal){
        x--;
        y++;
    }
    else if (minVal == X_YPlus1){
        y++;
    }
    else
        x--;
    return;
}
bool CDrawCircle::PrintToConsole(bool bNeedRotate)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    if (m_DimY > (unsigned int)(min(csbi.dwMaximumWindowSize.X, csbi.dwMaximumWindowSize.Y) - 1) / 2 ||
        (bNeedRotate && m_DimX > (unsigned int)(csbi.dwMaximumWindowSize.Y - 1) / 2) ||
        (!bNeedRotate && m_DimX > (unsigned int)(csbi.dwMaximumWindowSize.X - 1) / 2))
    {
        cout << "Can't print circle with radius=" << m_DimY << " with diplay resultion(ratio) as following " << m_DimX << "x" << m_DimY << " as it exceeds maximum size of current console!";
        return false;
    }
    if (bNeedRotate){
        for (unsigned int j = 0; j < m_Raster[0].size(); j++){
            for (unsigned int i = 0; i < m_Raster.size(); i++)            
                cout << m_Raster[i][j];
            cout << endl;
        }
    }
    else{
        for (unsigned int i = 0; i < m_Raster.size(); i++){
            for (unsigned int j = 0; j < m_Raster[0].size(); j++)
                cout << m_Raster[i][j];
            cout << endl;
        }
    }    
    return true;
}

//This method will put '*' to the 2D array. Considering all the '*' is symmetrical by x-axis and y-axis.
//So we actually can set 4 '*' together by one pair of (X,Y).
void CDrawCircle::set(int x, int y) // please define
{
    m_Raster[m_DimY - y][x + m_DimX] = m_CharInCircle;
    m_Raster[m_DimY + y][x + m_DimX] = m_CharInCircle;
    m_Raster[m_DimY - y][m_DimX - x] = m_CharInCircle;
    m_Raster[m_DimY + y][m_DimX - x] = m_CharInCircle;
}

vector<vector<unsigned char>> CDrawCircle::getRaster()
{
    return m_Raster;
}



