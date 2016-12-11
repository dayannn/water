#include "carcasmodelpainter.h"
#include "math.h"
#include <QDebug>
#include "matrix.h"

CarcasModelPainter::CarcasModelPainter() {}

CarcasModelPainter::~CarcasModelPainter() {}

void CarcasModelPainter::draw(BaseCanvas *canvas, BaseObject *object, Camera *camera)
{
    Model* model = static_cast<Model*>(object);

    QColor modelColor = model->getColor();

    PositionInfo posInfo = model->getPositionInfo();
    double a = posInfo.alpha;
    double b = posInfo.beta;
    double c = posInfo.gamma;

    Matrix transformMatr = Matrix::identity();

    Matrix matr = Matrix::rotateX(a);
    //transformMatr = matr * transformMatr;
    transformMatr.multLeft(matr);

    matr = Matrix::rotateY(b);
    //transformMatr = matr * transformMatr;
    transformMatr.multLeft(matr);

    matr = Matrix::rotateZ(c);
    //transformMatr = matr * transformMatr;
    transformMatr.multLeft(matr);

    a = posInfo.x;
    b = posInfo.y;
    c = posInfo.z;

    matr = Matrix::move(a, b, c);
    //transformMatr = matr * transformMatr;
    transformMatr.multLeft(matr);

    a = -camera->getCenter().x;
    b = -camera->getCenter().y;
    c = -camera->getCenter().z;

    matr = Matrix::move(a, b, c);
    //transformMatr = matr * transformMatr;
    transformMatr.multLeft(matr);

    b = camera->getBeta();
    matr = Matrix::rotateY(b);
    //transformMatr = matr * transformMatr;
    transformMatr.multLeft(matr);

    a = camera->getAlpha();
    matr = Matrix::rotateX(a);
    //transformMatr = matr * transformMatr;
    transformMatr.multLeft(matr);

    Matrix projection = Matrix::identity();
    Matrix Viewport = Matrix::viewport(canvas->width()/8, canvas->height()/8, canvas->width()*3/4, canvas->height()*3/4);

    Vec3d cam(0,0,1);
    projection[3][2] = -1.0/cam.z;

    //transformMatr = Viewport * projection * transformMatr;
    transformMatr.multLeft(projection);
    transformMatr.multLeft(Viewport);

    #pragma omp parallel for
    for (unsigned i = 0; i < model->getFacesNum(); i++)
    {
    vector<Vec3i>& face = model->face(i);
        /* Vec3d world_coords[3];
        for (int i = 0; i < 3; i++)
        {
            world_coords[i] = model->vertice(face[i][0]);
        }
        Vec3d n = (world_coords[2] - world_coords[0])^(world_coords[1]-world_coords[0]);
        Vec3d camdir (cos(camera->getAlpha())*sin(camera->getBeta()), -sin(camera->getAlpha()), -cos(camera->getAlpha())*cos(camera->getBeta()));
        double visibility = n*camdir;
        if (visibility > 0) */

        for (int i = 0; i < 3; i++)
        {
            Vec3d& v0 = model->vertice(face[i][0]);
            Vec3d& v1 = model->vertice(face[(i+1)%3][0]);

            Vec3i vscr0 = proj3d(transformMatr*embed<4>(v0));
            Vec3i vscr1 = proj3d(transformMatr*embed<4>(v1));

            if (vscr0.z >= 0 && vscr1.z >= 0)
                canvas->drawEdge(vscr0.x, vscr0.y, vscr1.x, vscr1.y, modelColor);
        }
    }

}

void CarcasModelPainter::setLightPoint(Vec3d light)
{}

void CarcasModelPainter::setAmbLight(QColor *clr)
{}

void CarcasModelPainter::setDiffLight(QColor *clr)
{}

void CarcasModelPainter::setSpecLight(QColor *clr)
{}
