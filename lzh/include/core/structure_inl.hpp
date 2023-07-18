#include <iostream>
template<class Type> inline lzh::LinkIter<Type>::LinkIter(Node<Type>* p)
{
	_ptr = p;
}
template<class Type> inline const lzh::LinkIter<Type> lzh::LinkIter<Type>::operator = (const LinkIter<Type> &iter)
{
	_ptr = iter._ptr;
	return *this;
}
template<class Type> inline lzh::int32 lzh::LinkIter<Type>::operator-(const LinkIter<Type> iter)const
{
	return _ptr->index - iter._ptr->index;
}
template<class Type> inline lzh::int32 lzh::LinkIter<Type>::operator+(const LinkIter<Type> iter)const
{
	return _ptr->index + iter._ptr->index;
}
template<class Type> inline bool lzh::LinkIter<Type>::operator != (const LinkIter<Type> &iter)const {
	return _ptr->index != iter._ptr->index;
}
template<class Type> inline bool lzh::LinkIter<Type>::operator == (const LinkIter<Type> &iter)const {
	return _ptr->index == iter._ptr->index;
}
template<class Type> inline bool lzh::LinkIter<Type>::operator>(const LinkIter<Type>& iter)
{
	return _ptr->index > iter._ptr->index;
}
template<class Type> inline bool lzh::LinkIter<Type>::operator<(const LinkIter<Type>& iter)
{
	return _ptr->index < iter._ptr->index;
}
template<class Type> inline const lzh::LinkIter<Type> lzh::LinkIter<Type>::operator ++ () {
	_ptr = _ptr->next;
	return *this;
}
template<class Type> inline const lzh::LinkIter<Type> lzh::LinkIter<Type>::operator ++ (int32) {
	LinkIter<Type> tmp = *this;
	_ptr = _ptr->next;
	return tmp;
}
template<class Type> inline const lzh::LinkIter<Type> lzh::LinkIter<Type>::operator+(int32 count)
{
	LinkIter<Type> iter(*this);
	while (count--) {
		iter._ptr = iter._ptr->next;
	}
	return iter;
}
template<class Type> inline const lzh::LinkIter<Type> lzh::LinkIter<Type>::operator+=(int32 count)
{
	while (count--) {
		_ptr = _ptr->next;
	}
	return *this;
}
template<class Type> inline const lzh::LinkIter<Type> lzh::LinkIter<Type>::operator--()
{
	_ptr = _ptr->pre;
	return *this;
}
template<class Type> inline const lzh::LinkIter<Type> lzh::LinkIter<Type>::operator--(int32)
{
	LinkIter<Type> tmp = *this;
	_ptr = _ptr->pre;
	return tmp;
}
template<class Type> inline const lzh::LinkIter<Type> lzh::LinkIter<Type>::operator-(int32 count)
{
	LinkIter<Type> iter(*this);
	while (count--) {
		iter._ptr = iter._ptr->pre;
	}
	return iter;
}
template<class Type> inline const lzh::LinkIter<Type> lzh::LinkIter<Type>::operator-=(int32 count)
{
	while (count--) {
		_ptr = _ptr->pre;
	}
	return *this;
}
template<class Type> inline Type& lzh::LinkIter<Type>::operator * ()const {
	return _ptr->data;
}
template<class Type> inline lzh::Node<Type>* lzh::LinkIter<Type>::operator()() const
{
	return _ptr;
}

template<class Type> inline lzh::LinkList<Type>::LinkList() : len(0), head(new Node<Type>), tail(new Node<Type>), left(nullptr), right(nullptr)
{
	initPtr();
	head->next = tail;
	tail->pre = head;
}
template<class Type> inline lzh::LinkList<Type>::LinkList(int32 list_length) : len(list_length), head(new Node<Type>), tail(new Node<Type>), left(nullptr), right(nullptr)
{
	initPtr();
	head->next = tail;
	tail->pre = head;
	head->index = 0;
	for (size_t index = 0; index < len; ++index) {
		Node<Type> *temp = new Node<Type>();
		if (temp == nullptr) {
			fprintf(stderr, "alloc memory fail!");
			throw temp;
		}
		temp->pre = tail;
		temp->index = right->index + 1;
		right->next = temp;
		right = right->next;
	}
	right->next = tail;
	tail->index = right->index + 1;
	tail->pre = right;
	PtrReDire();
}
template<class Type> inline lzh::LinkList<Type>::LinkList(LinkList & linklist) : len(0), head(new Node<Type>), tail(new Node<Type>), left(nullptr), right(nullptr)
{
	initPtr();
	head->next = tail;
	tail->pre = head;
	head->index = 0;
	for (size_t index = 0; index < linklist.size(); ++index) {
		Node<Type> *temp = new Node<Type>(linklist[index], nullptr, right);
		if (temp == nullptr) {
			fprintf(stderr, "alloc memory fail!");
			throw temp;
		}
		temp->index = right->index + 1;
		right->next = temp;
		right = right->next;
	}
	right->next = tail;
	tail->index = right->index + 1;
	tail->pre = right;
	len = linklist.size();
	PtrReDire();
}
template<class Type> inline lzh::LinkList<Type>::~LinkList()
{
	clear();
	delete head;
	delete tail;
}
template<class Type> inline bool lzh::LinkList<Type>::empty()
{
	if (len == 0)
		return false;
	return true;
}
template<class Type> inline lzh::int32 lzh::LinkList<Type>::length()
{
	return int32(len);
}
template<class Type> inline size_t lzh::LinkList<Type>::size()
{
	return len;
}
template<class Type> inline void lzh::LinkList<Type>::insert(iterator start, const iterator copy_start, const iterator copy_end)
{
	if (start < end()) {
		fprintf(stderr, "error:System.IndexOutOfBoundsException!");
	}
	iterator Next = start;
	if (Next == end()) {
		--Next;
		start--;
	}
	for (LinkList<Type>::iterator iter = copy_start; iter != copy_end; ++iter) {
		iterator Tail = Next + 1;
		iterator Next_ = iterator(CreatNode(*iter, Next(), (Next + 1)()));
		Tail()->pre = Next_();
		Next()->next = Next_();
		Next = Next_;
		len += 1;
	}
	UpdateIndex(start);
	PtrReDire();
}
template<class Type> inline void lzh::LinkList<Type>::insert(const Type source, const size_t index)
{
	if (index >= len) {
		fprintf(stderr, "error:System.IndexOutOfBoundsException!");
	}
	left = PosPtr(index)->pre;
	right = left->next;
	left->next = CreatNode(source, left, right);
	right->pre = left->next;
	UpdateIndex(index);
	len += 1;
	PtrReDire();
}
template<class Type> inline void lzh::LinkList<Type>::insert(LinkList<Type>& list, const size_t index, const size_t copy_start, const size_t copy_end)
{
	if (index >= len) {
		fprintf(stderr, "error:System.IndexOutOfBoundsException!");
	}
	left = PosPtr(index)->pre;
	for (size_t ind = copy_start; ind < copy_end; ++ind) {
		right = left->next;
		left->next = CreatNode(list[ind], left, right);
		right->pre = left->next;
		left = left->next;
	}
	UpdateIndex(index);
	len += (copy_end - copy_start);
	PtrReDire();
}
template<class Type> inline void lzh::LinkList<Type>::append(const Type source)
{
	right->next = CreatNode(source);
	right->next->index = right->index + 1;
	right->next->pre = right;
	right->next->next = tail;
	tail->pre = right->next;
	tail->index = right->next->index + 1;
	len += 1;
	PtrReDire();
}
template<class Type> inline void lzh::LinkList<Type>::push_back(const Type source)
{
	append(source);
}
template<class Type> inline void lzh::LinkList<Type>::print()
{
	if (len == 0)return;
	Node<Type> *ptr = head->next;
	std::cout << "[";
	while (ptr->next != tail) {
		std::cout << ptr->data << ", ";
		ptr = ptr->next;
	}
	std::cout << ptr->data << "]" << std::endl;
}
template<class Type> inline void lzh::LinkList<Type>::clear()
{
	right = head->next;
	while (right != tail) {
		left = right->next;
		delete right;
		right = left;
	}
	head->next = tail;
	tail->pre = head;
	initPtr();
	len = 0;
}
template<class Type> inline lzh::LinkIter<Type> lzh::LinkList<Type>::begin()
{
	return iterator(head->next);
}
template<class Type> inline lzh::LinkIter<Type> lzh::LinkList<Type>::end()
{
	return iterator(tail);
}
template<class Type> inline Type & lzh::LinkList<Type>::operator[](const int32 index)
{
	return PosPtr(index)->data;
}
template<class Type> inline void lzh::LinkList<Type>::initPtr()
{
	left = head;
	right = head;
}
template<class Type> inline void lzh::LinkList<Type>::PtrReDire()
{
	left = head;
	right = tail->pre;
}
template<class Type> inline void lzh::LinkList<Type>::UpdateIndex(const size_t index)
{
	if (right->index != index + 1) {
		right = head->next;
		for (size_t ind = 0; ind < index; ++ind) {
			right = right->next;
		}
	}
	while (right->next != nullptr) {
		right->next->index = right->index + 1;
		right = right->next;
	}
}
template<class Type> inline void lzh::LinkList<Type>::UpdateIndex(iterator start)
{
	if (start() == head)start++;
	while (start() != nullptr) {
		start()->index = (start - 1)()->index + 1;
		start++;
	}
}
template<class Type> inline lzh::Node<Type>* lzh::LinkList<Type>::PosPtr(const int32 index)
{
	if (size_t(index) > len - 1)return nullptr;
	Node<Type>* ptr = head;
	for (size_t ind = 0; ind <= size_t(index); ++ind)
		ptr = ptr->next;
	return ptr;
}
template<class Type> inline lzh::Node<Type>* lzh::LinkList<Type>::CreatNode(const Type data)
{
	return new Node<Type>(data);
}
template<class Type> inline lzh::Node<Type>* lzh::LinkList<Type>::CreatNode(const Type data, Node<Type>* pre, Node<Type>* next)
{
	Node<Type>* p = new Node<Type>(data);
	if (p == nullptr) {
		fprintf(stderr, "alloc memory fail!");
		throw p;
	}
	pre->next = p;
	p->pre = pre;
	p->next = next;
	return p;
}
template<class Type> inline void lzh::LinkList<Type>::swap(LinkList<Type>& linklist)
{
	std::swap(linklist, *this);
}
template<class Type> inline lzh::LinkIter<Type> lzh::LinkList<Type>::erase(const size_t index, const size_t del_len)
{
	if (index >= len) {
		fprintf(stderr, "error:System.IndexOutOfBoundsException!");
		throw std::exception("error:System.IndexOutOfBoundsException!");
	}
	left = PosPtr(index)->pre;
	iterator Next;
	iterator Start(left);
	iterator iter = Start + 1;
	iterator end = Start + int32(del_len + 1);
	while (iter != end) {
		Next = iter;
		(iter + 1)()->pre = iter()->pre;
		(iter - 1)()->next = (iter + 1)();
		iter += 1;
		delete Next();
		len -= 1;
	}
	UpdateIndex(Start);
	PtrReDire();
	return iterator(left);
}
template<class Type> inline lzh::LinkIter<Type> lzh::LinkList<Type>::erase(const iterator start, const iterator end)
{
	iterator Next;
	iterator iter = start;
	iterator Start = iter - 1;
	while (iter != end) {
		Next = iter;
		(iter + 1)()->pre = iter()->pre;
		(iter - 1)()->next = (iter + 1)();
		iter += 1;
		delete Next();
		len -= 1;
	}
	UpdateIndex(Start);
	return end;
}


template<class _Tp> inline lzh::SeqList<_Tp>::SeqList()
{
	real_ind = 0;
	data_length = 0;
	data = nullptr;
}
template<class _Tp> inline lzh::SeqList<_Tp>::SeqList(const msize len)
{
	real_ind = 0;
	data_length = len;
	data = new _Tp[len];
	CHECK_PTR(data);
}
template<class _Tp> inline lzh::SeqList<_Tp>::SeqList(const SeqList & sl)
{
	real_ind = sl.length();
	data_length = sl.size();
	if (data_length == 0)
		data = nullptr;
	else {
		data = new _Tp[data_length];
		memcpy(data, sl.takeAll(), sizeof(_Tp)*real_ind);
	}
}
template<class _Tp> inline lzh::SeqList<_Tp>::~SeqList()
{
	if (data != nullptr) {
		delete[]data;
		data = nullptr;
	}
}
template<class _Tp> inline void lzh::SeqList<_Tp>::setlength(const msize len)
{
	check(len);
	real_ind = len;
}
template<class _Tp> inline void lzh::SeqList<_Tp>::resetsize(const msize len)
{
	msize temp_length = data_length;
	_Tp *temp_array = data;
	data = nullptr;
	*this = SeqList<_Tp>(len);
	if (temp_array != nullptr) {
		memcpy(data, temp_array, sizeof(_Tp)*temp_length);
		delete[]temp_array;
	}
}
template<class _Tp> inline void lzh::SeqList<_Tp>::append(const _Tp & value)
{
	check(real_ind);
	data[real_ind++] = value;
}
template<class _Tp> inline void lzh::SeqList<_Tp>::append(const SeqList & value)
{
	msize array_size = data_length + value.size();
	msize temp_length = data_length;
	_Tp *temp_array = data;
	CHECK_PTR(temp_array);
	data = nullptr;
	SeqList(array_size);
	memcpy(data, temp_array, sizeof(_Tp)*temp_length);
	memcpy(data + temp_length, value.takeAll(), sizeof(_Tp)*value.size());
	real_ind = array_size;
	delete[]temp_array;
}
template<class _Tp> inline lzh::msize lzh::SeqList<_Tp>::length() const
{
	return real_ind;
}
template<class _Tp> inline lzh::msize lzh::SeqList<_Tp>::size() const
{
	return data_length;
}
template<class _Tp> inline const _Tp * lzh::SeqList<_Tp>::takeAll() const
{
	return data;
}
template<class _Tp> inline const _Tp & lzh::SeqList<_Tp>::takeAt(const msize index) const
{
	check(index);
	return data[index];
}
template<class _Tp> inline _Tp& lzh::SeqList<_Tp>::at(const msize index)const
{
	check(index);
	return data[index];
}
template<class _Tp> inline bool lzh::SeqList<_Tp>::contain(const _Tp & value) const
{
	_FOR(i, 0, real_ind, 1)
		if (data[i] == value)
			return true;
	return false;
}
template<class _Tp> inline lzh::int32 lzh::SeqList<_Tp>::local(const _Tp& value) const
{
	_FOR(i, 0, real_ind, 1)
		if (data[i] == value)
			return (int32)i;
}
template<class _Tp> inline void lzh::SeqList<_Tp>::insert(const msize pos, const _Tp & value)
{
	check(real_ind + 1);
	_FOR(i, real_ind, pos, -1)
		data[i] = data[i - 1];
	data[pos] = value;
}
template<class _Tp> inline _Tp lzh::SeqList<_Tp>::erase(const msize pos)
{
	check(pos);
	_Tp del_value = data[pos];
	_FOR(i, pos, real_ind - 1, 1)
		data[i] = data[i + 1];
	real_ind--;
	return del_value;
}
template<class _Tp> inline lzh::int32 lzh::SeqList<_Tp>::erase(const _Tp & value, bool isAll)
{
	msize pos = -1;
	_FOR(i, 0, real_ind, 1) {
		if (data[i] == value) {
			pos = i;
			_FOR(j, pos, real_ind - 1, 1)
				data[j] = data[j + 1];
			real_ind--;
			if (isAll)
				return (int32)pos;
		}
	}
	return (int32)pos;
}
template<class _Tp> inline lzh::SeqList<_Tp> lzh::SeqList<_Tp>::erase(const msize pos, const msize offset)
{
	check(pos + offset);
	SeqList del_value(offset);
	_FOR(i, pos, pos + offset, 1)
		del_value.append(data[i]);
	_FOR(i, pos, real_ind - offset, 1)
		data[i] = data[i + offset];
	real_ind -= offset;
	return del_value;
}
template<class _Tp> inline void lzh::SeqList<_Tp>::clear()
{
	real_ind = 0;
	_FOR(i, 0, data_length, 1)
		data[i] = _Tp();
}
template<class _Tp> inline void lzh::SeqList<_Tp>::printlist() const
{
	if (real_ind != 0) {
		_FOR(i, 0, real_ind, 1)
			std::cout << data[i];
		std::cout << std::endl;
	}
}
template<class _Tp> inline _Tp & lzh::SeqList<_Tp>::operator[](const int32 pos) const
{
	msize index = msize(pos);
	check(index);
	return data[pos];
}
template<class _Tp> inline void lzh::SeqList<_Tp>::check(const msize ind) const
{
	if (ind >= data_length)
		throw "out of memory!";
}

template<typename type> inline void lzh::reverse(type &array_point, msize pos, msize index)
{
	_FOR(i, 0, (index - pos) / 2 + (index - pos) % 2, 1)
		swap(array_point[pos + i], array_point[index - i]);
}
template<typename type> inline void lzh::Reverse(type &array_point, msize pos, msize index)
{
	reverse<type>(array_point, pos, pos + (index - pos) / 2 - 1);
	reverse<type>(array_point, pos + (index - pos) / 2, index);
	reverse<type>(array_point, pos, index);
}