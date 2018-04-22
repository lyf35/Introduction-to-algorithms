#pragma once
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<algorithm>
#include<cmath>
#include<memory>

template<typename T> class stack
{
private:
	int max_number;
	std::unique_ptr<T[]> element;
	int current;
public:
	stack(int n) :max_number(n), element(new T[n]())
	{
		current = -1;
	};
	void push(const T a)
	{
		if (check_full())
		{
			std::cout << "The stack is already full." << std::endl;
		}
		else
		{
			current++;
			element[current] = a;
		}
	}
	bool check_full()
	{
		if (current == max_number - 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	T pop()
	{
		if (check_empty())
		{
			std::cerr << "The stack is empty." << std::endl;
			return 0;
		}
		else
		{
			T temp = element[current];
			element[current] = 0;
			current--;
			return temp;
		}
	}
	bool check_empty()
	{
		if (current == -1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	void output_elements(std::ostream& os)
	{
		for (int i = 0;i <= current;i++)
		{
			os << element[i] << '\t';
		}
		os << std::endl;
	}
};

template<typename T> class queue
{
private:
	int max_number;
	std::unique_ptr<T[]> element;
	int head;
	int tail;
	bool full;
public:
	queue(int n) :max_number(n), element(new T[n]())
	{
		head = tail = 0;
	};
	void push(const T a)
	{
		if (full)
		{
			std::cout << "The queue is already full." << std::endl;
		}
		else
		{
			element[tail] = a;
			tail++;
			if (tail >= max_number)
			{
				tail = tail%max_number;
			}
			if (head == tail)
			{
				full = 1;
			}
		}
	}
	T pop()
	{
		if (check_empty())
		{
			std::cerr << "The queue is empty." << std::endl;
			return 0;
		}
		else
		{
			T temp = element[head];
			element[head] = 0;
			head--;
			if (head < 0)
			{
				head = head + max_number;
			}
			if (full == 1)
			{
				full = 0;
			}
			return temp;
		}
	}
	bool check_empty()
	{
		if (head == tail&&full == 0)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	void output_elements(std::ostream& os)
	{
		for (int i = 0;i < max_number;i++)
		{
			os << element[i] << '\t';
		}
		os << std::endl;
	}
};

enum status{ double_link,circular_link };
template<typename T> struct listnode
{
	std::shared_ptr<listnode<T>> prev;
	std::shared_ptr<listnode<T>> next;
	T element;
	listnode<T>() = default;
	listnode<T>(std::shared_ptr<listnode<T>> before, std::shared_ptr<listnode<T>> after, T elem) : prev(before), next(after), element(elem) {};
};

template<typename T> class List
{
private:
	int size;
	std::shared_ptr<listnode<T>> head;
	std::shared_ptr<listnode<T>> tail;
	status link;
public:
	std::shared_ptr<listnode<T>> get_end()
	{
		return tail->prev;
	}
	std::shared_ptr<listnode<T>> get_beg()
	{
		return head->next;
	}
	std::shared_ptr<listnode<T>> get_tail()
	{
		return tail;
	}
	std::shared_ptr<listnode<T>> get_head()
	{
		return head;
	}
	List(status stat = double_link)
	{
		head = std::make_shared<listnode<T>>();
		if (link == circular_link) //����ǻ����б�Ļ�����ô����ָ����β��tailָ��Ϳ��Բ��ã���ʼ��ʱֱ����prev��next��ָ�붼ָ���Լ��γɻ�·
		{
			head->prev = head;
			head->next = head;
		}
		if (link == double_link) //������ǻ����б�head��tailָ�붼Ҫʹ��
		{
			tail = std::make_shared<listnode<T>>();
			head->next = tail;
			tail->prev = head;
			size = 0;
		}
	};
	void copy(List c)  
	{
		head(c.head);
		tail(c.tail);
		size(c.size);
		link(c.link);
	}
	void insert_begin(T elem)
	{
		std::shared_ptr<listnode<T>> temp = std::make_shared<listnode<T>>(listnode<T>(head, head->next, elem));
		head->next->prev = temp;
		head->next = temp;
		size++;
	}
	void insert_end(T elem)
	{
		std::shared_ptr<listnode<T>> temp = std::make_shared<listnode<T>>(listnode<T>(tail->prev, tail, elem));
		tail->prev->next = temp;
		tail->prev = temp;
		size++;
	}
	void insert_before(T elem, int pos)
	{
		std::shared_ptr<listnode<T>> temp(head);
		for (int i = 0;i < pos;i++)
		{
			temp = temp->next;
		}
		std::shared_ptr<listnode<T>> newnode = std::make_shared<listnode<T>>(listnode<T>(temp->next, tail->prev, elem));
		temp->next->prev = newnode;
		temp->next = newnode;
		size++;
	}
	listnode<T> operator[](int pos) const
	{
		std::shared_ptr<listnode<T>> temp(head);
		for (int i = 0;i <= pos;i++)
		{
			temp = temp->next;
		}
		return *temp;
	}
	T delete_begin()
	{
		auto temp = head->next;
		temp->next->prev = head;
		head->next = temp->next;
		size--;
		return temp->element;
	}
	T delete_end()
	{
		auto temp = tail->prev;
		tail->prev = temp->prev;
		temp->prev->next = tail;
		size--;
		return temp->element;
	}
	T delete_pos(int pos)
	{
		auto temp(head);
		for (int i = 0;i <= pos;i++)
		{
			temp = temp->next;
		}
		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;
		size--;
		return temp->element;
	}
	void delete_node(listnode<T> node)
	{
		node.prev = node.next->prev;
		node.next = node.prev->next;
		size--;
	}
	void delete_element(T elem)
	{
		int pos = search(elem);
		if (pos >= 0)
		{
			delete_node((*this)[pos]);
		}
	}
	void print(std::ostream& os) const
	{
		for (int i = 0;i < size;i++)
		{
			os << (*this)[i].element << '\t';
		}
		os << std::endl;
	}
	int get_size() const
	{
		return size;
	}
	int search(T elem) const
	{
		for (int i = 0;i < size;i++)
		{
			if ((*this)[i].element == elem)
			{
				return i;
			}
		}
		return -1;
	}
	void reverse() //�������Ԫ��˳��ת
	{
		std::shared_ptr<listnode<T>> temp = head->next;
		for (int i = 0;i < size;i++)
		{
			swap(temp->prev, temp->next);
			temp = temp->prev;
		}
		swap(head->next, tail->prev);
	}
};

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
template<typename T> listnode_np<T>* my_xor(const listnode_np<T>* left, const listnode_np<T>* right)//ptr���Ͳ���ֱ����������㣬���Ҫ�Ƚ�ָ��ת�����ͣ��ñ�������Ϊ���ڲ���unsigned���͵�ָ�벢�����Դ��������������������֮���ٽ�����תΪptr��
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
 //�������ṹ�Ĵ���
template<typename T> struct BinarytreeNode
{
	std::shared_ptr<BinarytreeNode<T>> parent;
	std::shared_ptr<BinarytreeNode<T>> leftchild;
	std::shared_ptr<BinarytreeNode<T>> rightchild;
	T element;
	int height;
	BinarytreeNode(std::shared_ptr<BinarytreeNode<T>> p = NULL, std::shared_ptr<BinarytreeNode<T>> lc = NULL, std::shared_ptr<BinarytreeNode<T>> rc = NULL, T elem = 0, int h = 0) :parent(p), leftchild(lc), rightchild(rc), element(elem), height(h) {};
};
template<typename T> class Binarytree
{
private:
	std::shared_ptr<BinarytreeNode<T>> root;
public:
	Binarytree(std::shared_ptr<BinarytreeNode<T>> node) :root(node) {};
	Binarytree(T elem)
	{
		root = std::shared_ptr<BinarytreeNode<T>>(new BinarytreeNode<T>(NULL, NULL, NULL, elem, 0));
	}
	std::shared_ptr<BinarytreeNode<T>> visit_root()
	{
		return root;
	}
	void updateheightabove(std::shared_ptr<BinarytreeNode<T>> node) //ֻ����������ߣ�������ʲô��������Ҫ���������ɵ������ĺ��������ж�
	{
		int i = node->height;
		while (node = node->parent)
		{
			if (node->height <= i)
			{
				node->height = ++i;
			}
			else
			{
				break;
			}
		}
	}
	std::shared_ptr<BinarytreeNode<T>> insert_leftchild(std::shared_ptr<BinarytreeNode<T>> node, std::shared_ptr<BinarytreeNode<T>> lc)
	{
		node->leftchild = lc;
		lc->parent = node;
		if (node->height <= lc->height)
		{
			updateheightabove(lc);
		}
		return lc;
	}
	std::shared_ptr<BinarytreeNode<T>> insert_rightchild(std::shared_ptr<BinarytreeNode<T>> node, std::shared_ptr<BinarytreeNode<T>> rc)
	{
		node->rightchild = rc;
		rc->parent = node;
		if (node->height <= rc->height)
		{
			updateheightabove(rc);
		}
		return rc;
	}
	std::shared_ptr<BinarytreeNode<T>> find_leftchild(std::shared_ptr<BinarytreeNode<T>> node)
	{
		return node->leftchild;
	}
	std::shared_ptr<BinarytreeNode<T>> find_rightchild(std::shared_ptr<BinarytreeNode<T>> node)
	{
		return node->rightchild;
	}
	std::shared_ptr<BinarytreeNode<T>> find_parent(std::shared_ptr<BinarytreeNode<T>> node)
	{
		return node->parent;
	}
	std::shared_ptr<BinarytreeNode<T>> delete_leftchild(std::shared_ptr<BinarytreeNode<T>> node)
	{
		auto temp = node->leftchild;
		node->leftchild = NULL;
		if (temp)
		{
			temp->parent = NULL;
		}
		if (node->rightchild&&node->height > node->rightchild->height + 1)
		{
			updateheightabove(node->rightchild);
		}
		else if (!node->rightchild)
		{
			node->height = 0;
			updateheightabove(node);
		}
		return temp;
	}
	std::shared_ptr<BinarytreeNode<T>> delete_rightchild(std::shared_ptr<BinarytreeNode<T>> node)
	{
		auto temp = node->rightchild;
		node->rightchild = NULL;
		if (temp)
		{
			temp->parent = NULL;
		}
		if (node->leftchild&&node->height > node->leftchild->height + 1)
		{
			updateheightabove(node->leftchild);
		}
		else if(!node->leftchild)
		{
			node->height = 0;
			updateheightabove(node);
		}
		return temp;
	}
	std::shared_ptr<BinarytreeNode<T>> delete_node(std::shared_ptr<BinarytreeNode<T>> node)
	{
		auto temp = node->parent;
		if (node != visit_root() && node->parent->leftchild)
		{
			node->parent->leftchild = NULL;
			node->parent = NULL;
			updateheightabove(temp->rightchild);
		}
		if (node != visit_root() && node->parent->rightchild)
		{
			node->parent->rightchild = NULL;
			node->parent = NULL;
			updateheightabove(temp->leftchild);
		}
		return node;
	}
	//�ݹ���ʽ�����ֱ���
	template<typename U> void preorder_traverse_recursive (std::shared_ptr<BinarytreeNode<T>> node, U op)//U���������⺯�������Ǿ��У������������
	{
		op(node->element);
		if (node->leftchild.get())
		{
			preorder_traverse_recursive(node->leftchild, op);
		}
		if (node->rightchild.get())
		{
			preorder_traverse_recursive(node->rightchild, op);
		}
	}
	template<typename U> void inorder_traverse_recursive(std::shared_ptr<BinarytreeNode<T>> node, U op)
	{
		if (node->leftchild.get())
		{
			inorder_traverse_recursive(node->leftchild, op);
		}
		op(node->element);
		if (node->rightchild.get())
		{
			inorder_traverse_recursive(node->rightchild, op);
		}
	}
	template<typename U> void postorder_traverse_recursive(std::shared_ptr<BinarytreeNode<T>> node, U op)
	{
		if (node->leftchild.get())
		{
			postorder_traverse_recursive(node->leftchild, op);
		}
		if (node->rightchild.get())
		{
			postorder_traverse_recursive(node->rightchild, op);
		}
		op(node->element);
	}
	//�����Ƿǵݹ���ʽ�����ֱ���
	template<typename U> void preorder_traverse_nonrecursive(std::shared_ptr<BinarytreeNode<T>> node, U op)
	{
		List<std::shared_ptr<BinarytreeNode<T>>> container; //ֻʹ��list��insert_end��delete_end��������ʵ�൱������ʹ��ջ������ͬ��
		container.insert_end(NULL);
		while (node)
		{
			op(node->element);
			if (find_rightchild(node).get())
			{
				container.insert_end(find_rightchild(node));
			}
			if (!(node = find_leftchild(node)).get())
			{
				node = container.delete_end();
			}
		}
	}
	void leftmost_trace(std::shared_ptr<BinarytreeNode<T>> node, List<std::shared_ptr<BinarytreeNode<T>>>& container)
	{
		while (node)
		{
			container.insert_end(node);
			node = find_leftchild(node);
		}
	}
	template<typename U> void inorder_traverse_nonrecursive(std::shared_ptr<BinarytreeNode<T>> node, U op)
	{
		List<std::shared_ptr<BinarytreeNode<T>>> container;
		container.insert_end(NULL);
		leftmost_trace(node, container);
		while (true) //һ��Ҫע�⣬һ��һֱ�������������������֧·Ҫ��֤������������ֻ����һ�飡
		{
			node = container.delete_end();
			if (node != NULL)
			{
				op(node->element);
			}
			else
			{
				break;
			}
			if (find_rightchild(node))
			{
				leftmost_trace(find_rightchild(node), container);
			}
		}
	}
	std::shared_ptr<BinarytreeNode<T>> leftmost_trace_v2(std::shared_ptr<BinarytreeNode<T>> node)
	{
		while (find_leftchild(node))
		{
			node = find_leftchild(node);
		}
		return node;
	}
	template<typename U> void inorder_traverse_nonrecursive_v2(std::shared_ptr<BinarytreeNode<T>> node, U op) //��ʹ��ջ��Ϊ��ʱ�洢�ĵݹ�
	{
		node = leftmost_trace_v2(node); //���������������ӽڵ㿪ʼ����
		while(node) //ֻҪnode���ǿս���һֱѭ����ȥ
		{
			op(node->element);
			if (find_rightchild(node)) //���node������������ֱ������������������ڵ㿪ʼ����
			{
				node = leftmost_trace_v2(find_rightchild(node)); 
			}
			else //���nodeû������������ô��Ҫ����
			{
				while (find_parent(node)->rightchild == node)
				{
					node = find_parent(node); //���node���Һ��ӣ�Ҫһֱ����׷��
					if (node == visit_root())
					{
						return;
					}
				}
				node = find_parent(node); //�˳���һ��whileѭ����ʱ��node�Ѿ������ӣ���ʱ���nodeһ�����ʹ������������׷��һ��
			}
		}
	}
	void insert_container(std::shared_ptr<BinarytreeNode<T>> p, List<std::shared_ptr<BinarytreeNode<T>>>& container)
	{
		while(p)
		{
			if (find_leftchild(p))
			{
				if (find_rightchild(p))
				{
					container.insert_end(find_rightchild(p));
				}
				container.insert_end(p = find_leftchild(p));
			}
			else
			{
				container.insert_end(p = find_rightchild(p));
			}
		}
		container.delete_end();
	}
	template<typename U> void postorder_traverse_nonrecursive(std::shared_ptr<BinarytreeNode<T>> node, U op)
	{
		List<std::shared_ptr<BinarytreeNode<T>>> container;
		container.insert_end(node);
		while (container.get_size())
		{
			if (container.get_end()->element != node->parent) //��ջ���ڵ㲻�ǵ�ǰ���ĸ��ף�����Ҫ��ջ���ڵ�����λ����������Ҳ����˵����ʱջ���ڵ����ҽڵ㣬����ǰ�ڵ���������ڵ�
			{
				insert_container(container.get_end()->element, container);
			}
			node = container.delete_end();
			op(node->element);
		}
	}
	template<typename U> void level_traverse(std::shared_ptr<BinarytreeNode<T>> node, U op)
	{
		List<std::shared_ptr<BinarytreeNode<T>>> container;
		container.insert_begin(node);
		while (container.get_beg() != container.get_tail())
		{
			node = container.delete_end();
			op(node->element);
			if (node->leftchild)
			{
				container.insert_begin(node->leftchild);
			}
			if (node->rightchild)
			{
				container.insert_begin(node->rightchild);
			}
		}
	}

};
//�������ֵܱ�ʾ������и����ṹ����
template<typename T> struct TreeNode
{
	T value;
	std::shared_ptr<TreeNode<T>> parent;
	std::shared_ptr<TreeNode<T>> leftchild;
	std::shared_ptr<TreeNode<T>> sibling;
	TreeNode(T num) :value(num), parent(NULL), leftchild(NULL), sibling(NULL) {};
};
template<typename T> class Tree
{
private:
	std::shared_ptr<TreeNode<T>> root;
public:
	Tree(T num) :root(std::shared_ptr<TreeNode<T>>(new TreeNode<T>(num))) {};
	std::shared_ptr<TreeNode<T>> visit_root()
	{
		return root;
	}
	std::shared_ptr<TreeNode<T>> get_leftchild(std::shared_ptr<TreeNode<T>> n)
	{
		return n->leftchild;
	}
	std::shared_ptr<TreeNode<T>> get_rightsibling(std::shared_ptr<TreeNode<T>> n)
	{
		return n->sibling;
	}
	std::shared_ptr<TreeNode<T>> get_parent(std::shared_ptr<TreeNode<T>> n)
	{
		return n->parent;
	}
	std::shared_ptr<TreeNode<T>> insert_leftchild(std::shared_ptr<TreeNode<T>> node, std::shared_ptr<TreeNode<T>> insert)
	{
		node->leftchild = insert;
		insert->parent = node;
		return insert;
	}
	std::shared_ptr<TreeNode<T>> insert_sibling(std::shared_ptr<TreeNode<T>> node, std::shared_ptr<TreeNode<T>> insert)
	{
		node->sibling = insert;
		insert->parent = node->parent;
		return insert;
	}
	std::shared_ptr<TreeNode<T>> delete_leftchild(std::shared_ptr<TreeNode<T>> node)
	{
		auto temp = node->leftchild;
		temp->parent = NULL;
		node->leftchild = NULL;
		return temp;
	}
	std::shared_ptr<TreeNode<T>> delete_sibling(std::shared_ptr<TreeNode<T>> node)
	{
		auto temp = node->sibling;
		temp->parent = NULL;
		node->sibling = NULL;
		return temp;
	}
	template<typename U> void layer_traverse(std::shared_ptr<TreeNode<T>> node, U op) //���ղ��˳������Ԫ�����
	{
		List<std::shared_ptr<TreeNode<T>>> container; //ʹ��һ��list��Ϊ��������������ʽ��
		container.insert_begin(node);
		while (container.get_size()) //ֻҪ��������Ԫ�ؾ�һֱѭ����ȥ
		{
			std::shared_ptr<TreeNode<T>> temp = container.delete_end();
			op(temp->value);
			if (temp->leftchild)
			{
				container.insert_begin(temp = temp->leftchild);
				while (temp->sibling)
				{
					container.insert_begin(temp = temp->sibling);
				}
			}
		}
	}
};
/*
����ʹ�õ����������룺
Binarytree<int> test(1);
auto temp = test.insert_leftchild(test.visit_root(), std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 2)));
auto temp2 = test.insert_rightchild(test.visit_root(), std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 3)));
test.insert_leftchild(temp, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 4)));
test.insert_rightchild(temp, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 5)));
test.insert_leftchild(temp2, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 6)));
test.insert_rightchild(temp2, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 7)));
temp = test.find_leftchild(temp);
test.insert_leftchild(temp, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 8)));
test.insert_rightchild(temp, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 9)));
temp = test.find_rightchild(test.find_parent(temp));
test.insert_leftchild(temp, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 10)));
test.insert_rightchild(temp, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 11)));
temp2 = test.find_leftchild(temp2);
test.insert_leftchild(temp2, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 12)));
test.insert_rightchild(temp2, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 13)));
temp2 = test.find_rightchild(test.find_parent(temp2));
test.insert_leftchild(temp2, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 14)));
test.insert_rightchild(temp2, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 15)));
visit<int> my_visit;
test.preorder_traverse_recursive(test.visit_root(), my_visit);
std::cout << "success!" << std::endl;
test.preorder_traverse_nonrecursive(test.visit_root(), my_visit);
std::cout << "success!" << std::endl;
test.inorder_traverse_recursive(test.visit_root(), my_visit);
std::cout << "success!" << std::endl;

test.inorder_traverse_nonrecursive(test.visit_root(), my_visit);
std::cout << "success!" << std::endl;
test.inorder_traverse_nonrecursive_v2(test.visit_root(), my_visit);
test.postorder_traverse_recursive(test.visit_root(), my_visit);
std::cout << "success!" << std::endl;

test.postorder_traverse_nonrecursive(test.visit_root(), my_visit);
std::cout << "success!" << std::endl;

test.level_traverse(test.visit_root(), my_visit);
std::cout << "success!" << std::endl;


Tree<int> test(1);
auto temp = test.insert_leftchild(test.visit_root(), std::shared_ptr<TreeNode<int>>(new TreeNode<int>(2)));
temp = test.insert_sibling(temp, std::shared_ptr<TreeNode<int>>(new TreeNode<int>(3)));
auto temp2 = test.insert_sibling(temp, std::shared_ptr<TreeNode<int>>(new TreeNode<int>(4)));
temp2 = test.insert_sibling(temp2, std::shared_ptr<TreeNode<int>>(new TreeNode<int>(5)));
temp2 = test.insert_sibling(temp2, std::shared_ptr<TreeNode<int>>(new TreeNode<int>(6)));
temp = test.insert_leftchild(temp, std::shared_ptr<TreeNode<int>>(new TreeNode<int>(7)));
auto temp3 = test.insert_sibling(temp, std::shared_ptr<TreeNode<int>>(new TreeNode<int>(8)));
temp = temp3;
temp = test.insert_sibling(temp, std::shared_ptr<TreeNode<int>>(new TreeNode<int>(9)));
temp = test.insert_sibling(temp, std::shared_ptr<TreeNode<int>>(new TreeNode<int>(10)));
temp = test.insert_leftchild(temp, std::shared_ptr<TreeNode<int>>(new TreeNode<int>(17)));
temp = test.insert_sibling(temp, std::shared_ptr<TreeNode<int>>(new TreeNode<int>(18)));
temp3 = test.insert_leftchild(temp3, std::shared_ptr<TreeNode<int>>(new TreeNode<int>(15)));
temp3 = test.insert_sibling(temp3, std::shared_ptr<TreeNode<int>>(new TreeNode<int>(16)));
visit<int> my_visit;
test.layer_traverse(test.visit_root(), my_visit);
std::cout << std::endl;
return 0;
*/