#ifndef GOBLIN_FILM_H
#define GOBLIN_FILM_H

#include "GoblinColor.h"
#include "GoblinFactory.h"
#include "GoblinUtils.h"
#include "GoblinVector.h"

namespace Goblin {

    const int FILTER_TABLE_WIDTH = 16;
    class Sample;
    class Filter;

    class Pixel {
    public:
        Pixel(): color(Color::Black), weight(0.0f) {}
        Color color;
        float weight;
        float pad[3];
    };

    typedef pair<Vector2, Vector2> DebugLine;

    class DebugInfo {
    public:
        DebugInfo() {}
        void addLine(const DebugLine& l, const Color& c);
        void addPoint(const Vector2& p, const Color& c);
        const vector<pair<DebugLine, Color> >& getLines() const;
        const vector<pair<Vector2, Color> >& getPoints() const;
    private:
        vector<pair<DebugLine, Color> > mLines;
        vector<pair<Vector2, Color> > mPoints;
    };

    inline void DebugInfo::addLine(const DebugLine& l, const Color& c) { 
        mLines.push_back(pair<DebugLine, Color>(l, c)); 
    }

    inline void DebugInfo::addPoint(const Vector2& p, const Color& c) { 
        mPoints.push_back(pair<Vector2, Color>(p, c)); 
    }

    inline const vector<pair<DebugLine, Color> >& DebugInfo::getLines() const {
        return mLines;
    }

    inline const vector<pair<Vector2, Color> >& DebugInfo::getPoints() const {
        return mPoints;
    }

    struct ImageRect {
        ImageRect() {}
        ImageRect(int x, int y, int w, int h): 
            xStart(x), yStart(y), xCount(w), yCount(h) {}
        int xStart, yStart, xCount, yCount;
    };

    class ImageTile {
    public:
        ImageTile(int tileWidth, int rowId, int rowNum, int colId, int colNum, 
            const ImageRect& imageRect, const Filter* filter, 
            const float* filterTable);
        ~ImageTile();
        void getImageRange(int* xStart, int *xEnd,
            int* yStart, int* yEnd) const;
        void getSampleRange(int* xStart, int* xEnd,
            int* yStart, int* yEnd) const;
        const Pixel* getTileBuffer() const;
        const DebugInfo& getDebugInfo() const;
        void addSample(const Sample& sample, const Color& L);
        void addDebugLine(const DebugLine& l, const Color& c);
        void addDebugPoint(const Vector2& p, const Color& c);
        void setInvPixelArea(float invPixelArea);
    private:
        int mTileWidth;
        int mRowId, mRowNum;
        int mColId, mColNum;
        ImageRect mTileRect;
        ImageRect mImageRect;
        Pixel* mPixels;
        const Filter* mFilter;
        const float* mFilterTable;
        DebugInfo mDebugInfo;
        float mInvPixelArea;
    };

    inline const Pixel* ImageTile::getTileBuffer() const {
        return mPixels;
    }

    inline const DebugInfo& ImageTile::getDebugInfo() const {
        return mDebugInfo;
    }

    inline void ImageTile::addDebugLine(const DebugLine& l, const Color& c) {
        mDebugInfo.addLine(l, c);
    }

    inline void ImageTile::addDebugPoint(const Vector2& p, const Color& c) {
        mDebugInfo.addPoint(p, c);
    }

    inline void ImageTile::setInvPixelArea(float invPixelArea) {
        mInvPixelArea = invPixelArea;
    }

    class Film {
    public:
        Film(int xRes, int yRes, const float crop[4], 
            Filter* filter, const std::string& filename, 
            int tileWidth, bool toneMapping = false, 
            float bloomRadius = 0.0f, float bloomWeight = 0.0f);
        ~Film();

        int getXResolution() const;
        int getYResolution() const;
        float getInvXResolution() const;
        float getInvYResolution() const;
        void getSampleRange(int* xStart, int* xEnd,
            int* yStart, int* yEnd) const;
        vector<ImageTile*>& getTiles();
        void addSample(float imageX, float imageY, const Color& L);
        void setFilmArea(float filmArea);
        float getFilmArea() const;
        void mergeTiles();
        void scaleImage(float s);
        void writeImage(bool normalize = true);

    private:
        int mXRes, mYRes;
        int mXStart, mYStart, mXCount, mYCount;
        float mInvXRes, mInvYRes;
        float mFilterTable[FILTER_TABLE_WIDTH * FILTER_TABLE_WIDTH];
        float mCrop[4];
        Filter* mFilter;
        Pixel* mPixels;
        vector<ImageTile*> mTiles;
        std::string mFilename;
        int mTileWidth;
        bool mToneMapping;
        float mBloomRadius;
        float mBloomWeight;
        float mFilmArea;
        float mInvPixelArea;
    };

    inline int Film::getXResolution() const { return mXRes; }

    inline int Film::getYResolution() const { return mYRes; }

    inline float Film::getInvXResolution() const { return mInvXRes; }
    
    inline float Film::getInvYResolution() const { return mInvYRes; }

    inline float Film::getFilmArea() const { return mFilmArea; }

    inline vector<ImageTile*>& Film::getTiles() { return mTiles; }

    class ImageFilmCreator : public Creator<Film, const ParamSet&, Filter*> {
    public:
        Film* create(const ParamSet& params, Filter* filter) const;
    };
}

#endif //GOBLIN_FILM_H
