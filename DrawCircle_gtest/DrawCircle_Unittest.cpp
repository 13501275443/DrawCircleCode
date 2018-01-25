#include "stdafx.h"
#include "..\DrawCircle.h"
#include "CppUnitTest.h"
#include "windows.h"
#include "set"
#include <iostream> 
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DrawCircle_gtest
{		

    class DrawCircle_Testable : public CDrawCircle{
        
    public:
        DrawCircle_Testable(unsigned int DimX, unsigned int DimY) : CDrawCircle(DimX, DimY){}

        //This method is for unit test only since we don't want output to console for unit test
        virtual bool PrintToConsole(bool bNeedRotate){
            return true;
        }
        
    };
    TEST_CLASS(UnitTest_DrawCircle)
	{
	public:
        //////////////////////////////////////////////////////////////////////
        //Those two method are the core test code to validate the 2D data formed a circle with '*'

        //This method will check whether the '*' is symmetrical by x-axis and y-axis 
        bool CheckNumberOfData(vector<vector<unsigned char>> &twoDimdata)
        {
            //Make sure every col has at least two '*'
            set<unsigned int> setIndexForRowCheck;
            vector<unsigned int> vIndex;
            for (unsigned int X = 0; X < twoDimdata[0].size(); X++){
                unsigned int countColStar = 0;
                vector<unsigned int> vIndex;
                for (unsigned int Y = 0; Y < twoDimdata.size(); Y++)
                    if (twoDimdata[Y][X] == '*'){
                        countColStar++;
                        vIndex.push_back(Y);
                    }
                //Only first and last col may contain odd number of '*'
                if (X > 0 && X < twoDimdata[0].size() - 1 && countColStar % 2 == 1)
                    return false;
                if (countColStar < 2 )  //Which means we miss some stars
                    return false;
                if (countColStar > 2)
                    setIndexForRowCheck.insert(vIndex.begin(), vIndex.end());
            }

            //Make sure every row has at least two '*'
            set<unsigned int> setIndexForColCheck;
            for (unsigned int Y = 0; Y < twoDimdata.size(); Y++){
                unsigned int countRowStar = 0;
                vector<unsigned int> vIndex;
                for (unsigned int X = 0; X < twoDimdata[Y].size(); X++)
                {
                    if (twoDimdata[Y][X] == '*'){
                        vIndex.push_back(X);
                        countRowStar++;
                    }
                }
                //Only first and last row may contain odd number of '*'
                if (Y > 0 && Y < twoDimdata.size() - 1 && countRowStar % 2 == 1)
                    return false;
                if (countRowStar <2 )  //Which means we miss some stars
                    return false;
                if (countRowStar > 2)
                    setIndexForColCheck.insert(vIndex.begin(), vIndex.end());
            }

            //Make sure we don't write too much '*'. It is a little tricky.
            //We need to check if there are more than 2 '*' in any row(or col), then we record the col(or row) for each '*'.
            //Then we check col(or row) we got from previous step, make sure there are only 2 '*'.
            for (int rowIndex : setIndexForRowCheck){
                vector<unsigned char> row = twoDimdata[rowIndex];
                unsigned int countRowStar = 0;
                for (unsigned char c : row){
                    if (c == '*')
                        countRowStar++;                  
                }
                if (countRowStar !=2)  //Which means we write too much '*'
                    return false;
            }

            for (int colIndex : setIndexForColCheck){
                unsigned int countColStar = 0;
                for (unsigned int Y = 0; Y < twoDimdata.size(); Y++)
                {
                    if (twoDimdata[Y][colIndex] == '*')
                        countColStar++;
                }
                if (countColStar != 2)  //Which means we write too much '*'
                    return false;
            }
            return true;
        }

        bool CircleValidate(vector<vector<unsigned char>> &twoDimdata, unsigned int dimX, unsigned int dimY)
        {
            unsigned int radium = min(dimX, dimY);
            

            unsigned int centerY = twoDimdata.size() / 2;
            unsigned int centerX = twoDimdata[0].size() / 2;
            if (dimY > dimX){
                unsigned int temp = centerX;                
                temp = dimX;
                dimX = dimY;
                dimY = temp;
            }
            for (unsigned int Y = 0; Y < twoDimdata.size(); Y++){
                for (unsigned int X = 0; X < twoDimdata[0].size(); X++)
                {
                    if (twoDimdata[Y][X] == '*')
                    {
                        unsigned int YY = Y > centerY ? Y - centerY : centerY -Y;
                        unsigned int XX = X > centerX ? X - centerX : centerX - X;
                        unsigned int newX = XX * dimY / dimX;                        
                        double r = sqrt(YY*YY +newX * newX);
                        if (abs(radium - r) > 0.5)
                            return false;
                    }
                }
            }            
            return true;
        }
        //////////////////////////////////////////////////////////////////////////////////////////////

		TEST_METHOD(Test_DimensionIsZero)
		{
			DrawCircle_Testable drawCircleTest1 = DrawCircle_Testable(0, 1);
            Assert::IsFalse(drawCircleTest1.DrawCircle());
            DrawCircle_Testable drawCircleTest2 = DrawCircle_Testable(100, 0);
            Assert::IsFalse(drawCircleTest2.DrawCircle());
		}

        

        TEST_METHOD(Test_DataValidationForDimXAndDimYIsSame)
        {
            int X = 13;
            int Y = 13;
            DrawCircle_Testable drawCircleTest1 = DrawCircle_Testable(X, Y);
            drawCircleTest1.DrawCircle();
            vector<vector<unsigned char>> twoDimdata = drawCircleTest1.getRaster();
            Assert::IsTrue(twoDimdata.size()>0 && twoDimdata[0].size()>0);
            Assert::IsTrue(CheckNumberOfData(twoDimdata));
            Assert::IsTrue(CircleValidate(twoDimdata, X,Y));
        }

        TEST_METHOD(Test_DataValidationForDimXAndDimYIsSame2)
        {
            int X = 2;
            int Y = 2;
            DrawCircle_Testable drawCircleTest1 = DrawCircle_Testable(X, Y);
            drawCircleTest1.DrawCircle();
            vector<vector<unsigned char>> twoDimdata = drawCircleTest1.getRaster();
            Assert::IsTrue(twoDimdata.size()>0 && twoDimdata[0].size()>0);
            Assert::IsTrue(CheckNumberOfData(twoDimdata));
            Assert::IsTrue(CircleValidate(twoDimdata, X, Y));
        }

        TEST_METHOD(Test_DataValidationForDimXGreaterThanDimY)
        {
            int X = 30;
            int Y = 20;
            DrawCircle_Testable drawCircleTest1 = DrawCircle_Testable(X, Y);
            drawCircleTest1.DrawCircle();
            vector<vector<unsigned char>> twoDimdata = drawCircleTest1.getRaster();
            Assert::IsTrue(twoDimdata.size()>0 && twoDimdata[0].size()>0);
            Assert::IsTrue(CheckNumberOfData(twoDimdata));
            Assert::IsTrue(CircleValidate(twoDimdata, X, Y));
        }

        TEST_METHOD(Test_DataValidationForDimXGreaterThanDimY2)
        {
            int X = 31;
            int Y = 6;
            DrawCircle_Testable drawCircleTest1 = DrawCircle_Testable(X, Y);
            drawCircleTest1.DrawCircle();
            vector<vector<unsigned char>> twoDimdata = drawCircleTest1.getRaster();
            Assert::IsTrue(twoDimdata.size()>0 && twoDimdata[0].size()>0);
            Assert::IsTrue(CheckNumberOfData(twoDimdata));
            Assert::IsTrue(CircleValidate(twoDimdata, X, Y));
        }

        TEST_METHOD(Test_DataValidationForDimXGreaterThanDimY3)
        {
            int X = 33;
            int Y = 7;
            DrawCircle_Testable drawCircleTest1 = DrawCircle_Testable(X, Y);
            drawCircleTest1.DrawCircle();
            vector<vector<unsigned char>> twoDimdata = drawCircleTest1.getRaster();
            Assert::IsTrue(twoDimdata.size()>0 && twoDimdata[0].size()>0);
            Assert::IsTrue(CheckNumberOfData(twoDimdata));
            Assert::IsTrue(CircleValidate(twoDimdata, X, Y));
        }

        TEST_METHOD(Test_DataValidationForDimXLessThanDimY)
        {
            int X = 5;
            int Y = 30;
            DrawCircle_Testable drawCircleTest1 = DrawCircle_Testable(X, Y);
            drawCircleTest1.DrawCircle();
            vector<vector<unsigned char>> twoDimdata = drawCircleTest1.getRaster();
            Assert::IsTrue(twoDimdata.size()>0 && twoDimdata[0].size()>0);
            Assert::IsTrue(CheckNumberOfData(twoDimdata));
            Assert::IsTrue(CircleValidate(twoDimdata, X, Y));
        }

        TEST_METHOD(Test_DataValidationForDimXLessThanDimY2)
        {
            int X = 15;
            int Y = 22;
            DrawCircle_Testable drawCircleTest1 = DrawCircle_Testable(X, Y);
            drawCircleTest1.DrawCircle();
            vector<vector<unsigned char>> twoDimdata = drawCircleTest1.getRaster();
            Assert::IsTrue(twoDimdata.size()>0 && twoDimdata[0].size()>0);
            Assert::IsTrue(CheckNumberOfData(twoDimdata));
            Assert::IsTrue(CircleValidate(twoDimdata, X, Y));
        }

        TEST_METHOD(Test_PerformanceTest)
        {
            int X = 30;
            int Y = 20;
            DrawCircle_Testable drawCircleTest = DrawCircle_Testable(X, Y);

            //// Perform time-consuming operation
            long int before = GetTickCount();            
            for (int i = 0; i < 10000; i++)
                drawCircleTest.DrawCircle();
            long int after = GetTickCount();
            Assert::IsTrue(after - before < 210);  //210ms which is hardware depended, may need to change it for different computer.
        }
	};
}