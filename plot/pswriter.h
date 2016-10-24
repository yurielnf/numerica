#ifndef PSWRITER_H
#define PSWRITER_H

#include<valarray>
#include<string>
#include<fstream>


typedef std::valarray<double> NVector;

using std::string;

enum class TextJustification{Left,Centered,Right};

class PSWriter
{
public:
    PSWriter(string filename);
    ~PSWriter();
    void Close();

    void SetFont(string fontName, double size);
    void SetLineWidth(double width);
    void SetColor(double red, double green, double blue);
    void SetDashedLine(string pattern="4 2");
    void SetSolidLine();
    void SetTextJustification(TextJustification j=TextJustification::Left);

    void DrawText(double x, double y, string text,double rot=0.0);
    void DrawSymbol(double x, double y, int num=108, double size=12);
    void DrawLine(double x1, double y1, double x2, double y2);
    void DrawConnectedLine(const NVector& xs,const NVector& ys,bool closed=false);

    static string path;


private:
    std::ofstream f;
    std::string tjCmd, name;
};




#endif // PSWRITER_H
