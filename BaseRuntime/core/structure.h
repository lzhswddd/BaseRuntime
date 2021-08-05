#ifndef __STRUCTURE_H__
#define __STRUCTURE_H__

#include "core.h"
#include "vector.h"

namespace lzh
{
	class DfsMark
	{
	public:
		vec_p2_32s v;
		std::vector<int32> dire;
		std::vector<int32> state;
	};
	template<class Type> class TEMPLATE_API Node
	{
	public:
		using value_type = typename Type;

		Node() : data(), next(nullptr), pre(nullptr) {}
		Node(Type data, Node *next = nullptr, Node * pre = nullptr) : data(data), next(next), pre(pre) {}

		size_t index;
		Type data;
		Node *next;
		Node *pre;
	};
	template<class Type> class TEMPLATE_API LinkIter :
		public std::iterator<std::random_access_iterator_tag, Type>
	{
	public:
		using value_type = typename Type;

		LinkIter(Node<Type>* p = nullptr);
		const LinkIter<Type> operator = (const LinkIter<Type> &iter);
		int32 operator - (const LinkIter<Type> iter)const;
		int32 operator + (const LinkIter<Type> iter)const;
		bool operator != (const LinkIter<Type> &iter)const;
		bool operator == (const LinkIter<Type> &iter)const;
		bool operator > (const LinkIter<Type> &iter);
		bool operator < (const LinkIter<Type> &iter);
		const LinkIter<Type> operator ++ ();
		const LinkIter<Type> operator ++ (int32);
		const LinkIter<Type> operator + (int32 count);
		const LinkIter<Type> operator += (int32 count);
		const LinkIter<Type> operator -- ();
		const LinkIter<Type> operator -- (int32);
		const LinkIter<Type> operator - (int32 count);
		const LinkIter<Type> operator -= (int32 count);
		Type& operator * ()const;
		Node<Type>* operator ()()const;
	private:
		Node<Type>* _ptr;
	};
	template<class Type> class TEMPLATE_API LinkList
	{
	public:
		using value_type = typename Type;

		typedef LinkIter<Type> iterator;
		LinkList();
		LinkList(const int32 list_length);
		LinkList(LinkList &linklist);
		~LinkList();

		bool empty();
		int32 length();
		size_t size();

		iterator erase(const size_t index, const size_t del_len = 1);
		iterator erase(const iterator start, const iterator end);
		void swap(LinkList<Type> &linklist);
		void insert(iterator start, const iterator copy_start, const iterator copy_end);
		void insert(const Type source, const size_t index = 0);
		void insert(LinkList<Type> &list, const size_t index = 0, const size_t copy_start = 0, const size_t copy_end = 1);
		void append(Type source);
		void push_back(Type source);
		void print();
		void clear();
		iterator begin();
		iterator end();
		Type& operator [](const int32 index);
		static Node<Type>* CreatNode(const Type data);
		static Node<Type>* CreatNode(const Type data, Node<Type>* pre, Node<Type>* next);
	protected:
		void initPtr();
		void PtrReDire();
		void UpdateIndex(const size_t index = 0);
		void UpdateIndex(iterator start);
		Node<Type>* PosPtr(const int32 index);
	private:
		size_t len;
		Node<Type>* head;
		Node<Type>* tail;

		Node<Type>* left;
		Node<Type>* right;
	};
	template<class _Tp> class TEMPLATE_API SeqList
	{
	public:
		using value_type = typename _Tp;

		SeqList();
		SeqList(const msize len);
		SeqList(const SeqList &sl);
		~SeqList();
		void setlength(const msize len);
		void resetsize(const msize len);
		void append(const _Tp& value);
		void append(const SeqList& value);
		msize length()const;
		msize size()const;
		const _Tp* takeAll()const;
		const _Tp& takeAt(const msize index)const;
		_Tp& at(const msize index)const;
		bool contain(const _Tp& value)const;
		int32 local(const _Tp& value)const;
		void insert(const msize pos, const _Tp& value);
		_Tp erase(const msize pos);
		int32 erase(const _Tp& value, bool isAll = false);
		SeqList<_Tp> erase(const msize pos, const msize offset);
		void clear();
		void printlist()const;
		_Tp& operator [](const int32 pos)const;

	protected:
		void check(const msize ind)const;

	private:
		msize real_ind;
		msize data_length;
		_Tp *data;
	};
	class LargeNumber : public std::string
	{
	public:
		explicit LargeNumber();
		LargeNumber(int8 ch);
		LargeNumber(uint8 val);
		LargeNumber(int32 val);
		LargeNumber(int64 val);
		LargeNumber(uint32 val);
		LargeNumber(uint64 val);
		LargeNumber(const int8 *str);
		LargeNumber(std::string str);
		LargeNumber(int32 *ptr, int32 n);

		static int32 SubStract(int32 *p1, int32 *p2, int32 len1, int32 len2);
		void show()const;
		void show(std::ostream &)const;
	};
	class LZHAPI LargeN
	{
	public:
		explicit LargeN();
		LargeN(int8 ch);
		LargeN(uint8 val);
		LargeN(int32 val);
		LargeN(int64 val);
		LargeN(uint32 val);
		LargeN(uint64 val);
		LargeN(const int8 *str);
		LargeN(std::string str);
		LargeN(int32 *ptr, int32 n);
		LargeN(const LargeN &largeNumber);
		~LargeN();
		LargeNumber& operator = (const LargeNumber& largenumber);
		LargeNumber& operator = (const LargeN& largenumber);
		operator LargeNumber& () { return *ln; }
		operator const LargeNumber& ()const { return *ln; }
	private:
		LargeNumber *ln;
	};

	template<typename type> void reverse(type &array_point, msize pos, msize index);
	template<typename type> void Reverse(type &array_point, msize pos, msize index);

	extern LZHAPI const LargeNumber operator -(const LargeNumber& a);

	extern LZHAPI bool operator <(const LargeNumber& a, const LargeNumber& b);
	extern LZHAPI bool operator >(const LargeNumber& a, const LargeNumber& b);
	extern LZHAPI bool operator <=(const LargeNumber& a, const LargeNumber& b);
	extern LZHAPI bool operator >=(const LargeNumber& a, const LargeNumber& b);
	extern LZHAPI bool operator ==(const LargeNumber& a, const LargeNumber& b);

	extern LZHAPI void operator += (LargeNumber& a, const LargeNumber& b);
	extern LZHAPI void operator -= (LargeNumber& a, const LargeNumber& b);
	extern LZHAPI void operator *= (LargeNumber& a, const LargeNumber& b);
	extern LZHAPI void operator /= (LargeNumber& a, const LargeNumber& b);

	extern LZHAPI const LargeNumber operator +(const LargeNumber& a, const LargeNumber& b);
	extern LZHAPI const LargeNumber operator -(const LargeNumber& a, const LargeNumber& b);
	extern LZHAPI const LargeNumber operator *(const LargeNumber& a, const LargeNumber& b);
	extern LZHAPI const LargeNumber operator /(const LargeNumber& a, const LargeNumber& b);
}
#include "structure_inl.hpp"
#endif // !__STRUCTURE_H__
