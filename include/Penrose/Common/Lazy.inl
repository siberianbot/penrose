#ifndef PENROSE_COMMON_LAZY_INL
#define PENROSE_COMMON_LAZY_INL

namespace Penrose {

    template<typename T>
    void Lazy<T>::acquire() {
        if (this->_ptr.has_value()) {
            return;
        }

        this->_ptr = this->_provider();
    }

    template<typename T>
    Lazy<T>::Lazy(const Provider &provider)
            : _provider(provider) {
        //
    }

    template<typename T>
    T *Lazy<T>::operator->() {
        acquire();

        return this->_ptr.value();
    }
}

#endif // PENROSE_COMMON_LAZY_INL
