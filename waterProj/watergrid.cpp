#include "watergrid.h"

WaterGrid::WaterGrid()
{
        // IMPLEMENT THIS!
}

WaterGrid::~WaterGrid()
{
        // IMPLEMENT THIS!
}

void free_Vec3d_matrix (Vec3d **p, int strs)
{
    for (int i = 0; i < strs; i++)
        delete p[i];
    delete p;
}

Vec3d** allocate_Vec3d_matrix (int strs, int cols)
{
    Vec3d **p = new Vec3d* [strs];
    if (!p)
        return NULL;
    for (int i = 0; i < strs; i++)
    {
        p[i] = new Vec3d[cols];
        if (!p[i])
        {
            free_Vec3d_matrix (p, strs);
            return NULL;
        }
    }
    return p;
}

void free_double_matrix (double **p, int strs)
{
    for (int i = 0; i < strs; i++)
        delete p[i];
    delete p;
}

double** allocate_double_matrix (int strs, int cols)
{
    double **p = new double* [strs];
    if (!p)
        return NULL;
    for (int i = 0; i < strs; i++)
    {
        p[i] = new double[cols];
        if (!p[i])
        {
            free_double_matrix (p, strs);
            return NULL;
        }
    }
    return p;
}

const double grav = 9.8;

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

    prevGrid = allocate_Vec3d_matrix(xnum+2, znum+2);   // на 2 больше, т.к. по краям находятся граничные условия
    curGrid = allocate_Vec3d_matrix(xnum+2, znum+2);
    nextGrid = allocate_Vec3d_matrix(xnum+2, znum+2);

    /**********************************/
    Hcur = allocate_Vec3d_matrix(xnum+2, znum +2);
    HhalfHor = allocate_Vec3d_matrix(xnum, xnum+1);
    HhalfVert = allocate_Vec3d_matrix(xnum+1, xnum);
    Hnew = allocate_Vec3d_matrix(xnum+2, xnum+2);
    Ucur = allocate_Vec3d_matrix(xnum+2, znum +2);
    UhalfHor = allocate_Vec3d_matrix(xnum, xnum+1);
    UhalfVert = allocate_Vec3d_matrix(xnum+1, xnum);
    Unew = allocate_Vec3d_matrix(xnum+2, xnum+2);
    Vcur = allocate_Vec3d_matrix(xnum+2, znum +2);
    VhalfHor = allocate_Vec3d_matrix(xnum, xnum+1);
    VhalfVert = allocate_Vec3d_matrix(xnum+1, xnum);
    Vnew = allocate_Vec3d_matrix(xnum+2, xnum+2);


    for (int i = 0; i < xnum+2; i++)
        for (int j = 0; j < znum+2; j++)
        {
            Hcur[i][j].x = ylevel;
            Hcur[i][j].y = Hcur[i][j].x = Ucur[i][j].x = Ucur[i][j].z = Vcur[i][j].x = Vcur[i][j].y = 0;
            Ucur[i][j].y = Vcur[i][j].z = ylevel*ylevel*grav/2;
        }
    for (int i = 1; i < _xnum + 1; i++)     // граничные условия (отражение)
    {
        Hcur[i][0] = Hcur[i][1];
        Hcur[i][_znum+1] = Hcur[i][_znum];
        Ucur[i][0] = Ucur[i][1];
        Ucur[i][_znum+1] = Ucur[i][_znum];
        Vcur[i][0] = Vcur[i][1] * (-1);
        Vcur[i][_znum+1] = Vcur[i][_znum] * (-1);

    }
    for (int i = 1; i < _znum+1; i++)
    {
        Hcur[0][i] = Hcur[1][i];
        Hcur[_xnum+1][i] = Hcur[_xnum][i];
        Ucur[0][i] = Ucur[1][i]* (-1);
        Ucur[_xnum+1][i] = Ucur[_xnum][i] * (-1);
        Vcur[0][i] = Vcur[1][i];
        Vcur[_xnum+1][i] = Vcur[_xnum][i];
    }

    /*****************************************/

    for (int i = 0; i < xnum+2; i++)
        for (int j = 0; j < znum+2; j++)
            prevGrid[i][j].y = curGrid[i][j].y = nextGrid[i][j].y = ylevel;

    this->setColor(QColor(55, 139, 195));
    this->setKoefsFromColor(this->get_koefs(), this->getColor());
    LightKoefs* tmpKoefs = this->get_koefs();
    tmpKoefs->transparency = 0.75;
    tmpKoefs->shininess = 32;
    tmpKoefs->spec_r = 0.9;
    tmpKoefs->spec_g = 0.9;
    tmpKoefs->spec_b = 0.9;
    tmpKoefs->diff_r /= 1.4;
    tmpKoefs->diff_g /= 1.4;
    tmpKoefs->diff_b /= 1.4;
   /* tmpKoefs->amb_r /= 2;
    tmpKoefs->amb_g /= 2;
    tmpKoefs->amb_b /= 2;*/

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


const double relax_param = 1.958;
const double wspeed = 50;

void WaterGrid::Solve()
{
    //milliseconds curTime = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    //milliseconds diff = duration_cast<milliseconds>(curTime - prevTime);
    //prevTime = curTime;
    //long dt = diff.count();


    #pragma omp parallel for
    for (int i = 1; i < _xnum + 1; i++)
        for (int j = 1; j < _znum + 1; j++)
            nextGrid[i][j].y = (1 - relax_param)*prevGrid[i][j].y +
                    relax_param*(curGrid[i][j+1].y + curGrid[i][j-1].y + curGrid[i+1][j].y + curGrid[i-1][j].y)/4;

  //  for (int i = 1; i < _xnum + 1; i++)
  //      for (int j = 1; j < _znum + 1; j++)
  //          nextGrid[i][j].y = curGrid[i][j].y + (1-relax_param*0.5)*(curGrid[i][j].y - prevGrid[i][j].y)
  //                  + 0.5*0.5*0.25*0.25*(4*curGrid[i][j].y - curGrid[i][j+1].y - curGrid[i][j-1].y - curGrid[i+1][j].y - curGrid[i-1][j].y);


    #pragma omp parallel for
    for (int i = 1; i < _xnum + 1; i++)     // граничные условия (отражение)
    {
        nextGrid[i][0].y = nextGrid[i][1].y;
        nextGrid[i][_znum+1].y = nextGrid[i][_znum].y;
    }
    #pragma omp parallel for
    for (int i = 1; i < _znum+1; i++)
    {
        nextGrid[0][i].y = nextGrid[1][i].y;
        nextGrid[_xnum+1][i].y = nextGrid[_xnum][i].y;
    }
    #pragma omp parallel for
    for (int i = 0; i < _xnum; i++)
        for (int j = 0; j < _znum; j++)
            _verts[i*_znum + j].y = nextGrid[i+1][j+1].y;

    std::swap(prevGrid, nextGrid);  // цикл. перестановка сеток  <-prev<-cur<-next<-
    std::swap(curGrid, prevGrid);

}

/*void WaterGrid::Solve()
{
    double dt = 0.001;

    for (int i = 0; i < _xnum+1; i++)
        for (int j = 0; j < _xnum; j++)
        {
            HhalfVert[i][j] = (Hcur[i+1][j+1] + Hcur[i][j+1])*0.5 - (Ucur[i+1][j+1] - Ucur[i][j+1])*(dt/(_dx+_dx));
            double u = HhalfVert[i][j].y/UhalfVert[i][j].x;
            double v = HhalfVert[i][j].z/HhalfVert[i][j].x;
            UhalfVert[i][j].x = HhalfVert[i][j].y;
            UhalfVert[i][j].y = UhalfVert[i][j].x*u + grav*HhalfVert[i][j].x*HhalfVert[i][j].x/2;
            UhalfVert[i][j].z = UhalfVert[i][j].x * v;
        }

    for (int i = 0; i < _xnum; i++)
        for (int j = 0; j < _xnum+1; j++)
        {
            HhalfHor[i][j] = (Hcur[i+1][j+1] + Hcur[i+1][j])*0.5 - (Vcur[i+1][j+1] - Vcur[i+1][j])*(dt/(_dx+_dx));
            double u = HhalfHor[i][j].y/UhalfHor[i][j].x;
            double v = HhalfHor[i][j].z/HhalfHor[i][j].x;
            VhalfHor[i][j].x = HhalfHor[i][j].z;
            VhalfHor[i][j].y = VhalfHor[i][j].x * u;
            VhalfHor[i][j].z = VhalfHor[i][j].x*v + grav*HhalfHor[i][j].x*HhalfHor[i][j].x/2;
        }

    for (int i = 0; i < _xnum; i++)
        for (int j = 0; j < _xnum; j++)
        {
            Hnew[i+1][j+1] = Hcur[i+1][j+1] - (UhalfVert[i+1][j] - UhalfVert[i][j])*(dt/_dx) - (VhalfHor[i][j+1] - VhalfHor[i][j])*(dt/_dx);
            double u = Hnew[i+1][j+1].y/Hnew[i+1][j+1].x;
            double v = Hnew[i+1][j+1].z/Hnew[i+1][j+1].x;
            Unew[i+1][j+1].x = Hnew[i+1][j+1].y;
            Unew[i+1][j+1].y = Unew[i+1][j+1].x*u + grav*Hnew[i+1][j+1].x*Hnew[i+1][j+1].x/2;
            Unew[i+1][j+1].z = Unew[i+1][j+1].x * v;
            Vnew[i+1][j+1].x = Hnew[i+1][j+1].z;
            Vnew[i+1][j+1].y = Vnew[i+1][j+1].x * u;
            Vnew[i+1][j+1].z = Vnew[i+1][j+1].x*v + grav*Hnew[i+1][j+1].x*Hnew[i+1][j+1].x/2;

            _verts[i*_xnum + j].y = Hnew[i+1][j+1].x;
        }
    for (int i = 1; i < _xnum + 1; i++)     // граничные условия (отражение)
    {
        Hnew[i][0] = Hnew[i][1];
        Hnew[i][_znum+1] = Hnew[i][_znum];
        Unew[i][0] = Unew[i][1];
        Unew[i][_znum+1] = Unew[i][_znum];
        Vnew[i][0] = Vnew[i][1] * (-1);
        Vnew[i][_znum+1] = Vnew[i][_znum] * (-1);

    }
    for (int i = 1; i < _znum+1; i++)
    {
        Hnew[0][i] = Hnew[1][i];
        Hnew[_xnum+1][i] = Hnew[_xnum][i];
        Unew[0][i] = Unew[1][i]* (-1);
        Unew[_xnum+1][i] = Unew[_xnum][i] * (-1);
        Vnew[0][i] = Vnew[1][i];
        Vnew[_xnum+1][i] = Vnew[_xnum][i];
    }
    std::swap(Hcur, Hnew);
    std::swap(Ucur, Unew);
    std::swap(Vcur, Vnew);
}
*/
