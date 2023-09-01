#ifndef PENROSE_RESOURCES_LAZY_INL
#define PENROSE_RESOURCES_LAZY_INL

namespace Penrose {

    template<class T>
    Lazy<T>::Lazy(Lazy::Factory factory)
            : _factory(factory),
              _payload(std::nullopt) {
        //
    }

    template<class T>
    T *Lazy<T>::operator->() {
        return this->acquire();
    }

    template<class T>
    T *Lazy<T>::get() {
        return this->acquire();
    }

    template<class T>
    void Lazy<T>::reset() {
        this->_payload = std::nullopt;
    }

    template<class T>
    T *Lazy<T>::acquire() {
        if (!this->_payload.has_value()) {
            this->_payload = this->_factory();
        }

        return *this->_payload;
    }

    template<class T>
    LazyCollection<T>::LazyCollection(LazyCollection::Factory factory)
            : _factory(factory),
              _payload(std::nullopt) {
        //
    }

    template<class T>
    LazyCollection<T>::Iterator LazyCollection<T>::begin() {
        return this->acquire().begin();
    }

    template<class T>
    LazyCollection<T>::Iterator LazyCollection<T>::end() {
        return this->acquire().end();
    }

    template<class T>
    void LazyCollection<T>::reset() {
        this->_payload = std::nullopt;
    }

    template<class T>
    std::vector<T *> &LazyCollection<T>::acquire() {
        if (!this->_payload.has_value()) {
            this->_payload = this->_factory();
        }

        return *this->_payload;
    }
}

#endif // PENROSE_RESOURCES_LAZY_INL
