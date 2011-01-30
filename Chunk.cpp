#include "Chunk.h"

//#include <QImage>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

#include <cstdlib>
using namespace std;

bool Chunk::s_initialized = false;

uint qHash(const Chunk::Coord & coord)
{
    return (coord.x * 8191 + coord.z) * 131071 + coord.y;
}

Chunk::Chunk(const Coord &pos, const Coord &size) :
    m_pos(pos),
    m_size(size),
    m_blocks(m_size.x * m_size.y * m_size.z)
{
    initialize();

    // fill with dark air
    for (int i = 0; i < m_blocks.size(); i++) {
        Block * block = new Block();
        block->type = 0;
        block->light = 0;
        block->sky_light = 0;
        block->metadata = 0;
        m_blocks.replace(i, QSharedPointer<Block>(block));
    }
    // no need to update since air has null textures
}

void Chunk::initialize()
{
    if (s_initialized)
        return;
    s_initialized = true;

    {
        // grab all the textures from resources
        //QImage terrain(":/textures/terrain.png");
        //terrain.convertToFormat(QImage::Format_ARGB32_Premultiplied);

        QFile texture_index_file(":/textures/textures.txt");
        texture_index_file.open(QFile::ReadOnly);
        QTextStream stream(&texture_index_file);
        while (! stream.atEnd()) {
            QString line = stream.readLine().trimmed();
            if (line.isEmpty() || line.startsWith("#"))
                continue;
            QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
            Q_ASSERT(parts.size() == 5);
            QString name = parts.at(0);
            int x = parts.at(1).toInt();
            int y = parts.at(2).toInt();
            int w = parts.at(3).toInt();
            int h = parts.at(4).toInt();
            //QImage image = terrain.copy(x, y, w, h).rgbSwapped();
            //Texture * texture = new Texture(image);
            //s_textures.insert(name, texture);
        }
        texture_index_file.close();
    }

    {
        // grab all the solid block data from resources
        QFile blocks_file(":/textures/blocks.txt");
        blocks_file.open(QFile::ReadOnly);
        QTextStream stream(&blocks_file);
        while(! stream.atEnd()) {
            QString line = stream.readLine().trimmed();
            if (line.isEmpty() || line.startsWith("#"))
                continue;
            QStringList parts = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
            Q_ASSERT(parts.size() == 8);
            int id = parts.at(0).toInt();
            QString name = parts.at(1);
            Q_UNUSED(name);
            QString top = parts.at(2);
            QString bottom = parts.at(3);
            QString front = parts.at(4);
            QString back = parts.at(5);
            QString left = parts.at(6);
            QString right = parts.at(7);

//            Mesh * textured_cube = Mesh::createUnitCube(Constants::white,
//                s_textures.value(top),
//                s_textures.value(bottom),
//                s_textures.value(front),
//                s_textures.value(back),
//                s_textures.value(left),
//                s_textures.value(right));
//            s_meshes.replace(id, textured_cube);
        }
        blocks_file.close();
    }
}

int Chunk::indexOf(const Coord & coord) const
{
    return coord.y + (coord.z * m_size.y) + (coord.x * m_size.y * m_size.z);
}

QSharedPointer<Chunk::Block> Chunk::getBlock(const Coord & coord) const
{
    return m_blocks.at(indexOf(coord));
}

void Chunk::randomize()
{
    // create and allocate this chunk
    m_size.setValue(16, 16, 128);
    m_blocks.resize(16 * 16 * 128);

    // for each block, assign random values
    Coord coord;
    for (coord.z = 0; coord.z < m_size.z; coord.z++) {
        for (coord.y = 0; coord.y < m_size.y; coord.y++) {
            for (coord.x = 0; coord.x < m_size.x; coord.x++) {
                Block * new_block = new Block;
                new_block->type = (rand() % 20 == 1) ? (rand() % 92 + 1) : 0;
                if (rand() % 20 == 1)
                    new_block->type = 79;
                new_block->light = 0;
                new_block->metadata = 0;
                new_block->sky_light = 0;
                m_blocks.replace(indexOf(coord), QSharedPointer<Block>(new_block));
            }
        }
    }

}