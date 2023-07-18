#ifndef CONTAINER_H
#define CONTAINER_H

#include <set>
#include <queue>
#include <vector>

namespace lzh
{
template<typename _Tp> bool Contains(const _Tp &_container,  const typename _Tp::value_type &_val){
    if(_container.empty())return false;
    auto iter = std::find(_container.begin(),_container.end(),_val);
    return iter != _container.end();
}
template<typename _Tp>
class Queue : public std::queue<_Tp>
{
public:
    using Base = std::queue<_Tp>;
    Queue() : Base(){}
    bool isEmpty()const noexcept{
        return Base::empty();
    }
    bool contains(const _Tp &_val) const noexcept{
        bool success = false;
        auto size = Base::size();
        for(auto i=size;i!=0;i--){
            const _Tp & val = dequeue();
            if(val == _val){
                success = true;
            }
            enqueue(val);
        }
        return success;
    }
    void enqueue(const _Tp &_val){
        Base::push(_val);
    }
    _Tp & dequeue(){
        _Tp & _val = Base::front();
        Base::pop();
        return _val;
    }
    size_t count()const noexcept{
        return Base::size();
    }
    _Tp & first(){
        return Base::front();
    }
    const _Tp & first()const{
        return Base::front();
    }
    _Tp & last(){
        return Base::back();
    }
    const _Tp & back()const{
        return Base::front();
    }
    void clear(){
        Queue<_Tp>().swap(*this);
    }
    bool removeOne(const _Tp & _val){
        bool success = false;
        auto size = Base::size();
        for(auto i=size;i!=0;i--){
            const _Tp & val = dequeue();
            if(val != _val){
                enqueue(val);
            }else{
                success = true;
            }
        }
        return success;
    }
};

template<typename _Tp>
class Set : public std::set<_Tp>
{
public:
    using Base = std::set<_Tp>;
    Set() : Base(){}
    bool isEmpty()const noexcept{
        return Base::empty();
    }
    bool contains(const _Tp &_val) const noexcept{
        return Contains(*this, _val);
    }
    size_t count()const noexcept{
        return Base::size();
    }
};

template<typename _Tp>
class List : public std::vector<_Tp>
{
public:
    using Base = std::vector<_Tp>;
    List() : Base(){}
    bool isEmpty()const noexcept{
        return Base::empty();
    }
    bool contains(const _Tp &_val) const noexcept{
        return Contains(*this, _val);
    }
    size_t count()const noexcept{
        return Base::size();
    }
    _Tp & first(){
        return Base::front();
    }
    const _Tp & first()const{
        return Base::front();
    }
    _Tp & last(){
        return Base::back();
    }
    const _Tp & back()const{
        return Base::front();
    }
    void removeFirst(){
        if(Base::empty())return;
        Base::erase(Base::begin());
    }
    void removeEnd(){
        if(Base::empty())return;
        Base::erase(Base::end());
    }
    bool removeOne(const _Tp & _val){
        auto iter = std::find(Base::begin(), Base::end(), _val);
        if(iter != Base::end()){
            Base::erase(iter);
            return true;
        }return false;
    }
    typename std::vector<_Tp>::iterator insert(size_t idx, const _Tp &_val){
        return Base::insert(Base::begin() + idx, _val);
    }
};
}

#endif // CONTAINER_H
