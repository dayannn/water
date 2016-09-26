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

class Model : public BaseObject{
public:
    Model();
    ~Model();

    BaseObject* loadFromFile(const std::string *filename);

    vector<Vec3d>& getVerts();
    vector<vector<Vec3i> > &getFaces();

    Vec3d& vertice(int i);
    vector<Vec3i> &face(int i);
    Vec3d& norm(int iface, int nvert);

    size_t getVertsNum() const;
    size_t getFacesNum() const;

    QImage* getTexture();

    QColor diffuse(Vec2i uv);
    Vec2i uv(int iface, int nvert);

    PositionInfo& getPositionInfo();

    QColor& getColor();
    void setColor (QColor color);

    void remakeNormals();

protected:
    PositionInfo posInfo;

    vector<Vec3d> _verts;
    vector<vector<Vec3i> > _faces;
    vector<Vec3d> _norms;
    vector<Vec2d> _uv;

    QColor _color;

    QImage _diffusemap;
    void loadTexture(const std::string *filename, const char* suffix);
};

#endif // Model_H
