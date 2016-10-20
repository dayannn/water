#include "polygonalmodelpainter.h"

PolygonalModelPainter::PolygonalModelPainter(){}

PolygonalModelPainter::~PolygonalModelPainter(){}

Matrix lookat(Vec3d eye, Vec3d center, Vec3d up) {
    Vec3d z = (eye-center).normalize();
    Vec3d x = (up^z).normalize();
    Vec3d y = (z^x).normalize();
    Matrix res = Matrix::identity();
    for (int i=0; i<3; i++) {
        res[0][i] = x[i];
        res[1][i] = y[i];
        res[2][i] = z[i];
        res[i][3] = -center[i];
    }
    return res;
}

void PolygonalModelPainter::draw(BaseCanvas *canvas, BaseObject *object, Camera *camera)
{
    Model* model = static_cast<Model*>(object);

    QColor modelColor = model->getColor();

    /*Vec3d eye(sin(camera->getAlpha()) + cos(camera->getBeta()), sin(camera->getBeta()), cos(camera->getAlpha()));
   // eye.normalize();
    Vec3d center(camera->getCenter().x , camera->getCenter().y, camera->getCenter().z);

    Matrix ModelView  = lookat(eye, center, Vec3d(0,1,0));*/


    PositionInfo &posInfo = model->getPositionInfo();
    double a = posInfo.alpha;
    double b = posInfo.beta;
    double c = posInfo.gamma;

    Matrix transformMatr = Matrix::identity();

    Matrix matr;
    if (a != 0)
    {
        matr = Matrix::rotateX(a);
        //transformMatr = matr * transformMatr;
        transformMatr.multLeft(matr);
    }

    if (b != 0)
    {
        matr = Matrix::rotateY(b);
        //transformMatr = matr * transformMatr;
        transformMatr.multLeft(matr);
    }

    if (c != 0)
    {
        matr = Matrix::rotateZ(c);
        //transformMatr = matr * transformMatr;
        transformMatr.multLeft(matr);
    }

    Matrix modelRotMatr = transformMatr;

    a = posInfo.x;
    b = posInfo.y;
    c = posInfo.z;

    if (a || b || c)
    {
        matr = Matrix::move(a, b, c);
        //transformMatr = matr * transformMatr;
        transformMatr.multLeft(matr);
    }

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
    Matrix Viewport = viewport(canvas->width()/8, canvas->height()/8, canvas->width()*3/4, canvas->height()*3/4);

    Vec3d cam(0,0,1);
    projection[3][2] = -0.9/cam.z;

    //transformMatr = Viewport * projection * transformMatr;
    transformMatr.multLeft(projection);
    transformMatr.multLeft(Viewport);

    Vec3d light_dir = {0.1, 1, 0.3};

    //uncomment below to make camera a light sourse
    //Vec3d light_dir = {cos(camera->getAlpha())*sin(camera->getBeta()), -sin(camera->getAlpha()), -cos(camera->getAlpha())*cos(camera->getBeta())};
    light_dir.normalize();

    for (unsigned i = 0; i < model->getFacesNum(); i++)
    {
        vector<Vec3i>& face = model->face(i);
        Vec3i screen_coords[3];
        //Vec3d world_coords[3];
        double intensity[3];

        for (int j = 0; j < 3; j++)
        {
            Vec3d& v = model->vertice(face[j][0]);
            screen_coords[j] = Vec3d(transformMatr*Matrix(v));
            //world_coords[j] = v;
            intensity[j] = Vec3d(modelRotMatr * Matrix(model->norm(i, j)))*light_dir;
        }

  /*      Vec3d n = (world_coords[2] - world_coords[0])^(world_coords[1]-world_coords[0]);
      //  Vec3d camdir (sin(camera->getBeta()), -cos(camera->getBeta())*sin(camera->getAlpha()), -cos(camera->getBeta())*cos(camera->getAlpha()));
       // Vec3d camdir (sin(camera->getBeta())*cos(camera->getAlpha()), sin(camera->getBeta())*sin(camera->getAlpha()), -cos(camera->getBeta()));
        Vec3d camdir (cos(camera->getAlpha())*sin(camera->getBeta()), -sin(camera->getAlpha()), -cos(camera->getAlpha())*cos(camera->getBeta()));
        double visibility = n*camdir;

        if (visibility >= 0)*/
            canvas->fillTriangle(screen_coords[0], screen_coords[1], screen_coords[2], intensity[0], intensity[1], intensity[2], modelColor);
    }
}
