#include "polygonalmodelpainter.h"
#include <omp.h>

PolygonalModelPainter::PolygonalModelPainter(){
    _light.point = Vec3d(200, 100, -70);
    _light.amb_r = 55;
    _light.amb_g = 55;
    _light.amb_b = 55;
    _light.diff_r = 248;
    _light.diff_g = 238;
    _light.diff_b = 228;
    _light.spec_r = 255;
    _light.spec_g = 238;
    _light.spec_b = 175;
}

PolygonalModelPainter::~PolygonalModelPainter(){}


void PolygonalModelPainter::draw(BaseCanvas *canvas, BaseObject *object, Camera *camera)
{
    Model* model = static_cast<Model*>(object);

    /*Vec3d eye(sin(camera->getAlpha()) + cos(camera->getBeta()), sin(camera->getBeta()), cos(camera->getAlpha()));
   // eye.normalize();
    Vec3d center(camera->getCenter().x , camera->getCenter().y, camera->getCenter().z);

    Matrix ModelView  = Matrix::lookat(eye, center, Vec3d(0,1,0));*/

    //Matrix ModelView = Matrix::lookat(Vec3d(0, -1, 3), Vec3d(0, 0, 0), Vec3d(0, 1, 0));

    PositionInfo &posInfo = model->getPositionInfo();
    double a = posInfo.alpha;
    double b = posInfo.beta;
    double c = posInfo.gamma;

    Matrix transformMatr = Matrix::identity();

    Matrix matr;
    if (a != 0)
    {
        matr = Matrix::rotateX(a);
        transformMatr.multLeft(matr);
    }

    if (b != 0)
    {
        matr = Matrix::rotateY(b);
        transformMatr.multLeft(matr);
    }

    if (c != 0)
    {
        matr = Matrix::rotateZ(c);
        transformMatr.multLeft(matr);
    }

    Matrix modelRotMatr = transformMatr;

    a = posInfo.x;
    b = posInfo.y;
    c = posInfo.z;

    if (a || b || c)
    {
        matr = Matrix::move(a, b, c);
        transformMatr.multLeft(matr);
    }

    a = -camera->getCenter().x;
    b = -camera->getCenter().y;
    c = -camera->getCenter().z;

    matr = Matrix::move(a, b, c);
    transformMatr.multLeft(matr);

    b = camera->getBeta();
    matr = Matrix::rotateY(b);
    transformMatr.multLeft(matr);

    a = camera->getAlpha();
    matr = Matrix::rotateX(a);
    transformMatr.multLeft(matr);

    Matrix projection = Matrix::identity();
    Matrix Viewport = Matrix::viewport(canvas->width()/8, canvas->height()/8, canvas->width()*3/4, canvas->height()*3/4);

    Vec3d cam(0,0,1);
    projection[3][2] = -1.0/cam.z;

    transformMatr.multLeft(projection);
    transformMatr.multLeft(Viewport);

    //Vec3d light = camera->getCenter(); // camera is a light source

    Vec3d cam_pos = camera->getCenter();

    #pragma omp parallel for
    for (unsigned i = 0; i < model->getFacesNum(); i++)
    {
        vector<Vec3i>& face = model->face(i);
        Vec3d screen_coords[3];
        Vec3d coords[3];
        Vec3d normals[3];
       // Vec3d world_coords[3];

        for (int j = 0; j < 3; j++)
        {
            Vec3d& v = model->vertice(face[j][0]);
            screen_coords[j] = proj3d(transformMatr*embed<4>(v));
          // world_coords[j] = v;
            normals[j] = proj3d(modelRotMatr * embed<4>(model->norm(i, j)));
            coords[j] = proj3d(modelRotMatr * embed<4>(v));
        }

        Vec3d n = cross(coords[1]-coords[0], coords[2] - coords[0]);
        Vec3d camdir = ((coords[0] + coords[1] + coords[2])/3 - cam_pos);
        double visibility = n*camdir;


        if ((visibility >= 0 || fabs(model->get_koefs()->transparency - 1) < 1e-3) &&
                ((screen_coords[0][0] > 0 && screen_coords[0][1] > 0 && screen_coords[0][2] > 0) ||
                (screen_coords[1][0] > 0 && screen_coords[1][1] > 0 && screen_coords[1][2] > 0) ||
                (screen_coords[2][0] > 0 && screen_coords[2][1] > 0 && screen_coords[2][2] > 0)))
            canvas->fillTriangle(screen_coords, coords, normals, &_light, cam_pos, model->get_koefs());
    }

}

void PolygonalModelPainter::setLightPoint(Vec3d light)
{
    _light.point = light;
}

void PolygonalModelPainter::setAmbLight(QColor *clr)
{
    _light.amb_r = clr->red();
    _light.amb_g = clr->green();
    _light.amb_b = clr->blue();
}

void PolygonalModelPainter::setDiffLight(QColor *clr)
{
    _light.diff_r = clr->red();
    _light.diff_g = clr->green();
    _light.diff_b = clr->blue();
}

void PolygonalModelPainter::setSpecLight(QColor *clr)
{
    _light.spec_r = clr->red();
    _light.spec_g = clr->green();
    _light.spec_b = clr->blue();
}
