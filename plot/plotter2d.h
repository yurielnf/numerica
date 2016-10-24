#ifndef PLOTTER2D_H
#define PLOTTER2D_H

#include "pswriter.h"
#include<vector>
#include<sstream>
#include<functional>

namespace numerica
{
using std::vector;
using std::string;
using std::function;
typedef function<double(double)> Function;
void plot(const Function& f,double x1,double x2);

class Plotter2D
{
public:
    Plotter2D(string filename="myplot");
    void AddPoint(double x,double y,int symbol=154,double size=12);
    void AddPoints(const NVector& xs,const NVector& ys,int symbol=154,double size=10);
    void AddFunction(const Function& f,double x1,double x2,
                     string dashed="",double red=1,double green=0,double blue=0,int nPoints=100);
    void AddFunction(const Function& f,double x1,double x2,vector<double> &discontinuities,
                     string dashed="",double red=1,double green=0,double blue=0,int nPoints=100);

    void AddHorLine(double y0,double x1,double x2,
                    string dashed="",double red=1,double green=0,double blue=0,int nPoints=10);
    void AddVerLine(double x0,double y1,double y2,
                    string dashed="",double red=1,double green=0,double blue=0,int nPoints=10);
    void DoIt();

    string xLabel,yLabel,plotLabel;

private:
    void DrawFrame();
    void DrawTicks();
    void DrawXTicks(std::vector<double> ticks,double size,bool label=true);
    void DrawYTicks(std::vector<double> ticks,double size,bool label=true);
    void DrawLabels();
    void DrawFunctions();
    inline double TransformX(double x);
    inline double TransformY(double y);
    vector<double> DecimalPartition(double xLower,double xUpper,int ndiv);
    vector<double> DecimalMinorPartition(double xLower,double xUpper,
                                         const vector<double>& ticks);


    constexpr static double gx1=97.5+50,gy1=343.5,
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
    FunctionData(NVector& xs,NVector& ys,string dashed="",
                 double red=1,double green=1,double blue=1)
        :xs(xs),ys(ys),dashed(dashed),red(red),
          green(green),blue(blue)
    {}
    NVector xs,ys;
    string dashed;
    double red,green,blue;
};

}//namespace numerica



#endif // PLOTTER2D_H
