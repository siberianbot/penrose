#ifndef PENROSE_ASSETS_ASSET_READER_INL
#define PENROSE_ASSETS_ASSET_READER_INL

namespace Penrose {

    template<typename T>
    T AssetReader::read() {
        T t;
        this->read(sizeof(T), &t);

        return t;
    }
}

#endif // PENROSE_ASSETS_ASSET_READER_INL
