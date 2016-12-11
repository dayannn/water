#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <QImage>
#include <QColor>
#include "baseobject.h"
#include "geometry.h"
#include <vector>
using std::vector;

struct PositionInfo{
    double x;
    double y;
    double z;

    double alpha;
    double beta;
    double gamma;

    double scale;
};

struct LightKoefs{
    double transparency;
    double amb_r;
    double amb_g;
    double amb_b;
    double diff_r;
    double diff_g;
    double diff_b;
    double spec_r;
    double spec_g;
    double spec_b;
    double shininess;
};

class Model : public BaseObject{
public:
    Model();
    ~Model();

    BaseObject* loadFromFile(const std::string *filename);

    vector<Vec3d>& getVerts();
    vector<vector<Vec3i> > &getFaces();

    Vec3d& vertice(int i);
    Vec3d &vertice(int iface, int nvert);
    vector<Vec3i> &face(int i);
    Vec3d& norm(int iface, int nvert);

    size_t getVertsNum() const;
    size_t getFacesNum() const;

    QImage* getTexture();

    QColor diffuse(Vec2i uv);
    Vec2i uv(int iface, int nvert);

    PositionInfo& getPositionInfo();

    LightKoefs* get_koefs();
    void set_koefs(LightKoefs* koefs);

    QColor& getColor();
    void setColor (QColor color);

    void remakeNormals();
    void reverseNormals();
    void changeVertsOrder();

    void setKoefsFromColor(LightKoefs* koefs, QColor clr);

protected:
    PositionInfo posInfo;

    vector<Vec3d> _verts;
    vector<vector<Vec3i> > _faces;
    vector<Vec3d> _norms;
    vector<Vec3d> _facenorms;
    vector<Vec2d> _uv;

    QColor _color;
    LightKoefs _koefs;
    QImage _diffusemap;
    void loadTexture(const std::string *filename, const char* suffix);
};

#endif // Model_H
