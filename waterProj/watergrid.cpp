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

    curGrid[9][12].y = 0.6;
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
    prevGrid[12][13].y = 0.6;

   /* for (int i = 1; i < xnum+1; i++)
        for (int j = 1; j < znum+1; j++)
            curGrid[i][j].y *= 2*exp(-5*(pow(dx*(i-20), 2) + pow(dz*(j-20), 2))) + 1;*/

    return this;
}

void WaterGrid::recalculateNormals()
{
    for (auto norm : _norms)
        norm[0] = norm[1] = norm[2] = 0;

    for (auto face : _faces)
    {
        for (auto vert : face)
            _norms[vert[2]] = _norms[vert[2]] + _verts[vert[0]];
    }
}


const double relax_param = 1.985;

void WaterGrid::Solve()
{
    for (int i = 1; i < _xnum + 1; i++)
        for (int j = 1; j < _znum + 1; j++)
            nextGrid[i][j].y = (1 - relax_param)*prevGrid[i][j].y +
                    relax_param*(curGrid[i][j+1].y + curGrid[i][j-1].y + curGrid[i+1][j].y + curGrid[i-1][j].y)/4;

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
