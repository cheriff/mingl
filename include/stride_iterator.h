#ifndef __STRIDE_ITERATOR_H
#define __STRIDE_ITERATOR_H

#include <iterator>

struct stride_iter
{
    stride_iter( ) : ptr(NULL), step(0), count(0), upto(0) { };

    stride_iter(const stride_iter& x) :
        ptr(x.ptr), step(x.step), count(x.count), upto(x.upto) { }

    stride_iter(void *x, size_t n, size_t c) :
        ptr(x), step(n), count(c) , upto(0) { }
    stride_iter(void *x, size_t n, size_t c, int ut) :
        ptr(x), step(n), count(c) , upto(ut) { }

    bool operator==(stride_iter a) {
        return (a.ptr == ptr) && (a.upto == upto);
    }
    bool operator!=(stride_iter a) {
        return (a.ptr != ptr) || (a.upto != upto);
    }

    stride_iter& operator++()
    {
        count--;
        upto++;
        return *this;
    }

    int operator*()
    {
        char *cp = (char*)ptr;
        switch(step) {
            case 1: {
                int8_t i8 = *(int8_t*)(cp+upto);
                return int(i8);
            }
            case 2: {
                int16_t i16 = *(int16_t*)(cp+upto*2);
                return int(i16);
            }
            case 4: {
                int32_t i32 = *(int32_t*)(cp+upto*4);
                return int(i32);
            }
        }
        assert(!"DEAD");
    }



    private:
    void *ptr;
    size_t step;
    size_t count;
    size_t upto;
};


#endif
