#include "test_plot.h"
#include "plot/plotter2d.h"
#include<cmath>

using namespace numerica::tools;

void TestPlot()
{
    plot(sin,1,1.15);
    Plotter2D p("testing");
    for(double x=0;x<10;x+=.1)
        p.AddPoint(x,sin(x));
    p.xLabel="abcisa x";
    p.yLabel="y=sin(x)";
    p.plotLabel="grafica del sin(x)";

    p.AddFunction(cos,0,10);
    p.AddFunction(atan,0,10,RGBColor(0,1,0),"2 6");
    p.DoIt();
}
