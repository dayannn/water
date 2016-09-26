#ifndef FACADE_H
#define FACADE_H

#include "drawmanager.h"
#include "loadmanager.h"
#include "scenemanager.h"
#include "transformmanager.h"
#include "stream.h"
#include "scene.h"
#include "watergrid.h"

#include <QObject>
#include <QTimer>

class Facade : public QObject
{
    Q_OBJECT

public:
    explicit Facade(PaintWidget *paintWigdet);
    ~Facade();

    void transformModel(TransformInfo info);
    void transformCamera(TransformInfo info);

    void loadModelFromFile(StreamInfo info  = {SOURCE_UNDEFINED, NULL});
    void loadCameraFromFile(StreamInfo info  = {SOURCE_UNDEFINED, NULL});

    void changeModelType(eModelType type);

    void drawScene();

    void moveBoat(double dl, double dalpha);

public slots:
    void updateWaterGrid();

private:
    DrawManager *drawManager;
    LoadManager *loadManager;
    SceneManager *sceneManager;
    TransformManager *transformManager;
    CanvasInfo canvasInfo;

    WaterGrid *waterGrid;
    Model *boat;

    void processStream(StreamInfo*);
};

#endif // FACADE_H
