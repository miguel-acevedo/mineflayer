#ifndef CHUNK_H
#define CHUNK_H

#include <QtGlobal>

#include <OpenEXR/ImathVec.h>
using namespace Imath;

uint qHash(const Vec3<int> & coord);

#include <QVector>
#include <QSharedPointer>
#include <QHash>

class Chunk {
public:
    typedef Vec3<int> Coord;

    struct Block {
        int type;
        int metadata;
        int light;
        int sky_light;
    };

public:
    Chunk(const Coord & pos, const Coord & size);

    // use to set or retrieve Block data. Call updateBlock when done.
    QSharedPointer<Block> getBlock(const Coord & coord) const;

    // debug method to generate random blocks
    void randomize();

    Chunk::Coord position() const { return m_pos; }
    Chunk::Coord size() const { return m_size; }

    static void initialize();

private:
    Coord m_pos;
    Vec3<int> m_size;
    QVector<QSharedPointer<Block> > m_blocks;

    static bool s_initialized;

private:
    int indexOf(const Coord & coord) const;
};

#endif // CHUNK_H