#pragma once

//������clrs 10.2-8��Ŀ�Ĵ���
template<typename T> struct listnode_np
{
	listnode_np<T>* np; //npָ��ֻ����ǰһλ�ͺ�һλ��������ĵ�ַ�йأ�������listnode�ĵ�npֵ�޹ء���һָ��ֻ�������ǰ���������һ�����ı䣬���Ҳ���Ӱ�������������������ԣ�����ʹ�ñ�׼�������ָ������޷�����ͨ������Ϊ����������ʱ��Ҫ�Ƚ�����ָ���Ϊ��ָͨ�룬�������Ҫ���ȥ����д����������ʱ��������ͶԲ��ϡ�
	T element;
	listnode_np<T>() = default;
	listnode_np<T>(listnode_np<T>* ptr, T elem) : np(ptr), element(elem) {};
	listnode_np<T>* operator = (listnode_np<T>* a)
	{
		np = a->np;
		element = a->element;
		return this;
	}
	~listnode_np()
	{
		delete np;
	}
};
template<typename T> class list_np //ֻʹ��һ��ָ����ͬʱ����np��prev��next��Ϣ����һ����ʱ�任�ռ�Ĳ���
{
	template<typename U> friend listnode_np<T>* my_xor(const listnode_np<T>* left, const listnode_np<T>* right);
	template<typename U> friend listnode_np<T>* prev(const listnode_np<T>* left, const listnode_np<T>* right);
	template<typename U> friend listnode_np<T>* next(const listnode_np<T>* left, const listnode_np<T>* right);
private:
	listnode_np<T>* head;
	listnode_np<T>*	tail;
	listnode_np<T>* null;
	int size;
public:
	list_np() = default;
	void init()
	{
		head = new listnode_np<T>();
		tail = new listnode_np<T>();
		null = new listnode_np<T>();
		size = 0;
		head->np = my_xor(null, tail);
		tail->np = my_xor(null, head);
	}
	void copy(list_np l)
	{
		head = l.head;
		tail = l.tail;
		size = l.size;
		null = l.null;
	}
	void insert_end(T elem); //�ڶ��е�������Ԫ��
	void insert_begin(T elem); //�ڶ��е�ǰ�˲���Ԫ�أ�ԭ�����ڶ��к����һ��
	void insert_before(T elem, int index); //��indexָʾλ�õ�ǰ�˲���Ԫ��
	T get(int index); //����ĳһλ�õ�Ԫ��
	T delete_begin(); //ɾ����һ��Ԫ��
	T delete_end();
	T delete_pos(int index);
	void print(std::ostream& os);
	void reverse();
};
template<typename T> listnode_np<T>* my_xor(const listnode_np<T>* left, const listnode_np<T>* right)//ptr���Ͳ���ֱ����������㣬���Ҫ�Ƚ�ָ��ת�����ͣ��ñ�������Ϊ���ڲ���unsigned���͵�ָ�벢�����Դ��������������������֮���ٽ�����תΪptr
{
	return (listnode_np<T>*)((unsigned long(left)) ^ (unsigned long(right))); //ת��unsigned long����Ϊc++��unsigned long��ָ�����͵��ֽ������
}
template<typename T> listnode_np<T>* next(const listnode_np<T>* left, const listnode_np<T>* curr) //ͨ��curr��curr��ߵ�ָ��Ѱ��curr�ұߵ�ָ��
{
	return my_xor(curr->np, left);
}
template<typename T> listnode_np<T>* prev(const listnode_np<T>* curr, const listnode_np<T>* right)//ͨ��curr��curr�ұߵ�ָ��Ѱ��curr��ߵ�ָ�롣�Ժ�ע��������Ҫ���ʹ�õı��ʽ����ר��Ϊ����дһ����������������д����Ҳ�����޸ĳ��򣬶�������ÿһ��ʹ�õ�ʱ��дһ���ر��ӵı��ʽ��
{
	return my_xor(curr->np, right);
}
/*
�����Ҳ����ֱ��ʹ��ģ�����ͣ�������ģ���Լ�ȥ�ж�T������ʲô���ȽϷ���

�����������ඨ���У�
template<typename U> friend U my_xor(const U& left, const U& right);
template<typename U> friend U prev(const U& left, const U& right);
template<typename U> friend U next(const U& left, const U& right);

�����������ඨ�����棺
template<typename T> T my_xor(const T& left, const T& right)
{
return (T)((unsigned long(left)) ^ (unsigned long(right)));
}
template<typename T> T next(const T& left, const T& curr)
{
return my_xor(curr->np, left);
}
template<typename T> T prev(const T& curr, const T& right)
{
return my_xor(curr->np, right);
}
*/
template<typename T> void list_np<T>::insert_end(T elem) //�ڶ��е�������Ԫ��
{
	listnode_np<T>* temp = new listnode_np<T>();
	temp->np = my_xor(tail, prev(tail, null)); //����temp��ָ��
	temp->element = elem;
	prev(tail, null)->np = my_xor(temp, prev(prev(tail, null), tail)); //�����֮ǰtailǰ���Ѿ���Ԫ�أ�����Ҫ�޸���ǰ���Ǹ�Ԫ�ص�ָ�롣(((tail->np) ^ NULL)->np)�����޸�֮ǰtailǰ���Ԫ�أ�((((tail->np) ^ NULL)->np) ^ tail)�����޸�ǰtailǰ������2��Ԫ��
	tail->np = my_xor(null, temp);
	size++;
}
template<typename T> void list_np<T>::insert_begin(T elem) //�ڶ��е�ǰ�˲���Ԫ�أ�ԭ�����ڶ��к����һ��
{
	listnode_np<T>* temp = new listnode_np<T>();
	temp->np = my_xor(head, next(null, head));
	temp->element = elem;
	next(null, head)->np = my_xor(temp, next(head, next(null, head))); //�����֮ǰhead�����Ѿ���Ԫ�أ�����Ҫ�޸��������Ǹ�Ԫ�ص�ָ��
	head->np = my_xor(null, temp);
	size++;
}
template<typename T> void list_np<T>::insert_before(T elem, int index) //��indexָʾλ�õ�ǰ�˲���Ԫ��
{
	listnode_np<T>* temp = new listnode_np<T>();
	temp->element = elem;
	listnode_np<T>* ptr = head;
	listnode_np<T>* tempptr = ptr;
	listnode_np<T>* ptrb = null; //����ptr�ĺ���һ��Ԫ��
	for (int i = 0;i <= index;i++)
	{
		tempptr = ptr;
		ptr = next(ptrb, ptr); //��ptr��ǰ�ƶ�һλ
		ptrb = tempptr; //��ptrb��ǰ�ƶ�һλ��ÿ��ѭ������֮��ptrb��tempptr��ָ��Ԫ����ͬ
	}
	temp->np = my_xor(ptrb, ptr);
	ptrb->np = my_xor(temp, prev(ptrb, ptr));
	ptr->np = my_xor(temp, next(tempptr, ptr));
	size++;
}
template<typename T>T list_np<T>::get(int index) //����ĳһλ�õ�Ԫ��
{
	listnode_np<T>* ptr = head; //�����ͷλ��
	listnode_np<T>* ptrb = null; //����ptr�ĺ���һ��Ԫ��
	for (size_t i = 0;i <= index;i++)
	{
		auto tempptr = ptr;
		ptr = next(ptrb, ptr); //��ptr��ǰ�ƶ�һλ
		ptrb = tempptr; //��ptrb��ǰ�ƶ�һλ
	}
	return ptr->element;
}
template<typename T> T list_np<T>::delete_begin() //ɾ����һ��Ԫ��
{
	if (size > 0)
	{
		listnode_np<T>* ptr = next(null, head); //�����һ��Ԫ�أ�����ɾ��Ԫ��
		next(head, ptr)->np = my_xor(head, next(ptr, next(head, ptr)));//((ptr->np)^head)��ptr����ǰһ��Ԫ�أ�((((ptr->np) ^ head)->np)^ptr)��ptr��ǰ����Ԫ��
		head->np = my_xor(null, next(head, ptr));
		size--;
		return ptr->element;
	}
	return -1;
}
template<typename T> T list_np<T>::delete_end() //ɾ�����һ��Ԫ��
{
	if (size > 0)
	{
		listnode_np<T>* ptr = prev(tail, null); //�������һ��Ԫ�أ���ɾ��Ԫ��
		prev(tail, null)->np = my_xor(tail, prev(prev(ptr, tail), tail));//((ptr->np)^tail)��ptr�����һ��Ԫ�أ�((((ptr->np) ^ tail)->np)^ptr)��ptr�������Ԫ��
		tail->np = my_xor(null, prev(tail, null));
		size--;
		return ptr->element;
	}
	return -1;
}
template<typename T> T list_np<T>::delete_pos(int index) //ɾ��indexָʾ��Ԫ��
{
	if (size > 0)
	{
		listnode_np<T>* ptr = next(null, head); //�����һ��Ԫ�أ�ѭ��֮���ΪҪɾ����Ԫ��
		listnode_np<T>* tempptr;
		listnode_np<T>* ptrb = head; //����head����ptr�ĺ���һ��Ԫ��
		for (int i = 0;i < index;i++)
		{
			tempptr = ptr;
			ptr = next(ptrb, ptr); //��ptr��ǰ�ƶ�һλ
			ptrb = tempptr; //��ptrb��ǰ�ƶ�һλ��ÿ��ѭ������֮��ptrb��tempptr��ָ��Ԫ����ͬ
		}
		next(ptrb, ptr)->np = my_xor(ptrb, next(ptr, next(ptrb, ptr)));//((ptr->np) ^ ptrb)��ptr��ǰһ��Ԫ�أ�((((ptr->np) ^ ptrb)->np) ^ ptr)��ptr��ǰ����Ԫ��
		ptrb->np = my_xor(prev(ptrb, ptr), next(ptrb, ptr)); //((ptrb->np) ^ ptr)��ptr�������Ԫ��
		size--;
		return ptr->element;
	}
	return -1;
}
template<typename T> void list_np<T>::print(std::ostream& os)
{
	listnode_np<T>* ptr = head; //�����ͷλ��
	listnode_np<T>* ptrb = null; //����ptr�ĺ���һ��Ԫ��
	for (size_t i = 0;i < size;i++)
	{
		auto tempptr = ptr;
		ptr = next(ptrb, ptr); //��ptr��ǰ�ƶ�һλ
		ptrb = tempptr; //��ptrb��ǰ�ƶ�һλ
		os << ptr->element << '\t';
	}
	os << std::endl;
}
template<typename T> void list_np<T>::reverse() //Ҫ������ת��ֻ��Ҫ��ͷβ��ָ���������
{
	listnode_np<T>* temp = head;
	head = tail;
	tail = temp;
}