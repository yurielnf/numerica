#include "plotter2d.h"

using namespace std;

#include<limits>
#include<cstring>

using namespace numerica;

Plotter2D::Plotter2D(string filename)
    : xLabel("x"),
      yLabel("y"),
      plotLabel(filename),
      ps(*new PSWriter(filename+".ps")),
      xmin(numeric_limits<double>::max()),
      ymin(numeric_limits<double>::max()),
      xmax(numeric_limits<double>::min()),
      ymax(numeric_limits<double>::min()),
      lenYTicks(1)
{}

void Plotter2D::AddPoint(double x, double y,int symbol,double size)
{
    xmin=min(xmin,x);
    xmax=max(xmax,x);
    ymin=min(ymin,y);
    ymax=max(ymax,y);
    points.push_back(PointData(x,y,symbol,size));
}

void Plotter2D::AddPoints(const NVector &xs, const NVector &ys, int symbol, double size)
{
    xmin=min(xmin,xs.min());
    xmax=max(xmax,xs.max());
    ymin=min(ymin,ys.min());
    ymax=max(ymax,ys.max());
    for(size_t i=0;i<xs.size();i++)
        points.push_back(PointData(xs[i],ys[i],symbol,size));
}

void Plotter2D::AddHorLine(double y0, double x1, double x2,
                           string dashed, double red, double green, double blue, int nPoints)
{
    NVector x(nPoints),y(nPoints);
    double dx=(x2-x1)/(nPoints-1);
    for(int i=0;i<nPoints;i++)
    {
        x[i]=x1+dx*i;
        y[i]=y0;
    }
    xmin=std::min(xmin,x.min());
    ymin=std::min(ymin,y.min());
    xmax=std::max(xmax,x.max());
    ymax=std::max(ymax,y.max());
    funs.push_back(FunctionData(x,y,dashed,red,green,blue));
}

void Plotter2D::AddVerLine(double x0, double y1, double y2,
                           string dashed, double red, double green, double blue, int nPoints)
{
    NVector x(nPoints),y(nPoints);
    double dy=(y2-y1)/(nPoints-1);
    for(int i=0;i<nPoints;i++)
    {
        x[i]=x0;
        y[i]=y1+dy*i;
    }
    xmin=std::min(xmin,x.min());
    ymin=std::min(ymin,y.min());
    xmax=std::max(xmax,x.max());
    ymax=std::max(ymax,y.max());
    funs.push_back(FunctionData(x,y,dashed,red,green,blue));
}


void Plotter2D::DoIt()
{
    DrawFrame();
    DrawTicks();

    for(size_t i=0;i<points.size();i++)
        ps.DrawSymbol(TransformX(points[i].x),TransformY(points[i].y),
                      points[i].symbol,points[i].size);
    DrawLabels();
    DrawFunctions();
    delete &ps;
}

void Plotter2D::AddFunction(const Function& f,double x1,double x2,
                            string dashed,double red,double green,double blue,int nPoints)
{
    NVector x(nPoints),y(nPoints);
    double dx=(x2-x1)/(nPoints-1);
    for(int i=0;i<nPoints;i++)
    {
        x[i]=x1+dx*i;
        y[i]=f(x[i]);
    }
    xmin=std::min(xmin,x.min());
    ymin=std::min(ymin,y.min());
    xmax=std::max(xmax,x.max());
    ymax=std::max(ymax,y.max());
    funs.push_back(FunctionData(x,y,dashed,red,green,blue));
}

void Plotter2D::AddFunction(const Function& f,double x1,double x2,vector<double> &xs,
                            string dashed,double red,double green,double blue,int nPoints)
{
    //check range!

    vector<double> xn;
    xn.push_back(x1);
    for(size_t i=0;i<xs.size();i++)
        xn.push_back(xs[i]);
    xn.push_back(x2);

    double dx=(x2-x1)/(nPoints-1);
    for(size_t i=1;i<xn.size();i++)
    {
        int n=(int)((xn[i]-xn[i-1])/dx);
        AddFunction(f,xn[i-1]+1e-6*dx,xn[i]-1e-6*dx,dashed,red,green,blue,n);
        if (i<xn.size()-1)
            AddVerLine(xn[i],f(xn[i]-1e-6*dx),f(xn[i]+1e-6*dx),
                       dashed,red,green,blue,2);
    }
}

//------------------- Private methods -----------------------------------


void Plotter2D::DrawLabels()
{
    static double fontSize=18;
    if (xLabel=="" && yLabel=="" && plotLabel=="")
        return;

    ps.SetFont("Arial",fontSize);
    ps.SetTextJustification(TextJustification::Centered);
    if (xLabel!="")
        ps.DrawText(0.5*gxSize+gx1,gy1-ymarg-1.3*fontSize-ticksfsize,xLabel);
    if (yLabel!="")
        ps.DrawText(gx1-xmarg-fontSize-0.5*lenYTicks*ticksfsize,0.5*gySize+gy1,yLabel,90);
    if (plotLabel!="")
        ps.DrawText(0.5*gxSize+gx1,gy1+gySize+ymarg+0.5*fontSize,plotLabel);

}


void Plotter2D::DrawFrame()
{
    static double ddx[]={gx1-xmarg,gx1-xmarg,gx1+gxSize+xmarg,gx1+gxSize+xmarg};
    static double ddy[]={gy1-ymarg,gy1+gySize+ymarg,gy1+gySize+ymarg,gy1-ymarg};
    static NVector dx(ddx,4),dy(ddy,4);
    ps.DrawConnectedLine(dx,dy,true);
    if (ymin*ymax<0)
    {
        ps.SetDashedLine();
        ps.DrawLine(gx1-xmarg,TransformY(0),gx1+gxSize+xmarg,TransformY(0));
        ps.SetSolidLine();
    }
}

void Plotter2D::DrawFunctions()
{
    double x1,y1,x2,y2;
    for(size_t i=0;i<funs.size();i++)
    {
        FunctionData& fd=funs[i];
        ps.SetDashedLine(fd.dashed);
        ps.SetColor(fd.red,fd.green,fd.blue);
        for (size_t j=0;j<fd.xs.size()-1;j++)
        {
            x1=TransformX(fd.xs[j]);
            x2=TransformX(fd.xs[j+1]);
            y1=TransformY(fd.ys[j]);
            y2=TransformY(fd.ys[j+1]);
            ps.DrawLine(x1,y1,x2,y2);
        }
    }
}

vector<double> Plotter2D::DecimalPartition(double xLower, double xUpper, int ndiv)
{
    double dx,expo,x0,d;
    dx=(xUpper-xLower)/ndiv;
    expo=pow(10,floor(log10(dx)));
    int cif=(int) (dx/expo+.5);
    d=cif*expo;
    x0=((int)(xLower/expo)+1)*expo;
    vector<double> ts;
    for(double x=x0;x<=xUpper;x+=d)
        ts.push_back(x);
    return ts;
}

vector<double> Plotter2D::DecimalMinorPartition
(double xLower, double xUpper, const vector<double> &ticks)

{
    double d=ticks[1]-ticks[0];
    double x0=ticks[0];
    vector<double> ts;
    for(double x=x0;x>=xLower;x-=0.2*d)
        ts.push_back(x);
    for(double x=x0+0.2*d;x<=xUpper;x+=0.2*d)
        ts.push_back(x);
    return ts;
}

void Plotter2D::DrawTicks()
{
    if (xmin==xmax)
    {
        xmin-=1;
        xmax+=1;
    }
    if (ymin==ymax)
    {
        ymin-=1;
        ymax+=1;
    }
    vector<double>
            ticksX=DecimalPartition(xmin,xmax,5),
            ticksXMinor=DecimalMinorPartition(xmin,xmax,ticksX),
            ticksY=DecimalPartition(ymin,ymax,5),
            ticksYMinor=DecimalMinorPartition(ymin,ymax,ticksY);


    DrawXTicks(ticksX,8);
    DrawXTicks(ticksXMinor,4,false);
    DrawYTicks(ticksY,8);
    DrawYTicks(ticksYMinor,4,false);
}

void Plotter2D::DrawXTicks(vector<double> ticks, double size,bool label)
{
    char str[20];
    double x;
    if (label)
    {
        ps.SetFont("Arial",ticksfsize);
        ps.SetTextJustification(TextJustification::Centered);
    }
    for(size_t i=0;i<ticks.size();i++)
    {
        x=TransformX(ticks[i]);
        ps.DrawLine(x,gy1-ymarg,x,gy1-ymarg+size);
        ps.DrawLine(x,gy1+ymarg+gySize-size,x,gy1+ymarg+gySize);
        if (label)
        {
            sprintf(str,"%g",ticks[i]);
            ps.DrawText(x,gy1-ymarg-ticksfsize,str);
        }
    }

}

void Plotter2D::DrawYTicks(vector<double> ticks, double size,bool label)
{
    char str[20];
    double y;
    if (label)
        ps.SetTextJustification(TextJustification::Right);
    for(size_t i=0;i<ticks.size();i++)
    {
        y=TransformY(ticks[i]);
        ps.DrawLine(gx1-xmarg,y,gx1-xmarg+size,y);
        ps.DrawLine(gx1+gxSize+xmarg-size,y,gx1+gxSize+xmarg,y);
        if (label)
        {
            sprintf(str,"%g",ticks[i]);
            ps.DrawText(gx1-xmarg-0.3*ticksfsize,y-0.3*ticksfsize,str);
            if (strlen(str)>lenYTicks)
                lenYTicks=strlen(str);
        }
    }
}


inline double Plotter2D::TransformX(double x)
{
    return gx1+gxSize*(x-xmin)/(xmax-xmin);
}

inline double Plotter2D::TransformY(double y)
{
    return gy1+gySize*(y-ymin)/(ymax-ymin);
}

void numerica::plot(const function<double(double)>& f,double x1,double x2)
{
    static char i='A';
    Plotter2D p(string("plot")+i++);
    p.AddFunction(f,x1,x2);
    p.DoIt();
}
