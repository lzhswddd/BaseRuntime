template<typename _Tp> inline void lzh::Swap(_Tp & a, _Tp & b) {
	_Tp temp = a;
	a = b;
	b = temp;
}
template<typename _Tp> inline bool lzh::Sort::comp_min(_Tp a, _Tp b) {
	return a > b;
}
template<typename _Tp> inline bool lzh::Sort::comp_max(_Tp a, _Tp b)
{
	return a < b;
}
template<typename _Tp, typename _Ptr> inline void lzh::Sort::bubble(_Tp * begin, _Tp * end, _Ptr compare) {
#ifdef LZH_DEBUG
	CHECK_PTR(begin);
	CHECK_PTR(end);
	CHECK_PTR_ORDER(begin, end);
#endif //LZH_DEBUG
	int32 i, j,
		length = ARRAY_LEN(begin, end);
	_Tp * m = begin;
	if (length > 0)
		for (i = 0; i < length - 1; i++)
			for (j = i + 1; j < length; j++) {
				if (compare(m[i], m[j]))
					Swap(m[i], m[j]);
			}
}
template<typename _Tp, typename _Ptr> inline void lzh::Sort::insert(_Tp * begin, _Tp * end, _Ptr compare) {
#ifdef LZH_DEBUG
	CHECK_PTR(begin);
	CHECK_PTR(end);
	CHECK_PTR_ORDER(begin, end);
#endif //LZH_DEBUG
	int32 i, j, k;
	int32 length = ARRAY_LEN(begin, end);
	_Tp * m = begin;
	if (length > 0)
		for (i = 0; i < length; i++)
			for (j = 0; j < i; j++) {
				if (compare(m[j], m[i]))
					for (k = i; k > j; k--)
						Swap(m[k], m[k - 1]);
			}
}
template<typename _Tp, typename _Ptr> inline void lzh::Sort::select(_Tp * begin, _Tp * end, _Ptr compare)
{
#ifdef LZH_DEBUG
	CHECK_PTR(begin);
	CHECK_PTR(end);
	CHECK_PTR_ORDER(begin, end);
#endif //LZH_DEBUG
	int32 i, j,
		index = 0,
		length = ARRAY_LEN(begin, end);
	_Tp * m = begin;
	if (length > 0)
		for (i = 0; i < length - 1; index = ++i) {
			for (j = i + 1; j < length; j++) {
				if (compare(m[index], m[j]))
					index = j;
			}
			if (i != index)
				Swap(m[i], m[index]);
		}
}
template<typename _Tp, typename _Ptr> inline void lzh::Sort::comb(_Tp * begin, _Tp * end, _Ptr compare)
{
#ifdef LZH_DEBUG
	CHECK_PTR(begin);
	CHECK_PTR(end);
	CHECK_PTR_ORDER(begin, end);
#endif //LZH_DEBUG
	int32 length = ARRAY_LEN(begin, end);
	int32 j, i, k;
	_Tp * m = begin;
	if (length > 0)
		for (k = 0, i = (int32)(length / shrink_factor); i > 1 || k; i = (i > 1) ? (int32)(i / shrink_factor) : i) {
			k = 0;
			for (j = 0; j < length - i; j++) {
				if (compare(m[j], m[j + i])) {
					Swap(m[j], m[j + i]);
					k = 1;
				}
			}
		}
}
template<typename _Tp, typename _Ptr> inline void lzh::Sort::gnome(_Tp * begin, _Tp * end, _Ptr compare)
{
#ifdef LZH_DEBUG
	CHECK_PTR(begin);
	CHECK_PTR(end);
	CHECK_PTR_ORDER(begin, end);
#endif //LZH_DEBUG
	int32 i, flag,
		length = ARRAY_LEN(begin, end);
	_Tp * m = begin;
	if (length > 0)
		for (i = 0; i < length&&i >= 0; flag ? i++ : i--, flag = 0) {
			if (i == 0 || compare(m[i], m[i - 1]))
				flag = 1;
			else
				Swap(m[i - 1], m[i]);
		}
}
template<typename _Tp, typename _Ptr> inline void lzh::Sort::heap(_Tp * begin, _Tp * end, _Ptr compare)
{
#ifdef LZH_DEBUG
	CHECK_PTR(begin);
	CHECK_PTR(end);
	CHECK_PTR_ORDER(begin, end);
#endif //LZH_DEBUG
	int32 length = ARRAY_LEN(begin, end);
	if (length > 0) {
		makeheap(begin, length, compare);
		for (int32 i = length - 1; i >= 1; i--)
			heaparray(begin, 0, i, compare);
	}
}
template<typename _Tp, typename _Ptr> inline void lzh::Sort::shell(_Tp * begin, _Tp * end, _Ptr compare)
{
#ifdef LZH_DEBUG
	CHECK_PTR(begin);
	CHECK_PTR(end);
	CHECK_PTR_ORDER(begin, end);
#endif //LZH_DEBUG
	int32 j, i, k,
		length = ARRAY_LEN(begin, end);
	_Tp * m = begin;
	if (length > 0)
		for (i = length >> 1; i > 0; i >>= 1)
			for (j = i; j < length; j++) {
				for (k = j - i; k >= 0 && compare(m[k], m[k + i]); k -= i)
					Swap(m[k], m[k + i]);
			}
}
template<typename _Tp, typename _Ptr> inline void lzh::Sort::quick(_Tp * begin, _Tp * end, _Ptr compare)
{
#ifdef LZH_DEBUG
	CHECK_PTR(begin);
	CHECK_PTR(end);
	CHECK_PTR_ORDER(begin, end);
#endif //LZH_DEBUG
	int32 length = ARRAY_LEN(begin, end);
	_Tp * m = begin;
	if (length > 0)
		_quick(m, 0, length - 1, compare);
}
template<typename _Tp, typename _Ptr> inline void lzh::Sort::merge(_Tp * begin, _Tp * end, _Ptr compare) {
#ifdef LZH_DEBUG
	CHECK_PTR(begin);
	CHECK_PTR(end);
	CHECK_PTR_ORDER(begin, end);
#endif //LZH_DEBUG
	_Tp *p;
	int32 length = ARRAY_LEN(begin, end);
	_Tp * m = begin;
	if (length > 0) {
		p = (_Tp *)malloc(length * sizeof(_Tp));
		CHECK_PTR(p);
		_merge(m, p, 0, length - 1, compare);
		free(p);
	}
}

template<typename _Tp, typename _Ptr> inline void lzh::Sort::heapdown(_Tp * m, int32 i, int32 n, _Ptr compare)
{
	for (int32 j = 2 * i + 1; j < n; i = j, j = 2 * j + 1) {
		if (j + 1 < n && compare(m[j + 1], m[j]))
			j++;
		if (!compare(m[j], m[i]))
			break;
		Swap(m[j], m[i]);
	}
}
template<typename _Tp, typename _Ptr> inline void lzh::Sort::heapup(_Tp * m, int32 i, _Ptr compare) {
	for (int32 j = (i - 1) / 2; j >= 0 && compare(m[i], m[j]); i = j, j = (i - 1) / 2)
		Swap(m[i], m[j]);
}
template<typename _Tp, typename _Ptr> inline void lzh::Sort::makeheap(_Tp * m, int32 length, _Ptr compare) {
	for (int32 i = 0; i < length; i++)
		heapup(m, i, compare);
}
template<typename _Tp, typename _Ptr> inline void lzh::Sort::heaparray(_Tp * m, int32 i, int32 n, _Ptr compare) {
	Swap(m[i], m[n]);
	heapdown(m, i, n, compare);
}
template<typename _Tp, typename _Ptr> inline void lzh::Sort::mergearray(_Tp * a, _Tp * b, int32 start, int32 mid, int32 end, _Ptr compare) {
	int32 i = start,
		j = mid + 1,
		k = 0;
	while (i <= mid && j <= end) {
		if (compare(a[j], a[i]))
			b[k++] = a[i++];
		else
			b[k++] = a[j++];
	}
	while (i <= mid)
		b[k++] = a[i++];
	while (j <= end)
		b[k++] = a[j++];
	for (i = 0; i < k; i++)
		a[start + i] = b[i];
}
template<typename _Tp, typename _Ptr> inline void lzh::Sort::_merge(_Tp * a, _Tp * b, int32 start, int32 end, _Ptr compare) {
	int32 mid;
	if (start < end) {
		mid = (start + end) / 2;
		_merge(a, b, start, mid, compare);
		_merge(a, b, mid + 1, end, compare);
		mergearray(a, b, start, mid, end, compare);
	}
}
template<typename _Tp, typename _Ptr> inline void lzh::Sort::_quick(_Tp * m, int32 low, int32 high, _Ptr compare) {
	int32 start, end;
	_Tp mark;
	if (low >= high)return;
	start = low;
	end = high;
	mark = m[start];
	while (start < end) {
		while (start < end && !compare(mark, m[end])) {
			end -= 1;
		}
		m[start] = m[end];
		while (start < end && !compare(m[start], mark)) {
			start += 1;
		}
		m[end] = m[start];
	}
	m[start] = mark;
	_quick(m, low, start - 1, compare);
	_quick(m, start + 1, high, compare);
}
