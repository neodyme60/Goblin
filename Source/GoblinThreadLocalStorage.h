#ifndef GOBLIN_THREAD_LOCAL_STORAGE_H
#define GOBLIN_THREAD_LOCAL_STORAGE_H

#include <boost/thread.hpp>
#include "GoblinDebugData.h"
#include "GoblinFilm.h"

namespace Goblin {

    class ThreadLocalStorage {
    public:
        virtual ~ThreadLocalStorage() {}
    };

    typedef boost::thread_specific_ptr<ThreadLocalStorage> TLSPtr;

    class TLSManager {
    public:
        // implement this method to initialize a thread local storage
        virtual void initialize(TLSPtr& tlsPtr) = 0;
        // implement this method to specify the operations on
        // thread local storage before corresponding thread life cycle finishs
        virtual void finalize(TLSPtr& tlsPtr) = 0;
    };

    class RenderingTLS : public ThreadLocalStorage {
    public:
        RenderingTLS(const Film& film): mTile(NULL), mSampleCount(0) {
            ImageRect r;
            film.getImageRect(r);
            const FilterTable& filterTable = film.getFilterTable();
            mTile = new ImageTile(r, filterTable);
        }

        ~RenderingTLS() {
            if (mTile) {
                delete mTile;
                mTile = NULL;
            }
        }

        ImageTile* getTile() { return mTile; }

        void addSampleCount(uint64_t sampleCount) {
            mSampleCount += sampleCount;
        }

        uint64_t getSampleCount() const { return mSampleCount; }

        DebugData& getDebugData() { return mDebugData; }

    private:
        ImageTile* mTile;
        uint64_t mSampleCount;
        DebugData mDebugData;
    };

    class RenderingTLSManager : public TLSManager {
    public:
        RenderingTLSManager(Film* film):
          mFilm(film), mTotalSampleCount(0) {}

        void initialize(TLSPtr& tlsPtr) {
            tlsPtr.reset(new RenderingTLS(*mFilm));
        }

        void finalize(TLSPtr& tlsPtr) {
            {
                boost::lock_guard<boost::mutex> lk(mMergeTLSMutex);
                RenderingTLS* renderingTLS =
                    static_cast<RenderingTLS*>(tlsPtr.get());
                mFilm->mergeTile(*renderingTLS->getTile());
                mTotalSampleCount += renderingTLS->getSampleCount();

                const DebugData& debugData = renderingTLS->getDebugData();
                const vector<pair<Ray, Color> >& debugRays =
                    debugData.getRays();
                for (size_t i = 0; i < debugRays.size(); ++i) {
                    mDebugData.addRay(debugRays[i].first, debugRays[i].second);
                }
                const vector<pair<Vector3, Color> >& debugPoints =
                    debugData.getPoints();
                for (size_t i = 0; i < debugPoints.size(); ++i) {
                    mDebugData.addPoint(debugPoints[i].first,
                        debugPoints[i].second);
                }
            }
        }

        uint64_t getTotalSampleCount() const { return mTotalSampleCount; }

        const DebugData& getDebugData() const { return mDebugData; }

    private:
        Film* mFilm;
        boost::mutex mMergeTLSMutex;
        uint64_t mTotalSampleCount;
        DebugData mDebugData;
    };
}

#endif //GOBLIN_THREAD_LOCAL_STORAGE_H