#include "watergrid.h"

WaterGrid::WaterGrid()
{
        // IMPLEMENT THIS!
}

WaterGrid::~WaterGrid()
{
        // IMPLEMENT THIS!
}

void free_grid_matrix_rows (Vec3d **p, int strs)
{
    for (int i = 0; i < strs; i++)
        delete p[i];
    delete p;
}

Vec3d** allocate_grid_matrix_rows (int strs, int cols)
{
    Vec3d **p = new Vec3d* [strs];
    if (!p)
        return NULL;
    for (int i = 0; i < strs; i++)
    {
        p[i] = new Vec3d[cols];
        if (!p[i])
        {
            free_grid_matrix_rows (p, strs);
            return NULL;
        }
    }
    return p;
}

BaseObject *WaterGrid::createGrid(double xn, double zn, double xlen, double zlen, double ylevel, int xnum, int znum)
{
    _xnum = xnum;
    _znum = znum;

    double dx = xlen/xnum;
    double dz = zlen/znum;

    _dx = dx; // neccesary?
    _dz = dz;

    for (int i = 0; i < xnum; i++)
        for (int j = 0; j < znum; j++)
        {
            _verts.push_back(Vec3d(xn + dx*i, ylevel, zn + dz*j));
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


    _norms.resize(_verts.size());

    for (auto norm : _norms)
        norm[0] = norm[1] = norm[2] = 0;

    for (auto face : _faces)
    {
        for (auto vert : face)
            _norms[vert[2]] = _norms[vert[2]] + _verts[vert[0]];        // vert[2] - номер нормали в векторе нормалей
    }                                                                   // vect[0] - номер вершины в векторе вершин

    prevGrid = allocate_grid_matrix_rows(xnum+2, znum+2);   // на 2 больше, т.к. по краям находятся граничные условия
    curGrid = allocate_grid_matrix_rows(xnum+2, znum+2);
    nextGrid = allocate_grid_matrix_rows(xnum+2, znum+2);

    for (int i = 0; i < xnum+2; i++)
        for (int j = 0; j < znum+2; j++)
            prevGrid[i][j].y = curGrid[i][j].y = nextGrid[i][j].y = ylevel;

   /* curGrid[9][12].y = 0.6;
    curGrid[9][13].y = 0.6;
    curGrid[10][11].y = 0.6;
    curGrid[10][12].y = 0.4;
    curGrid[10][13].y = 0.4;
    curGrid[10][14].y = 0.6;
    curGrid[11][11].y = 0.6;
    curGrid[11][12].y = 0.4;
    curGrid[11][13].y = 0.4;
    curGrid[11][14].y = 0.6;
    curGrid[12][12].y = 0.6;
    curGrid[12][13].y = 0.6;

    prevGrid[9][12].y = 0.6;
    prevGrid[9][13].y = 0.6;
    prevGrid[10][11].y = 0.6;
    prevGrid[10][12].y = 0.4;
    prevGrid[10][13].y = 0.4;
    prevGrid[10][14].y = 0.6;
    prevGrid[11][11].y = 0.6;
    prevGrid[11][12].y = 0.4;
    prevGrid[11][13].y = 0.4;
    prevGrid[11][14].y = 0.6;
    prevGrid[12][12].y = 0.6;
    prevGrid[12][13].y = 0.6;*/

   /* for (int i = 1; i < xnum+1; i++)
        for (int j = 1; j < znum+1; j++)
            curGrid[i][j].y *= 2*exp(-5*(pow(dx*(i-20), 2) + pow(dz*(j-20), 2))) + 1;*/

    return this;
}

void WaterGrid::recalculateNormals()
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


const double relax_param = 1.959;
const double wspeed = 50;

void WaterGrid::Solve()
{
    milliseconds curTime = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    milliseconds diff = duration_cast<milliseconds>(curTime - prevTime);
    prevTime = curTime;
    long dt = diff.count();


    #pragma omp parallel for
    for (int i = 1; i < _xnum + 1; i++)
        for (int j = 1; j < _znum + 1; j++)
            nextGrid[i][j].y = (1 - relax_param)*prevGrid[i][j].y +
                    relax_param*(curGrid[i][j+1].y + curGrid[i][j-1].y + curGrid[i+1][j].y + curGrid[i-1][j].y)/4;

  /*  for (int i = 1; i < _xnum + 1; i++)
        for (int j = 1; j < _znum + 1; j++)
            nextGrid[i][j].y = curGrid[i][j].y + (1-relax_param*0.5)*(curGrid[i][j].y - prevGrid[i][j].y)
                    + 0.5*0.5*0.25*0.25*(4*curGrid[i][j].y - curGrid[i][j+1].y - curGrid[i][j-1].y - curGrid[i+1][j].y - curGrid[i-1][j].y);
*/

    for (int i = 1; i < _xnum + 1; i++)     // граничные условия (отражение)
    {
        nextGrid[i][0].y = nextGrid[i][1].y;
        nextGrid[i][_znum+1].y = nextGrid[i][_znum].y;
    }
    for (int i = 1; i < _znum+1; i++)
    {
        nextGrid[0][i].y = nextGrid[1][i].y;
        nextGrid[_xnum+1][i].y = nextGrid[_xnum][i].y;
    }

    for (int i = 0; i < _xnum; i++)
        for (int j = 0; j < _znum; j++)
            _verts[i*_znum + j].y = nextGrid[i+1][j+1].y;

    std::swap(prevGrid, nextGrid);  // цикл. перестановка сеток  <-prev<-cur<-next<-
    std::swap(curGrid, prevGrid);

}

