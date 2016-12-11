#include "land.h"

Land::Land()
{
        // IMPLEMENT THIS!
}

Land::~Land()
{
        // IMPLEMENT THIS!
}

BaseObject *Land::createGrid()
{
    _xnum = 60;
    _znum = 60;
    _xn = -0.1;
    _zn = 0;
    _xlen = 50.2;
    _zlen = 60.0;
    double ylevel = 0.0;
    _dx = _xlen/_xnum;
    _dz = _zlen/_znum;


    for (int i = 0; i < _xnum; i++)
        for (int j = 0; j < _znum; j++)
        {
            double r1 = -0.2 + ((double)rand() / RAND_MAX/2.5); //randomizing heights
            _verts.push_back(Vec3d(_xn + _dx*i, ylevel + pow(_dz*j,2)/350 + r1, _zn + _dz*j));
        }

    vector<Vec3i> v;
    v.resize(3);

    // грани поверхности
    for (int i = 0; i < _xnum - 1; i++)
        for (int j = 0; j < _znum - 1; j++)
        {
            v[0][2] = v[0][0] = i*_znum + j;
            v[1][2] = v[1][0] = (i+1)*_znum + j;
            v[2][2] = v[2][0] = (i+1)*_znum + j + 1;
            _faces.push_back(v);

            v[0][2] = v[0][0] = i*_znum + j;
            v[1][2] = v[1][0] = (i+1)*_znum + j + 1;
            v[2][2] = v[2][0] = i*_znum + j + 1;
            _faces.push_back(v);
        }

    // боковые грани
    _verts.push_back(Vec3d(_verts[0].x, 0, _verts[0].z));
    for (int i = 1; i < _xnum; i++)
    {
        _verts.push_back(Vec3d(_verts[i*_znum].x, 0, _verts[i*_znum].z));

        v[2][2] = v[2][0] = (i-1)*_znum;
        v[1][2] = v[1][0] = i*_znum;
        v[0][2] = v[0][0] = _verts.size()-2;
        _faces.push_back(v);

        v[2][2] = v[2][0] = i*_znum;
        v[1][2] = v[1][0] = _verts.size()-1;
        v[0][2] = v[0][0] = _verts.size()-2;
        _faces.push_back(v);
    }

    _verts.push_back(Vec3d(_verts[_xnum-1].x, 0, _verts[_xnum-1].z));
    for (int i = 1; i < _xnum; i++)
    {
        _verts.push_back(Vec3d(_verts[i*_znum + _xnum-1].x, 0, _verts[i*_znum + _xnum-1].z));

        v[0][2] = v[0][0] = (i-1)*_znum + _xnum-1;
        v[1][2] = v[1][0] = i*_znum + _xnum-1;
        v[2][2] = v[2][0] = _verts.size()-2;
        _faces.push_back(v);

        v[0][2] = v[0][0] = i*_znum + _xnum-1;
        v[1][2] = v[1][0] = _verts.size()-1;
        v[2][2] = v[2][0] = _verts.size()-2;
        _faces.push_back(v);
    }

    _verts.push_back(Vec3d(_verts[0].x, 0, _verts[0].z));
    for (int i = 1; i < _xnum; i++)
    {
        _verts.push_back(Vec3d(_verts[i].x, 0, _verts[i].z));

        v[0][2] = v[0][0] = i-1;
        v[1][2] = v[1][0] = i;
        v[2][2] = v[2][0] = _verts.size()-2;
        _faces.push_back(v);

        v[0][2] = v[0][0] = i;
        v[1][2] = v[1][0] = _verts.size()-1;
        v[2][2] = v[2][0] = _verts.size()-2;
        _faces.push_back(v);
    }

    _verts.push_back(Vec3d(_verts[(_znum-1)*_xnum].x, 0, _verts[(_znum-1)*_xnum].z));
    for (int i = 1; i < _xnum; i++)
    {
        _verts.push_back(Vec3d(_verts[(_znum-1)*_xnum + i].x, 0, _verts[(_znum-1)*_xnum + i].z));

        v[2][2] = v[2][0] = (_znum-1)*_xnum + i-1;
        v[1][2] = v[1][0] = (_znum-1)*_xnum + i;
        v[0][2] = v[0][0] = _verts.size()-2;
        _faces.push_back(v);

        v[2][2] = v[2][0] = (_znum-1)*_xnum + i;
        v[1][2] = v[1][0] = _verts.size()-1;
        v[0][2] = v[0][0] = _verts.size()-2;
        _faces.push_back(v);
    }


    _norms.resize(_verts.size());

    this->setColor(QColor(208, 189, 131));
    this->setKoefsFromColor(this->get_koefs(), this->getColor());
    LightKoefs* tmpKoefs = this->get_koefs();
    tmpKoefs->shininess = 2;
    tmpKoefs->spec_r /= 1.5;
    tmpKoefs->spec_g /= 1.5;
    tmpKoefs->spec_b /= 1.5;

    tmpKoefs->diff_r /= 1.25;
    tmpKoefs->diff_g /= 1.25;
    tmpKoefs->diff_b /= 1.25;

    return this;
}

void Land::recalculateNormals()
{
    for (auto norm : _norms)
        norm[0] = norm[1] = norm[2] = 0;


    for (auto face : _faces)
    {
        //for (auto vert : face)
        //    _norms[vert[2]] += _verts[vert[0]];     // vert[2] - номер нормали вершины модели, vert[0]  -номер вершины модели
        Vec3d a = _verts[face[1][0]] - _verts[face[0][0]];
        Vec3d b = _verts[face[2][0]] - _verts[face[0][0]];
        Vec3d facenorm = cross (a, b);
        for (auto vert : face)
            _norms[vert[2]] += facenorm;
    }
}


double Land::getHeight(double x, double z)
{
    if (x < _xn || x > _xn + _xlen || z < _zn || z > _zn + _zlen)
        return 500.0;

    int xn = (int) ((_xn + x)/_dx);
    int zn = (int) ((_zn + z)/_dz);

    double xk = (x - (_xn + _dx*xn)) / (_dx * (2*xn + 1));
    double zk = (z - (_zn + _dz*zn)) / (_dz * (2*zn + 1));

    double y11 = _verts[xn*_znum + zn].y;
    double y12 = _verts[xn*_znum + zn+1].y;
    double y21 = _verts[(xn+1)*_znum + zn].y;
    double y22 = _verts[(xn+1)*_znum + zn+1].y;

    double y1 = y11 + xk*(y12 - y11);
    double y2 = y21 + xk*(y22 - y21);
    double y = y1 + zk*(y2 - y1);

    return y;
}
