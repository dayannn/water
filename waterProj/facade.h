#ifndef FACADE_H
#define FACADE_H

#include "drawmanager.h"
#include "loadmanager.h"
#include "scenemanager.h"
#include "transformmanager.h"
#include "stream.h"
#include "scene.h"
#include "watergrid.h"
#include "land.h"

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

    void setLightPoint(Vec3d light);
    void setAmbLight(QColor *clr);
    void setDiffLight(QColor* clr);
    void setSpecLight(QColor *clr);

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
    Land * landGrid;

    void processStream(StreamInfo*);
};

#endif // FACADE_H
