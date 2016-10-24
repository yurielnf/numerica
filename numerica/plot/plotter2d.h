#ifndef PLOTTER2D_H
#define PLOTTER2D_H

#include "pswriter.h"
#include<vector>
#include<functional>

namespace numerica
{
    namespace tools
    {
        using std::vector;
        using std::string;
        using std::function;
        typedef function<double(double)> NFunction;

        void plot(NFunction f,double x1,double x2);

        class Plotter2D
        {
        public:
            Plotter2D(string filename="myplot");
            void AddPoint(double x,double y,int symbol=154,double size=12);
            void AddPoints(const NVector& xs,const NVector& ys,int symbol=154,double size=10);
            void AddFunction(NFunction f,double x1,double x2,RGBColor color=Red,string dashed="",int nPoints=100);
            void AddHorLine(double y0,double x1,double x2,RGBColor color,string dashed="",int nPoints=10);
            void AddVerLine(double x0,double y1,double y2,RGBColor color,string dashed="",int nPoints=10);
            void DoIt();

            string xLabel,yLabel,plotLabel;

        private:
            void UpdateMinMax(double xmin,double xmax,double ymin,double ymax);
            void DrawFrame();
            void DrawTicks();
            void DrawXTicks(std::vector<double> ticks,double size,bool label=true);
            void DrawYTicks(std::vector<double> ticks,double size,bool label=true);
            void DrawLabels();
            void DrawFunctions();
            inline double TransformX(double x);
            inline double TransformY(double y);

            static constexpr double
                    gx1=147.5,gy1=343.5,
                    gxSize=400,gySize=360,
                    xmarg=15,ymarg=15,
                    ticksfsize=16;

            PSWriter& ps;
            double xmin,ymin,xmax,ymax;
            unsigned lenYTicks;
            struct PointData;
            struct FunctionData;
            vector<PointData> points;
            vector<FunctionData> funs;
        };

        //------------- Other definitions -------------------------------------

        struct Plotter2D::PointData
        {
            PointData(double x,double y,int symbol,double size)
                :x(x),y(y),symbol(symbol),size(size){}
            double x,y;
            int symbol;
            double size;
        };

        struct Plotter2D::FunctionData
        {
            FunctionData(NVector& xs,NVector& ys,RGBColor col,string dashed="")
                :xs(xs),ys(ys),dashed(dashed),col(col)
            {}
            NVector xs,ys;
            string dashed;
            RGBColor col;
        };
    }
}


#endif // PLOTTER2D_H
