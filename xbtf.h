/*
 * Copyright (C) 2012 Lars Hall
 *
 * This file is part of xbtfextractor.
 *
 * xbtfextractor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * xbtfextractor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with xbtfextractor.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef XBTF_H
#define XBTF_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <vector>
#include <map>

#include <lzo/lzo1x.h>
#include <iostream>
#include <jpeglib.h>
#include <png.h>
#include <gif_lib.h>
#include <squish.h>

#define MAGIC "XBTF"
// Max size of string is 256 in a xbtf file
#define MAX_STR 256

// Taken from XBMC
#define XB_FMT_MASK   0xffff
#define XB_FMT_DXT_MASK   15
#define XB_FMT_UNKNOWN     0
#define XB_FMT_DXT1        1
#define XB_FMT_DXT3        2
#define XB_FMT_DXT5        4
#define XB_FMT_DXT5_YCoCg  8
#define XB_FMT_A8R8G8B8   16
#define XB_FMT_A8         32
#define XB_FMT_RGBA8      64
#define XB_FMT_RGB8      128
#define XB_FMT_OPAQUE  65536

using std::vector;
using std::map;
using std::make_pair;

struct Frame
{
    uint32_t width;
    uint32_t height;
    uint32_t duration;
    uint32_t format;
    uint64_t packed;
    uint64_t unpacked;
    uint64_t offset;
    uint8_t *bytes;
};

struct MediaFile 
{
    enum FileType
    {
        JPEG = 0,
        PNG,
        GIF
    };

    FileType fileType;
    char fullPath[256];
    char filename[256];
    char path[256];
    uint32_t loop;
    uint32_t numFrames;
    vector<Frame> frames;
};

// C string compare for keys in map
struct strCmp
{
    bool operator()(char const *a, char const *b) const
    {
        return strcmp(a, b) < 0;
    }
};

class Xbtf
{
    private:
        char version[2];
        // TODO: Should use a hashtable instead
        typedef map<char*, MediaFile, strCmp> MediaFiles;
        // grr map has to contain a value
        typedef map<const char*, bool, strCmp> PathCache;

        MediaFiles mediaFiles;
        PathCache pathCache;

        FILE *file;
        bool fileOpened;

        bool readHeader();
        bool readMeta();

        void readStr(char *buf, size_t size);
        uint32_t readU32();
        uint64_t readU64();

        void parsePath(MediaFile &mediaFile);
        bool createPath(const char *path);
        int makeDir(const char *dir);
        bool isPathInCache(const char *path);

        void extractMediaFile(MediaFile &mediaFile,
            const char *dst, bool createDirs);

        bool uncompressFrame(unsigned char *dst, const Frame &frame);
        bool decompressDXT(unsigned char *data,
            unsigned int len, const Frame &frame);

        bool compressJpeg(const Frame &frame, unsigned char *data,
            const char *filename);
        bool compressPng(const Frame &frame, unsigned char *data,
            const char *filename);
        bool compressGif(const Frame &frame, unsigned char *data,
            const char *filename);


    public:
        Xbtf();
        ~Xbtf();

        bool open(const char *filename);
        bool parse();
        void extractFile(const char *filename,
            const char *dst, bool createDirs);
        void extractAllFiles(const char *dst, bool createDirs);
        void printFiles();
};

#endif
