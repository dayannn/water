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

        drawManager->drawScene(sceneManager->getScene(), sceneManager->currentCamera());

        WaterGrid *grid = new WaterGrid;
        waterGrid = grid;
        grid->setColor(QColor(32, 178, 201));

        sceneManager->addObject(grid->createGrid(0, 0, 50, 50, 3, 60, 60));
        grid->Solve();
        grid->recalculateNormals();


        StreamInfo boatInfo;
        std::string* boatFile = new std::string;
        *boatFile = "boat.obj";
        boatInfo.sourceName = boatFile;
        boatInfo.sourceType = SOURCE_FILE;
        boat = (Model*) loadManager->loadObject(&boatInfo);
        boat->setColor(QColor(168, 101, 64));
        boat->remakeNormals();      // do smth with it!
        sceneManager->addObject(boat);
        delete boatFile;

        TransformInfo boatTransform;
        boatTransform.type = TRANSFORM_MOVE_Y;
        boatTransform.delta = 3;
        transformManager->transformModel(boat, &boatTransform);

        boatTransform.type = TRANSFORM_MOVE_X;
        boatTransform.delta = 10;
        transformManager->transformModel(boat, &boatTransform);

        boatTransform.type = TRANSFORM_MOVE_Z;
        boatTransform.delta = 10;
        transformManager->transformModel(boat, &boatTransform);

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

int frame = 0;

void Facade::updateWaterGrid()
{

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

    boatTransform.type = TRANSFORM_MOVE_X;
    boatTransform.delta = dx;
    transformManager->transformModel(boat, &boatTransform);

    boatTransform.type = TRANSFORM_MOVE_Z;
    boatTransform.delta = dz;
    transformManager->transformModel(boat, &boatTransform);

    double x = boat->getPositionInfo().x;
    double z = boat->getPositionInfo().z;

    for (int i = 1; i < waterGrid->_xnum+1; i++)
        for (int j = 1; j < waterGrid->_znum+1; j++)
        {
           waterGrid->curGrid[i][j].y += -exp(-(pow(i*waterGrid->_dx - (x - (dl >= 0 ? 4*cos(beta) : -4*cos(beta))), 2)
                                                + pow(j*waterGrid->_dz - (z + (dl >= 0 ? 4*sin(beta) : -4*sin(beta))), 2)))/5;

           waterGrid->curGrid[i][j].y += exp(-(pow(i*waterGrid->_dx - (x + (dl >= 0 ? 6*cos(beta) : -6*cos(beta))), 2)
                                                    + pow(j*waterGrid->_dz - (z - (dl >= 0 ? 6*sin(beta) : -6*sin(beta))), 2)))/5;


           /* waterGrid->curGrid[i][j].y += exp(-(pow(i*waterGrid->_dx - (x - (dl >= 0 ? 8*cos(beta+0.3) : -8*cos(beta+0.3))), 2)
                                                            + pow(j*waterGrid->_dz - (z + (dl >= 0 ? 8*sin(beta+0.3) : -8*sin(beta+0.3))), 2)))/7.5;
            waterGrid->curGrid[i][j].y += -exp(-(pow(i*waterGrid->_dx - (x - (dl >= 0 ? 5*cos(beta) : -5*cos(beta))), 2)
                                                            + pow(j*waterGrid->_dz - (z + (dl >= 0 ? 5*sin(beta) : -5*sin(beta))), 2)))/2.5;
            waterGrid->curGrid[i][j].y += exp(-(pow(i*waterGrid->_dx - (x - (dl >= 0 ? 8*cos(beta-0.3) : -8*cos(beta-0.3))), 2)
                                                            + pow(j*waterGrid->_dz - (z + (dl >= 0 ? 8*sin(beta-0.3) : -8*sin(beta-0.3))), 2)))/7.5;
        */
        }

}
