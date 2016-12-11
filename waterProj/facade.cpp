#include "facade.h"
#include <QFileDialog>

Facade::Facade(PaintWidget *paintWigdet)
{
    canvasInfo.paintWidget = paintWigdet;
    drawManager = new DrawManager(&canvasInfo);
    loadManager = new LoadManager(new Stream(new FileStreamImp));
    sceneManager = new SceneManager(new Scene);
    transformManager = new TransformManager(new Transformer);

}

Facade::~Facade()
{
    delete drawManager;
    delete loadManager;
    delete sceneManager;
    delete transformManager;
}

void Facade::processStream(StreamInfo *streamInfo)
{
    switch(streamInfo->sourceType)
    {
        case SOURCE_FILE:
            if(streamInfo->sourceName == NULL)
            {
               // streamInfo->sourceName = alertWindowService->selectFile().c_str();;
                *streamInfo->sourceName = QFileDialog::getOpenFileName(0, "Открыть файл", ".").toStdString();
            }
            break;
        default:
            throw EmptySourceStreamException();
            break;
    }
}

void Facade::loadModelFromFile(StreamInfo info)
{
    try
    {
        //processStream(&info);

        sceneManager->addObject(loadManager->loadObject(&info));

        drawManager->drawScene(sceneManager->getScene(), sceneManager->currentCamera());

    }
    catch(BaseException& exc)
    {
       // qDebug() << exc.what();
       // this->alertWindowService->showErrorMessage(exc.what());
    }
}



void Facade::loadCameraFromFile(StreamInfo info)
{
    try
    {
        processStream(&info);

        sceneManager->addCamera(loadManager->loadCamera(&info));



        WaterGrid *grid = new WaterGrid;
        waterGrid = grid;
        sceneManager->addObject(grid->createGrid(0, 0, 50, 50, 5, 50, 50));
        grid->Solve();
        grid->recalculateNormals();

        Land *land = new Land;
        landGrid = land;
        land->createGrid();
        land->recalculateNormals();
        sceneManager->addObject(land);

        StreamInfo boatInfo;
        std::string* boatFile = new std::string;
        *boatFile = "boat.obj";
        boatInfo.sourceName = boatFile;
        boatInfo.sourceType = SOURCE_FILE;
        boat = (Model*) loadManager->loadObject(&boatInfo);
        boat->setColor(QColor(178, 205, 214));
        boat->setKoefsFromColor(boat->get_koefs(), boat->getColor());

        boat->changeVertsOrder();
        boat->remakeNormals();      // do smth with it!
        sceneManager->addObject(boat);
        delete boatFile;

        TransformInfo boatTransform;
        boatTransform.type = TRANSFORM_MOVE_Y;
        boatTransform.delta = 5.6;
        transformManager->transformModel(boat, &boatTransform);

        boatTransform.type = TRANSFORM_MOVE_X;
        boatTransform.delta = 10;
        transformManager->transformModel(boat, &boatTransform);

        boatTransform.type = TRANSFORM_MOVE_Z;
        boatTransform.delta = 10;
        transformManager->transformModel(boat, &boatTransform);

        drawManager->drawScene(sceneManager->getScene(), sceneManager->currentCamera());

        QTimer::singleShot(200, this, SLOT(updateWaterGrid()));
    }
    catch(BaseException& exc)
    {
     //   this->alertWindowService->showErrorMessage(exc.what());
    }
}

void Facade::transformModel(TransformInfo info)
{
    transformManager->transformModel(*sceneManager->currentObject(), &info);

    drawManager->drawScene(sceneManager->getScene(), sceneManager->currentCamera());
}

void Facade::transformCamera(TransformInfo info)
{
    transformManager->transformCamera(*sceneManager->currentCamera(), &info);

    drawManager->drawScene(sceneManager->getScene(), sceneManager->currentCamera());
}

void Facade::drawScene()
{
    drawManager->drawScene(sceneManager->getScene(), sceneManager->currentCamera());
}

void Facade::changeModelType(eModelType type)
{
    drawManager->changeModelType(type);

    drawManager->drawScene(sceneManager->getScene(), sceneManager->currentCamera());
}

void Facade::updateWaterGrid()
{
    static  int r = 0;
    r++;
    static double koef = 0.1;
    koef += 0.01;
    if (koef > 0.3)
        koef = 0.1;
    for (int i = 0 ; i < waterGrid->_xnum; i++)
    {
        double rnd = -0.1 + (double)rand() / RAND_MAX / 5;
        waterGrid->curGrid[i][0].y += koef * sin(r/4) + rnd;
    }

    waterGrid->Solve();
    waterGrid->recalculateNormals();
    drawManager->drawScene(sceneManager->getScene(), sceneManager->currentCamera());

    QTimer::singleShot(30, this, SLOT(updateWaterGrid()));
}

void Facade::moveBoat(double dl, double dalpha)
{
    TransformInfo boatTransform;
    boatTransform.type = TRANSFORM_ROTATE_Y;
    boatTransform.delta = dalpha;
    transformManager->transformModel(boat, &boatTransform);

    double beta = boat->getPositionInfo().beta;
    double dx = dl * cos(beta);
    double dz = -dl * sin(beta);

    double new_x = boat->getPositionInfo().x + dx;
    double new_z = boat->getPositionInfo().z + dz;

    if (5.0 - landGrid->getHeight(new_x, new_z) > 10e-5)
    {

        boatTransform.type = TRANSFORM_MOVE_X;
        boatTransform.delta = dx;
        transformManager->transformModel(boat, &boatTransform);

        boatTransform.type = TRANSFORM_MOVE_Z;
        boatTransform.delta = dz;
        transformManager->transformModel(boat, &boatTransform);

        for (int i = 1; i < waterGrid->_xnum+1; i++)
            for (int j = 1; j < waterGrid->_znum+1; j++)
            {
               waterGrid->curGrid[i][j].y += -exp(-(pow(i*waterGrid->_dx - (new_x - (dl >= 0 ? 4*cos(beta) : -4*cos(beta))), 2)
                                                    + pow(j*waterGrid->_dz - (new_z + (dl >= 0 ? 4*sin(beta) : -4*sin(beta))), 2)))/7.5;

               waterGrid->curGrid[i][j].y += exp(-(pow(i*waterGrid->_dx - (new_x + (dl >= 0 ? 6*cos(beta) : -6*cos(beta))), 2)
                                                        + pow(j*waterGrid->_dz - (new_z - (dl >= 0 ? 6*sin(beta) : -6*sin(beta))), 2)))/7.5;


                // additional values
               /* waterGrid->curGrid[i][j].y += exp(-(pow(i*waterGrid->_dx - (x - (dl >= 0 ? 8*cos(beta+0.3) : -8*cos(beta+0.3))), 2)
                                                                + pow(j*waterGrid->_dz - (z + (dl >= 0 ? 8*sin(beta+0.3) : -8*sin(beta+0.3))), 2)))/7.5;
                waterGrid->curGrid[i][j].y += -exp(-(pow(i*waterGrid->_dx - (x - (dl >= 0 ? 5*cos(beta) : -5*cos(beta))), 2)
                                                                + pow(j*waterGrid->_dz - (z + (dl >= 0 ? 5*sin(beta) : -5*sin(beta))), 2)))/2.5;
                waterGrid->curGrid[i][j].y += exp(-(pow(i*waterGrid->_dx - (x - (dl >= 0 ? 8*cos(beta-0.3) : -8*cos(beta-0.3))), 2)
                                                                + pow(j*waterGrid->_dz - (z + (dl >= 0 ? 8*sin(beta-0.3) : -8*sin(beta-0.3))), 2)))/7.5;
            */
            }
    }
}

void Facade::setLightPoint(Vec3d light)
{
    drawManager->setLightPoint(light);
}

void Facade::setAmbLight(QColor *clr)
{
    drawManager->setAmbLight(clr);
}

void Facade::setDiffLight(QColor *clr)
{
    drawManager->setDiffLight(clr);
}

void Facade::setSpecLight(QColor *clr)
{
    drawManager->setSpecLight(clr);
}
