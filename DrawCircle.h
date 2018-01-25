#include <vector>
using namespace std;

class CDrawCircle
{
private:
    vector<vector<unsigned char>> m_Raster;  //2D array to save the characters which can direct out put to console or file which will show a circle
    unsigned int m_DimX;   //It is half of the width of the  2D array  radius of circle will be the  smaller one of m_DimX and m_DimY
    unsigned int m_DimY;   //It is half of the heigt of the  2D array
   
    const unsigned char m_CharInCircle = '*';
    unsigned int m_RadiusSquare;   //Radius is alwasy m_DimY here, so we don't need to store radius explicitly

    CDrawCircle& operator=(const CDrawCircle&); // Not defined elsewhere

    CDrawCircle(const CDrawCircle&); // Not defined elsewhere

public:
    virtual bool PrintToConsole(bool bNeedRotate);

    CDrawCircle(unsigned int DimX, unsigned int DimY);

    bool DrawCircle();

    vector<vector<unsigned char>> getRaster();

    void set(int x, int y); // please define

    void GetNextCandidateByIncreaseX(unsigned int &x, unsigned int &y);
    void GetNextCandidateByDecreaseX(unsigned int &x, unsigned int &y);

};