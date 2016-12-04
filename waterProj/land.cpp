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
    int xnum = 60;
    int znum = 60;
    double xn = -0.1;
    double zn = 0;
    double xlen = 50.2;
    double zlen = 60.0;
    double ylevel = 0.0;
    double dx = xlen/xnum;
    double dz = zlen/znum;


    for (int i = 0; i < xnum; i++)
        for (int j = 0; j < znum; j++)
        {
            double r1 = -0.2 + ((double)rand() / RAND_MAX/2.5); //randomizing heights
            _verts.push_back(Vec3d(xn + dx*i, ylevel + pow(dz*j,2)/350 + r1, zn + dz*j));
        }

    vector<Vec3i> v;
    v.resize(3);

    // грани поверхности
    for (int i = 0; i < xnum - 1; i++)
        for (int j = 0; j < znum - 1; j++)
        {
            v[0][2] = v[0][0] = i*znum + j;
            v[1][2] = v[1][0] = (i+1)*znum + j;
            v[2][2] = v[2][0] = (i+1)*znum + j + 1;
            _faces.push_back(v);

            v[0][2] = v[0][0] = i*znum + j;
            v[1][2] = v[1][0] = (i+1)*znum + j + 1;
            v[2][2] = v[2][0] = i*znum + j + 1;
            _faces.push_back(v);
        }






    // боковые грани
    _verts.push_back(Vec3d(_verts[0].x, 0, _verts[0].z));
    for (int i = 1; i < xnum; i++)
    {
        _verts.push_back(Vec3d(_verts[i*znum].x, 0, _verts[i*znum].z));

        v[2][2] = v[2][0] = (i-1)*znum;
        v[1][2] = v[1][0] = i*znum;
        v[0][2] = v[0][0] = _verts.size()-2;
        _faces.push_back(v);

        v[2][2] = v[2][0] = i*znum;
        v[1][2] = v[1][0] = _verts.size()-1;
        v[0][2] = v[0][0] = _verts.size()-2;
        _faces.push_back(v);
    }

    _verts.push_back(Vec3d(_verts[xnum-1].x, 0, _verts[xnum-1].z));
    for (int i = 1; i < xnum; i++)
    {
        _verts.push_back(Vec3d(_verts[i*znum + xnum-1].x, 0, _verts[i*znum + xnum-1].z));

        v[0][2] = v[0][0] = (i-1)*znum + xnum-1;
        v[1][2] = v[1][0] = i*znum + xnum-1;
        v[2][2] = v[2][0] = _verts.size()-2;
        _faces.push_back(v);

        v[0][2] = v[0][0] = i*znum + xnum-1;
        v[1][2] = v[1][0] = _verts.size()-1;
        v[2][2] = v[2][0] = _verts.size()-2;
        _faces.push_back(v);
    }

    _verts.push_back(Vec3d(_verts[0].x, 0, _verts[0].z));
    for (int i = 1; i < xnum; i++)
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

    _verts.push_back(Vec3d(_verts[(znum-1)*xnum].x, 0, _verts[(znum-1)*xnum].z));
    for (int i = 1; i < xnum; i++)
    {
        _verts.push_back(Vec3d(_verts[(znum-1)*xnum + i].x, 0, _verts[(znum-1)*xnum + i].z));

        v[2][2] = v[2][0] = (znum-1)*xnum + i-1;
        v[1][2] = v[1][0] = (znum-1)*xnum + i;
        v[0][2] = v[0][0] = _verts.size()-2;
        _faces.push_back(v);

        v[2][2] = v[2][0] = (znum-1)*xnum + i;
        v[1][2] = v[1][0] = _verts.size()-1;
        v[0][2] = v[0][0] = _verts.size()-2;
        _faces.push_back(v);
    }


    _norms.resize(_verts.size());                                                            // vect[0] - номер вершины в векторе вершин

    return this;
}

void Land::recalculateNormals()
{
    for (auto norm : _norms)
        norm[0] = norm[1] = norm[2] = 0;

  /*  for (int i = 0; i < _faces.size(); i++)
    {
        _facenorms[i] = cross(_verts[_faces[i][1][0]] - _verts[_faces[i][0][0]], _verts[_faces[i][2][0]] - _verts[_faces[i][0][0]]);
    }

    for (int i = 0; i < _faces.size(); i++)
        for (int j = 0; j < 3; j++)
            _norms[_faces[i][j][2]] +=  _facenorms[i];*/

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
