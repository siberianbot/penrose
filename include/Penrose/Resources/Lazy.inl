#ifndef PENROSE_RESOURCES_LAZY_INL
#define PENROSE_RESOURCES_LAZY_INL

namespace Penrose {

    template<IsResource T>
    Lazy<T>::Lazy(Lazy::Factory factory)
            : _factory(factory),
              _payload(std::nullopt) {
        //
    }

    template<IsResource T>
    T *Lazy<T>::operator->() {
        return this->acquire();
    }

    template<IsResource T>
    T *Lazy<T>::get() {
        return this->acquire();
    }

    template<IsResource T>
    void Lazy<T>::reset() {
        this->_payload = std::nullopt;
    }

    template<IsResource T>
    T *Lazy<T>::acquire() {
        if (!this->_payload.has_value()) {
            this->_payload = this->_factory();
        }

        return *this->_payload;
    }

    template<IsResource T>
    LazyCollection<T>::LazyCollection(LazyCollection::Factory factory)
            : _factory(factory),
              _payload(std::nullopt) {
        //
    }

    template<IsResource T>
    LazyCollection<T>::Iterator LazyCollection<T>::begin() {
        return this->acquire().begin();
    }

    template<IsResource T>
    LazyCollection<T>::Iterator LazyCollection<T>::end() {
        return this->acquire().end();
    }

    template<IsResource T>
    void LazyCollection<T>::reset() {
        this->_payload = std::nullopt;
    }

    template<IsResource T>
    std::vector<T *> &LazyCollection<T>::acquire() {
        if (!this->_payload.has_value()) {
            this->_payload = this->_factory();
        }

        return *this->_payload;
    }
}

#endif // PENROSE_RESOURCES_LAZY_INL
