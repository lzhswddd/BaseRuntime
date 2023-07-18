template<typename _Tp> inline _Tp * lzh::alignPtr(_Tp * data, int32 n)
{
	return (_Tp *)(((size_t)data + n - 1) & -n);
}

template<typename _Tp, typename _T2> inline void lzh::addList(_Tp * p, int32 &i, const _T2 & arg)
{
	p[i++] = _Tp(arg);
}
template<typename _Tp, typename _T2, typename ...Types> inline void lzh::addList(_Tp * p, int32 &i, const _T2 & arg1, const Types & ...args)
{
	p[i++] = _Tp(arg1);
	addList(p, i, args...);
}

template<typename _Tp> inline _Tp lzh::Value::toData() const
{
	switch (_type)
	{
	case lzh::TP_8U:return saturate_cast<_Tp>(data.u8);
	case lzh::TP_8S:return saturate_cast<_Tp>(data.i8);
	case lzh::TP_16U:return saturate_cast<_Tp>(data.u16);
	case lzh::TP_16S:return saturate_cast<_Tp>(data.i16);
	case lzh::TP_32U:return saturate_cast<_Tp>(data.u32);
	case lzh::TP_32S:return saturate_cast<_Tp>(data.i32);
	case lzh::TP_64U:return saturate_cast<_Tp>(data.u64);
	case lzh::TP_64S:return saturate_cast<_Tp>(data.i64);
	case lzh::TP_32F:return saturate_cast<_Tp>(data.f32);
	case lzh::TP_64F:return saturate_cast<_Tp>(data.f64);
	case lzh::TP_64D:return saturate_cast<_Tp>(data.f128);
	default:THROW_INFO(ERR_INFO_TYPE);
	}
}

template<typename _Tp> inline lzh::Range_<_Tp>::Range_Iterator::Range_Iterator(_Tp v, _Tp gap) :v(v), gap(gap) {}

template<typename _Tp> inline typename lzh::Range_<_Tp>::Range_Iterator lzh::Range_<_Tp>::Range_Iterator::operator+(int32 off) const
{
	Range_Iterator iter = *this;
	iter.v += off * gap;
	return iter;
}
template<typename _Tp> inline typename lzh::Range_<_Tp>::Range_Iterator lzh::Range_<_Tp>::Range_Iterator::operator-(int32 off) const
{
	Range_Iterator iter = *this;
	iter.v -= off * gap;
	return iter;
}
template<typename _Tp> inline typename lzh::Range_<_Tp>::Range_Iterator lzh::Range_<_Tp>::Range_Iterator::operator+=(int32 off)
{
	v += off * gap;
	return *this;
}
template<typename _Tp> inline typename lzh::Range_<_Tp>::Range_Iterator lzh::Range_<_Tp>::Range_Iterator::operator-=(int32 off)
{
	v -= off * gap;
	return *this;
}
template<typename _Tp> inline typename lzh::Range_<_Tp>::Range_Iterator lzh::Range_<_Tp>::Range_Iterator::operator--()
{
	v -= gap;
	return *this;
}
template<typename _Tp> inline typename lzh::Range_<_Tp>::Range_Iterator lzh::Range_<_Tp>::Range_Iterator::operator--(int32)
{
	Range_Iterator iter = *this;
	v -= gap;
	return iter;
}
template<typename _Tp> inline typename lzh::Range_<_Tp>::Range_Iterator lzh::Range_<_Tp>::Range_Iterator::operator++()
{
	v += gap;
	return *this;
}
template<typename _Tp> inline typename lzh::Range_<_Tp>::Range_Iterator lzh::Range_<_Tp>::Range_Iterator::operator++(int32)
{
	Range_Iterator iter = *this;
	v += gap;
	return iter;
}
template<typename _Tp> inline _Tp& lzh::Range_<_Tp>::Range_Iterator::operator*()
{
	return v;
}
template<typename _Tp> inline const _Tp& lzh::Range_<_Tp>::Range_Iterator::operator*()const
{
	return v;
}
template<typename _Tp> inline _Tp& lzh::Range_<_Tp>::Range_Iterator::operator->()
{
	return v;
}
template<typename _Tp> inline  const _Tp& lzh::Range_<_Tp>::Range_Iterator::operator->()const
{
	return v;
}
template<typename _Tp> inline bool lzh::Range_<_Tp>::Range_Iterator::operator==(const typename Range_<_Tp>::Range_Iterator & iter) const
{
	return isFloat(MAT_TYPE(_Tp)) ? isEqual(_T(v), _T(iter.v), _T(gap)*0.1) : v == iter.v;
}
template<typename _Tp> inline bool lzh::Range_<_Tp>::Range_Iterator::operator!=(const typename Range_<_Tp>::Range_Iterator & iter) const
{
	return isFloat(MAT_TYPE(_Tp)) ? !isEqual(_T(v), _T(iter.v), _T(gap)*0.1) : v != iter.v;
}
template<typename _Tp> inline bool lzh::Range_<_Tp>::Range_Iterator::operator>(const typename Range_<_Tp>::Range_Iterator & iter) const
{
	return v > iter.v;
}
template<typename _Tp> inline bool lzh::Range_<_Tp>::Range_Iterator::operator>=(const typename Range_<_Tp>::Range_Iterator & iter) const
{
	return v >= iter.v;
}
template<typename _Tp> inline bool lzh::Range_<_Tp>::Range_Iterator::operator<=(const typename Range_<_Tp>::Range_Iterator & iter) const
{
	return v <= iter.v;
}
template<typename _Tp> inline bool lzh::Range_<_Tp>::Range_Iterator::operator < (const typename Range_<_Tp>::Range_Iterator & iter) const
{
	return v < iter.v;
}

/****************************************************************************
½Ó¿ÚÀà
*****************************************************************************/
template<class _Tp> inline lzh::MatPtr<_Tp>::MatPtr()
{
	init(0, 0, 0);
}
template<class _Tp> inline lzh::MatPtr<_Tp>::MatPtr(int32 size)
{
	create(size);
}
template<class _Tp> inline lzh::MatPtr<_Tp>::MatPtr(const MatPtr<_Tp> & m)
{
	init(m.data, m.len, m.refcount);
	if (refcount)
		MAT_XADD(refcount, 1);
}
template<class _Tp> inline lzh::MatPtr<_Tp>::MatPtr(const _Tp *p, int32 n)
{
	init(p, n, 0);
}
template<class _Tp> template<typename ...Types> inline lzh::MatPtr<_Tp>::MatPtr(int32 size, const Types & ...args)
{
	create(size);
	addList(0, args...);
}
template<class _Tp> inline lzh::MatPtr<_Tp>::~MatPtr() { release(); }
template<class _Tp> void inline lzh::MatPtr<_Tp>::create(uint32 len) {
	release();
	this->len = len;
	uint32 totalsize = len * sizeof(_Tp);
	data = (_Tp*)fastMalloc(totalsize + sizeof(int32));
	refcount = (int32*)(((uint8 *)data) + totalsize);
	*refcount = 1;
}
template<class _Tp> inline void lzh::MatPtr<_Tp>::addref() {
	if (refcount)
		MAT_XADD(refcount, 1);
}
template<class _Tp> inline void lzh::MatPtr<_Tp>::release() {
	if (refcount && MAT_XADD(refcount, -1) == 1)
		fastFree(data);
	init(0, 0, 0);
}
template<class _Tp> inline lzh::MatPtr<_Tp> & lzh::MatPtr<_Tp>::operator=(const MatPtr<_Tp> & m) {
	if (this == &m)
		return *this;

	if (m.refcount)
		MAT_XADD(m.refcount, 1);

	release();
	data = m.data;
	len = m.len;
	refcount = m.refcount;
	return *this;
}
template<class _Tp> inline bool lzh::MatPtr<_Tp>::operator==(const MatPtr<_Tp> & m) const
{
	return data == m.data;
}
template<class _Tp> inline bool lzh::MatPtr<_Tp>::operator!=(const MatPtr<_Tp> & m) const
{
	return data != m.data;
}
template<class _Tp> inline bool lzh::MatPtr<_Tp>::operator==(void * m) const { return (this->data == m); }
template<class _Tp> inline bool lzh::MatPtr<_Tp>::operator!=(void * m) const { return (this->data != m); }
template<class _Tp> inline bool lzh::MatPtr<_Tp>::empty() const
{
	return data == nullptr;
}
template<class _Tp> inline lzh::int32 lzh::MatPtr<_Tp>::size() const {
	return len;
}
template<class _Tp> inline lzh::int32 & lzh::MatPtr<_Tp>::L() {
	return len;
}
template<class _Tp> inline _Tp *& lzh::MatPtr<_Tp>::P() {
	return data;
}
template<class _Tp> inline _Tp * lzh::MatPtr<_Tp>::obj()const {
	return data;
}
template<class _Tp> inline void lzh::MatPtr<_Tp>::set(const void * p, int32 length) {
	data = (_Tp*)p; len = length;
}
template<class _Tp> inline lzh::int32* lzh::MatPtr<_Tp>::getCount() const {
	return refcount;
}
template<class _Tp> inline void lzh::MatPtr<_Tp>::setCount(int32 * refcount) {
	this->refcount = refcount;
}
template<class _Tp> inline void lzh::MatPtr<_Tp>::init(const _Tp* p, int32 len, const int32 *count) {
	data = (_Tp*)p; this->len = len; refcount = (int32*)count;
}
template<class _Tp> inline void lzh::MatPtr<_Tp>::set(const MatPtr<_Tp> & ptr) {
	data = ptr.data; len = ptr.len;
}
template<class _Tp> inline _Tp * lzh::MatPtr<_Tp>::begin()
{
	return data;
}
template<class _Tp> inline const _Tp * lzh::MatPtr<_Tp>::begin() const
{
	return data;
}
template<class _Tp> inline _Tp * lzh::MatPtr<_Tp>::end()
{
	if (data == nullptr)return nullptr;
	if (refcount == nullptr)
		return data + len;
	return (_Tp*)refcount;
}
template<class _Tp> inline const _Tp * lzh::MatPtr<_Tp>::end() const
{
	if (data == nullptr)return nullptr;
	if (refcount == nullptr)return data + len;
	return (const _Tp*)refcount;;
}
template<class _Tp> inline _Tp & lzh::MatPtr<_Tp>::at(int32 i) const
{
#ifdef LZH_DEBUG
	CHECK_PTR(data);
	CHECK_INDEX(i, len);
#endif // LZH_DEBUG
	return data[i];
}
template<class _Tp> inline _Tp & lzh::MatPtr<_Tp>::operator[](int32 i) const {
#ifdef LZH_DEBUG
	CHECK_PTR(data);
	CHECK_INDEX(i, len);
#endif // LZH_DEBUG
	return data[i];
}
template<class _Tp> inline lzh::MatPtr<_Tp>::operator _Tp*() {
	return (_Tp*)data;
}
template<class _Tp> inline lzh::MatPtr<_Tp>::operator const _Tp*() const {
	return (const _Tp*)data;
}

template<typename _Tp> bool lzh::isType(int32 type)
{
	return DataType<_Tp>::type == type;
}

template<typename _Tp> lzh::int32 lzh::Floor(_Tp x)
{
	return int32(floor(x));
}