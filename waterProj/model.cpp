#include "model.h"
#include <QImageReader>

Model::Model()
{
    posInfo.x = posInfo.y = posInfo.z = 0;
    posInfo.alpha = posInfo.beta = posInfo.gamma = 0;
    posInfo.scale = 0;

    _koefs.amb_r = _koefs.amb_g = _koefs.amb_b = 0.5;
    _koefs.diff_r = _koefs.diff_g = _koefs.diff_b = 0.5;
    _koefs.spec_r = _koefs.spec_g = _koefs.spec_b = 0.5;

    _koefs.transparency = 1.0;
    _koefs.shininess = 4;
    _color = QColor(255, 255, 255);
}

Model::~Model()
{
    // is it necessary??
    for (auto it = _faces.begin(); it < _faces.end(); it++)
    {
        (*it).clear();
    }
    _faces.clear();
    _verts.clear();
}

BaseObject* Model::loadFromFile(const std::string *filename)
{
    std::ifstream f;
    f.open(*filename, std::ios_base::in);
    if (f.fail())
        return nullptr;     // EXCEPTION HERE!

    std::string line;

    while(!f.eof())
    {
        std::getline(f, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")){
            iss >> trash;
            Vec3d v;
            //for (int i = 0; i < 3; i++)
            //    iss >> v.raw[i];
            iss >> v.x;
            iss >> v.y;
            iss >> v.z;
            _verts.push_back(v);
        }
        else if (!line.compare(0, 3, "vn ")){
            iss >> trash >> trash;
            Vec3d n;
            for (int i = 0; i < 3; i++)
                iss >> n[i];
            _norms.push_back(n);
        }
        else if (!line.compare(0, 3, "vt ")){
            iss >> trash >> trash;
            Vec2d uv;
            for (int i = 0; i < 2; i++)
                iss >> uv[i];
            _uv.push_back(uv);
        }
        else if (!line.compare(0, 2, "f ")){
            std::vector<Vec3i> f;
            Vec3i tmp;
            iss >> trash;
            while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2])
            {
                for (int i = 0; i < 3; i++)
                    tmp[i]--;
                f.push_back(tmp);
            }
            _faces.push_back(f);
        }
    }

    _facenorms.resize(_faces.size());
    std::cerr << "# v# " << _verts.size() << " f# " << _faces.size()
              << " vt# " << _uv.size() << " vn# " << _norms.size() << std::endl;
    loadTexture(filename, "_diffuse.jpg");

    return this;
}

size_t Model::getVertsNum() const
{
    return _verts.size();
}

size_t Model::getFacesNum() const
{
    return _faces.size();
}

Vec3d &Model::norm(int iface, int nvert)
{
    int idx = _faces[iface][nvert][2];
    return _norms[idx].normalize();
}

vector<Vec3d>& Model::getVerts()
{
    return _verts;
}

vector<vector<Vec3i> >& Model::getFaces()
{
    return _faces;
}

vector<Vec3i>& Model::face(int i)
{
    return _faces[i];
}

Vec3d &Model::vertice(int i)
{
    return _verts[i];
}

Vec3d &Model::vertice(int iface, int nvert)
{
    return _verts[_faces[iface][nvert][0]];
}

void Model::loadTexture(const std::string* filename, const char *suffix)
{
    std::string textfile(*filename);
    size_t dot = textfile.find_last_of(".");
    if (dot != std::string::npos)
    {
        textfile = textfile.substr(0, dot) + std::string(suffix);
        QImageReader reader(QString(textfile.c_str()), "jpg");
        _diffusemap = reader.read();
        std::cerr << "textfile file " << textfile << " loading " << ((_diffusemap.isNull()) ? "failed" : "ok") << std::endl;
        _diffusemap = _diffusemap.mirrored(false, true);
    }
}

QColor Model::diffuse(Vec2i uv)
{
    return QColor(_diffusemap.pixel(uv.x, uv.y));
}

Vec2i Model::uv(int iface, int nvert)
{
    int idx = _faces[iface][nvert][1];
    return Vec2i(_uv[idx].x*_diffusemap.width(), _uv[idx].y*_diffusemap.height());
}

QImage* Model::getTexture()
{
    return &_diffusemap;
}

PositionInfo& Model::getPositionInfo()
{
    return this->posInfo;
}

void Model::setColor(QColor color)
{
    _color = color;
}

QColor& Model::getColor()
{
    return _color;
}

void Model::remakeNormals()
{
    _norms.resize(_verts.size());
    for (auto face : _faces)
    {
        for (auto vert : face)
            vert[2] = vert[0];
    }

    for (auto norm : _norms)
        norm[0] = norm[1] = norm[2] = 0;

   /* for (auto face : _faces)
    {
        for (auto vert : face)
            _norms[vert[2]] = _norms[vert[2]] + _verts[vert[0]];
    }*/
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

LightKoefs* Model::get_koefs()
{
    return &_koefs;
}

void Model::set_koefs(LightKoefs *koefs)
{
    _koefs = *koefs;
}

void Model::setKoefsFromColor(LightKoefs* koefs, QColor clr)
{
    koefs->amb_r = koefs->diff_r = koefs->spec_r = (double)clr.red()/255;
    koefs->amb_g = koefs->diff_g = koefs->spec_g = (double)clr.green()/255;
    koefs->amb_b = koefs->diff_b = koefs->spec_b = (double)clr.blue()/255;
    koefs->spec_r = std::max(0.0, koefs->spec_r/2);
    koefs->spec_g = std::max(0.0, koefs->spec_g/2);
    koefs->spec_b = std::max(0.0, koefs->spec_b/2);
}
