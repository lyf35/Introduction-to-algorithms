#pragma once
#include<iostream>
#include<algorithm>
#include<cmath>
#include<vector>
#include<list>
#include<memory>
#include<queue>
#include<stack>
#include<tuple>
#include"Fibonacci_heap.h"
#include"disjointed_data_set.h"

enum edge_status { tree_edge, backward_edge, forward_edge, cross_edge, undetermined };
enum vertex_status { undiscovered, discovered, visited };
enum graph_type { undirected, directed };

//������С·���Ƚϵ������
bool operator<(std::tuple<int, int, double> a, std::tuple<int, int, double> b)
{
	return std::get<2>(a) < std::get<2>(b);
}
bool operator>(std::tuple<int, int, double> a, std::tuple<int, int, double> b)
{
	return std::get<2>(a) > std::get<2>(b);
}
bool operator==(std::tuple<int, int, double> a, std::tuple<int, int, double> b)
{
	return std::get<2>(a) == std::get<2>(b);
}

template<typename T> struct graph_vertex
{
	int indegree;
	int outdegree;
	int discover_time_dfs; //dt��ft����dfs���������̣���ʼ��Ϊ-1������û�п�ʼ����
	int finish_time_dfs;
	int discover_time_bfs; //����bfs���������̣���¼�����뿪ʼ�������ľ��룬��ʼ��Ϊ-1������û�п�ʼ����
	int region_bfs; //����bfs����ʱ�������ڵ���ͨ��
	vertex_status stat_bfs;
	vertex_status stat_dfs;
	int parent_bfs; //����bfs��dfs���������̣���ֵΪ-1�������bfs������ʱ����Ϊ��ʼ�����Ľ��
	int parent_dfs;
	T key;
	graph_vertex(T k, int in = 0, int out = 0, int dtd = -1, int ftd = -1, int dtb = -1, int reg = 0, vertex_status s_bfs = undiscovered, vertex_status s_dfs = undiscovered, int p_b = -1, int p_d = -1) :indegree(in), outdegree(out), key(k), discover_time_dfs(dtd), finish_time_dfs(ftd), discover_time_bfs(dtb), region_bfs(reg), stat_bfs(s_bfs), stat_dfs(s_dfs), parent_bfs(p_b), parent_dfs(p_d) {};
	graph_vertex() = default;
	void init_bfs() //��bfs���������ݽ�������
	{
		discover_time_bfs = -1;
		region_bfs = 0;
		stat_bfs = undiscovered;
		parent_bfs = -1;
	}
	void init_dfs() //��dfs���������ݽ�������
	{
		discover_time_dfs = -1;
		finish_time_dfs = -1;
		stat_dfs = undiscovered;
		parent_dfs = -1;
	}
};

template<typename W> struct graph_edge
{
	double weight;
	edge_status stat_bfs;
	edge_status stat_dfs;
	int flow; //������㷨�бߵ�����
	int capacity; //����������㷨�б�ʾ�ߵ��������
	W data;
	graph_edge(W d = W(), double w = 1.0, int f = 0, int c = 0, edge_status s_bfs = undetermined, edge_status s_dfs = undetermined) :weight(w), stat_bfs(s_bfs), stat_dfs(s_dfs), data(d), flow(f), capacity(c) {};
	void init_bfs()
	{
		stat_bfs = undetermined;
	}
	void init_dfs()
	{
		stat_dfs = undetermined;
	}
	void init_max_flow()
	{
		flow = 0;
	}
};

template<typename T, typename W> class graph
{
	friend bool operator<(std::tuple<int, int, double> a, std::tuple<int, int, double> b);
	friend bool operator>(std::tuple<int, int, double> a, std::tuple<int, int, double> b);
	friend bool operator==(std::tuple<int, int, double> a, std::tuple<int, int, double> b);
private:
	//���㼯���ڽӾ����е�Ԫ���Ƕ�Ӧ�ģ����ڽӾ����е�i�л��i�еĶ���Ҳ��Ӧ�Ŷ��㼯�еĵ�i������
	std::vector<std::vector<std::shared_ptr<graph_edge<W>>>> matrix; //�ڽӾ���
	std::vector<graph_vertex<T>> vertex; //���㼯
	int number;
	graph_type type; //���������
public:
	graph(graph_type t = undirected) :type(t) {};
	graph(std::vector<std::vector<std::shared_ptr<graph_edge<W>>>> m, std::vector<graph_vertex<T>> v, graph_type t) :matrix(m), vertex(v), number(v.size()), type(t) {}; //����ָ���ַ�Ĺ��캯��
	graph(const graph& G) //�����Ѿ�����һ������ָ���ַ�Ĺ��캯��������Զ���һ��copy operator��ʹ�õ�����graph�У�edge��ָ��ָ�����һ���µı���
	{
		std::vector<std::vector<std::shared_ptr<graph_edge<W>>>> new_matrix(G.number, std::vector<std::shared_ptr<graph_edge<W>>>(G.number, NULL));
		for (int i = 0;i < G.number;i++)
		{
			for (int j = 0;j < G.number;j++)
			{
				if (G.matrix[i][j])
				{
					auto temp = *G.matrix[i][j];
					new_matrix[i][j] = std::make_shared<graph_edge<W>>(temp);
				}
			}
		}
		matrix = new_matrix;
		vertex = G.vertex;
		number = G.number;
		type = G.type;
	}
	graph& operator=(const graph& G) //�Զ���copy assignment operator�õ���graph��edge��ָ��ָ�����һ���µı���
	{
		std::vector<std::vector<std::shared_ptr<graph_edge<W>>>> new_matrix(G.number, std::vector<std::shared_ptr<graph_edge<W>>>(G.number, NULL));
		for (int i = 0;i < G.number;i++)
		{
			for (int j = 0;j < G.number;j++)
			{
				if (G.matrix[i][j])
				{
					auto temp = *G.matrix[i][j];
					new_matrix[i][j] = std::make_shared<graph_edge<W>>(temp);
				}
			}
		}
		matrix = new_matrix;
		vertex = G.vertex;
		number = G.number;
		type = G.type;
		return *this;
	}
	int get_size() const
	{
		return number;
	}
	void insert_vertex(T v)
	{
		vertex.emplace_back(v);
		matrix.push_back(std::vector<std::shared_ptr<graph_edge<W>>>(number + 1, NULL));
		for (auto i = matrix.begin();i < matrix.end() - 1;i++)
		{
			(*i).push_back(NULL);
		}
		number++;
	}
	void delete_vertex(int i)
	{
		for (int m = 0;m < number;m++)
		{
			if (matrix[i][m])
			{
				vertex[m].indegree--;
			}
			if (matrix[m][i])
			{
				vertex[m].outdegree--;
			}
		}
		number--;
		matrix.erase(i);
		for (int j = 0;j < number;j++)
		{
			matrix[j].erase(i);
		}
	}
	void insert_edge(int i, int j, W d = W(), double w = 1.0, int c = 0, int f = 0) //����ӱ��i�����j�Ķ���֮��ı�
	{
		std::shared_ptr<graph_edge<W>> newedge(new graph_edge<W>(d, w, f, c));
		matrix[i][j] = newedge;
		if (type == undirected)
		{
			matrix[j][i] = newedge;
			vertex[j].outdegree++;
			vertex[i].indegree++;
		}
		vertex[i].outdegree++;
		vertex[j].indegree++;
	}
	void delete_edge(int i, int j)
	{
		matrix[i][j] = NULL;
		vertex[i].outdegree--;
		vertex[j].indegree--;
	}
	void clear_bfs_status()
	{
		for (int i = 0;i < number;i++)
		{
			vertex[i].init_bfs();
			for (int j = 0;j < number;j++)
			{
				if (matrix[i][j])
				{
					matrix[i][j]->init_bfs();
				}
			}
		}
	}
	void clear_dfs_status()
	{
		for (int i = 0;i < number;i++)
		{
			vertex[i].init_dfs();
			for (int j = 0;j < number;j++)
			{
				if (matrix[i][j])
				{
					matrix[i][j]->init_dfs();
				}
			}
		}
	}
	void clear_maxflow_status()
	{
		for (int i = 0;i < number;i++)
		{
			for (int j = 0;j < number;j++)
			{
				matrix[i][j]->init_max_flow();
			}
		}
	}
	void broadth_first_search() //ȫ�ֵĹ�����������㷨
	{
		clear_bfs_status();
		int t = 1;
		for (int i = 0;i < number;i++)
		{
			if (vertex[i].stat_bfs == undiscovered) //�����㻹û�б����ʹ����Ե�ǰ������bfs
			{
				bfs(i, t);
				t++;
			}
		}
	}
	int bfs(int i,int t)//�ֲ���ͨ���bfs������i����vertex�еĵ�i�����㣬����t����ڼ�����ͨ��
	{
		std::queue<int> q;
		q.push(i);
		vertex[i].discover_time_bfs = 0; //�޸Ŀ�ʼ���ķ���ʱ��Ϊt
		vertex[i].stat_bfs = discovered;
		while (q.size()) //ֻҪ���в�Ϊ�վ�һֱѭ��
		{
			int node = q.front();
			vertex[node].region_bfs = t;
			q.pop();
			for (int j = 0;j < number;j++)
			{
				if (matrix[node][j] && vertex[j].stat_bfs == undiscovered)
				{
					vertex[j].stat_bfs = discovered;
					vertex[j].parent_bfs = node;
					vertex[j].discover_time_bfs = vertex[node].discover_time_bfs + 1;
					matrix[node][j]->stat_bfs = tree_edge;
					q.push(j);
				}
				else if (matrix[node][j] && vertex[j].stat_bfs != undiscovered)
				{
					if (vertex[node].discover_time_bfs > vertex[j].discover_time_bfs&&vertex[node].region_bfs == vertex[j].region_bfs) //�����������ͬһ��ͨ���ڲŻ��к����
					{
						matrix[node][j]->stat_bfs = backward_edge;
					}
					else //�������������������ֱ�ӱ��Ϊ���
					{
						matrix[node][j]->stat_bfs = cross_edge;
					}
				}
			}
			vertex[node].stat_bfs = visited;
		}
		return t;
	}
	void depth_first_search() //ȫ�ֵ�������������㷨
	{
		clear_dfs_status();
		int t = 0;
		for (int i = 0;i < number;i++)
		{
			if (vertex[i].stat_dfs == undiscovered) //�����㻹û�б����ʹ����Ե�ǰ������dfs
			{
				t = dfs(i, ++t);
			}
		}
	}
	int dfs(int i,int t) //�ֲ���ͨ���������������㷨��i����ʼ�����ĸ������ţ�t����Ե�ǰ���㿪ʼ����ʱ����Ӧ��ʱ���ǩ
	{
		std::stack<int> c; //����ջ�����dfs���������еĽ��
		vertex[i].discover_time_dfs = t; //�޸ĸ�����״̬
		vertex[i].stat_dfs = discovered;
		c.push(i); //�Ƚ����������ջ��
		while (c.size()) //���ջ��Ϊ�գ���һֱѭ����ȥ
		{
			int mark = c.top();
			int j = 0;
			for (;j < number;j++)
			{
				if (matrix[mark][j] && vertex[j].stat_dfs == undiscovered) //�������һ��û�б����ʹ��Ľ�㣬�Ͱ�������ջ�У���ֱ�ӿ�ʼ��һ�ֵ�ѭ��
				{
					t++;
					c.push(j);
					vertex[j].parent_dfs = mark;
					vertex[j].discover_time_dfs = t;
					matrix[mark][j]->stat_dfs = tree_edge;
					vertex[j].stat_dfs = discovered;
					break;
				}
				else if (matrix[mark][j] && vertex[j].stat_dfs != undiscovered&&matrix[mark][j]->stat_dfs != tree_edge)//�������ʹ��Ľ�㣬���ݽ���ʱ�������޸Ķ�Ӧ�ߵ�״̬
				{
					if (vertex[j].discover_time_dfs > vertex[mark].discover_time_dfs&&vertex[j].stat_dfs == visited) //ǰ��ߣ���ʱ���j�Ѿ�������ϣ���ǰ��tֵ����j��finish_time_dfs������mark��j���ֵ�ʱ����
					{
						matrix[mark][j]->stat_dfs = forward_edge;
					}
					if (vertex[j].discover_time_dfs < vertex[mark].discover_time_dfs&&vertex[j].stat_dfs == discovered)//����ߣ���ʱ���j����ջ�ڣ���δ������ϣ�j��finish_time_dfsΪ-1��
					{
						matrix[mark][j]->stat_dfs = backward_edge;
					}
					if (vertex[j].discover_time_dfs < vertex[mark].discover_time_dfs&&vertex[j].stat_dfs == visited)//����ߣ���ʱ���j�Ѿ�������ϣ���ǰ��tֵ����j��finish_time_dfs������mark��j���ֵ�ʱ����
					{
						matrix[mark][j]->stat_dfs = cross_edge;
					}
				}
			}
			if (j == number) //��������������б߶��Ѿ�������ϣ����Խ�����Ϊ�ѷ��ʹ�������ջ���Ƴ�
			{
				t++;
				vertex[mark].stat_dfs = visited;
				vertex[mark].finish_time_dfs = t;
				c.pop();
			}
		}
		return t; //������һ�ֲ���ͨ��ȫ���������֮���Ӧ��ʱ����
	}
	std::vector<int> topological_sort() //����ֵ�ǽ���ŵ��������򣬿�����ǿ��ͨ����������
	{
		depth_first_search();
		std::vector<std::pair<int, int>> temp(number, std::make_pair(0, 0));
		for (int i = 0;i < number;i++)
		{
			temp[i] = std::make_pair(i, vertex[i].finish_time_dfs);
		}
		stable_sort(temp.begin(), temp.end(), [](std::pair<int, int> a, std::pair<int, int> b) {return a.second > b.second;});//���ս��dfs���ʱ�������������
		std::vector<int> topological_sequence(number, 0);
		for (int i = 0;i < number;i++)
		{
			topological_sequence[i] = temp[i].first;
		}
		return topological_sequence;
	}
	graph transpose_graph(graph G) //��ԭͼ��ת��ͼ��ֻ������ͼ��Ч�������еı߶�����
	{
		if (G.type == undirected)
		{
			return G;
		}
		std::vector<std::vector<std::shared_ptr<graph_edge<W>>>> transposed_matrix(number, std::vector<std::shared_ptr<graph_edge<W>>>(number, NULL));
		for (int i = 0;i < number;i++)
		{
			for (int j = 0;j < number;j++)
			{
				if (G.matrix[j][i])
				{
					transposed_matrix[i][j] = new graph_edge<W>(G.matrix[j][i]);
				}
			}
		}
		graph<T, W> transposed_graph(transposed_matrix, G.vertex, G.type);
		for (int i = 0;i < number;i++)
		{
			std::swap(transposed_graph.vertex[i].indegree, transposed_graph.vertex[i].outdegree);
		}
		return transposed_graph;
	}
	std::vector<std::vector<graph_vertex<T>>> strongly_connected_components()
	{
		this->clear_bfs_status();
		this->clear_dfs_status();
		auto transposed_graph = transpose_graph(*this);
		auto topo_seq = topological_sort(); //����ȷ�϶�ת�þ�����dfs��˳��
		int t = 0;
		int cnt = 0;
		for (int i = 0;i < number;i++)
		{
			if (transposed_graph.vertex[topo_seq[i]].stat_dfs == undiscovered) //����ԭ�������������˳���ת�þ������dfs
			{
				t = transposed_graph.dfs(topo_seq[i], ++t);
				cnt++;
			}
		}
		std::vector<std::vector<graph_vertex<T>>> scc(cnt, std::vector<graph_vertex<T>>(0));
		int memo_beg = 1;
		int memo_end = 0;
		for (int i = 0;i < cnt;i++) //����ת�þ����dfs������˳�򣬶Խ����з���
		{
			for (int n = 0;n < number;n++)
			{
				if (transposed_graph.vertex[n].discover_time_dfs == memo_beg)
				{
					memo_end = transposed_graph.vertex[n].finish_time_dfs;
					for (int j = 0;j < number;j++)
					{
						if (transposed_graph.vertex[j].discover_time_dfs >= memo_beg&&transposed_graph.vertex[j].finish_time_dfs <= memo_end)
						{
							scc[i].push_back(transposed_graph.vertex[j]);
						}
					}
					memo_beg = memo_end + 1;
					break;
				}
			}
		}
		return scc;
	}
	std::vector<std::vector<std::shared_ptr<graph_edge<W>>>> MST_Kruskal() //ʹ��Kruskal�㷨���ɵ���С������
	{
		std::vector<std::tuple<std::shared_ptr<graph_edge<W>>, int, int>> edges;//�洢�߼����ھ���������Ӧ��λ��
		for (int i = 0;i < number;i++) //�����еı߼��뵽edges������
		{
			for (int j = i;j < number;j++)
			{
				if (matrix[i][j])
				{
					edges.push_back(std::make_tuple(matrix[i][j], i, j));
				}
			}
		}
		std::stable_sort(edges.begin(), edges.end(), [](std::tuple<std::shared_ptr<graph_edge<W>>, int, int> a, std::tuple<std::shared_ptr<graph_edge<W>>, int, int> b) {return std::get<0>(a)->weight < std::get<0>(b)->weight;}); //����Ȩ�ضԱ߽������򣬴˴�Ҳ���Ե���֮ǰд�Ŀ��ţ����ŵ��㷨��������ʹ�������涨��ı߱Ƚ������
		std::vector<std::shared_ptr<set_forest_node<graph_vertex<T>>>> vertex_set; //��ÿ�����㶼����һ�������ļ��ϣ�������MST��ʱ��Զ������ڼ������ϲ�
		for (int i = 0;i < number;i++) 
		{
			vertex_set.push_back(std::shared_ptr<set_forest_node<graph_vertex<T>>>(new set_forest_node<graph_vertex<T>>(vertex[i])));
			vertex_set[i]->node_fix();
		}
		std::vector<std::vector<std::shared_ptr<graph_edge<W>>>> MST(number, std::vector<std::shared_ptr<graph_edge<W>>>(number, NULL)); //�������MST�еı�
		for (int i = 0;i < edges.size();i++)
		{
			if (find_set(vertex_set[std::get<1>(edges[i])]) != find_set(vertex_set[std::get<2>(edges[i])])) //����������㲻����ͬһ�����ϣ���ô���˱߼��뵽��С�������в���ɻ�
			{
				MST[std::get<1>(edges[i])][std::get<2>(edges[i])] = std::get<0>(edges[i]);
				MST[std::get<2>(edges[i])][std::get<1>(edges[i])] = std::get<0>(edges[i]);
				union_set(vertex_set[std::get<1>(edges[i])], vertex_set[std::get<2>(edges[i])]); //������MST�ı�����Ӧ����������ϲ�Ϊͬһ������
			}
		}
		return MST;
	}
	std::vector<std::vector<std::shared_ptr<graph_edge<W>>>> MST_Prim() //prim�㷨���ɵ���С��������������һ��ʱ�̣������Ѿ�ѡ��Ķ���
	{
		Fibonacci_heap<std::tuple<int, int, double>> vertex_heap; //���涥�㼰����Ϣ��һ��쳲������ѣ�tuple�еĵ�һ��Ԫ�ش������ţ��ڶ���Ԫ�ش������ı�ţ�����������ǰ���������ӵ�Ȩ����С�ıߣ��������Ѿ�ȡ���Ķ��㣩
		std::vector<std::shared_ptr<Fibonacci_heap_node<std::tuple<int, int, double>>>> node;
		std::vector<std::vector<std::shared_ptr<graph_edge<W>>>> MST(number, std::vector<std::shared_ptr<graph_edge<W>>>(number, NULL)); //�������MST�еı�
		for (int i = 0;i < number;i++) //��node�����������Ա�decrease_key��ʱ��ʹ�ã�ͬʱ��Ԫ�ز��뵽fibonacci����
		{
			node.push_back(std::shared_ptr<Fibonacci_heap_node<std::tuple<int, int, double>>>(new Fibonacci_heap_node<std::tuple<int, int, double>>(NULL, NULL, NULL, NULL, 0, std::make_tuple(i, -1, LDBL_MAX), false, true)));
			vertex_heap.insert_node(node[i]);
		}
		vertex_heap.decrease_key(node[0], std::make_tuple(0, -1, 0.0));
		while (vertex_heap.get_size())
		{
			auto temp = vertex_heap.extract_min()->key;
			if (std::get<1>(temp) != -1) //����Ӧ�߲��뵽MST������
			{
				MST[std::get<1>(temp)][std::get<0>(temp)] = MST[std::get<0>(temp)][std::get<1>(temp)] = matrix[std::get<0>(temp)][std::get<1>(temp)];
			}
			for (auto i = 0;i < number;i++)
			{
				if (matrix[std::get<0>(temp)][i] && node[i]->inside) //���ҵ��ߣ���������������Ӧ����һ�����㻹�ڶ���
				{
					if (std::get<2>(node[i]->key)>matrix[std::get<0>(temp)][i]->weight)//�������������㵱ǰ��¼����СȨ��С���·��ֵ�Ȩ�أ����޸���
					{
						vertex_heap.decrease_key(node[i], std::make_tuple(i, std::get<0>(temp), matrix[std::get<0>(temp)][i]->weight));
					}
				}
			}
		}
		return MST;
	}
	std::pair<std::vector<double>, std::vector<int>> Bellman_Ford(int root) //��һ��vector����Ȩ�أ��ڶ������游���
	{
		std::vector<double> distance(number, LDBL_MAX); //���ڱ�����ľ��룬����Ϊ0���Ǹ������ʼ�����ĵ�
		std::vector<int> parent(number, -1);
		distance[root] = 0;
		int count = 0; //��count����������������ʱ����ѭ�������������Ȩ��Ϊ���Ļ�·����ô���·���ĳ��������V-1����˵�count=V-1��ʱ���������һ��ѭ����������·���Ա��޸ģ��Ǿ�˵�����ָ�Ȩ�صĻ�·
		bool stat = false; //����ȷ��ѭ���������Ƿ����С·�������޸�
		while (count < number)
		{
			stat = true; //false��������ѭ����������С·�����޸ģ�true����ѭ����ʱ��δ���޸Ĺ�
			for (int i = 0;i < number;i++)
			{
				for (int j = 0;j < number;j++)
				{
					if (matrix[i][j] && distance[i] != LDBL_MAX && distance[i] + matrix[i][j]->weight < distance[j])
					{
						stat = false;
						distance[j] = distance[i] + matrix[i][j]->weight;
						parent[j] = i;
					}
				}
			}
			count++;
		}
		if (!stat) //��������ڸ�Ȩ�صĻ�·����ôwhileѭ�������һ���������κβ����������if���ᱻִ�С����ִ�У�˵�����ڸ�Ȩ�ػ�·����ѻ�·�������������
		{
			std::cout << "The graph contains negative weight circuit: ";
			for (int i = 0;i < number;i++)
			{
				for (int j = 0;j < number;j++)
				{
					if (matrix[i][j] && distance[i] != LDBL_MAX && distance[i] + matrix[i][j]->weight < distance[j])
					{
						std::cout << i << "->" << j << " ";
					}
				}
			}
			std::cout << std::endl;
		}
		return std::make_pair(distance, parent);
	}
	std::pair<std::vector<double>, std::vector<int>> DAG_shortest_path(int i) //����DAGͼ����С���룬�ȶԽ������������Ȼ��ֻ��һ��ѭ������������н������ȷ��
	{
		auto seq = topological_sort();
		std::vector<double> distance(number, LDBL_MAX);
		distance[seq[i]] = 0;
		for (auto i : seq)
		{
			for (auto j = 0;j < number;j++)
			{
				if (matrix[i][j] && distance[i] + matrix[i][j]->weight < distance[j])
				{
					distance[j] = distance[i] + matrix[i][j]->weight;
				}
			}
		}
		return std::make_pair(distance, seq);
	}
	std::pair<std::vector<double>, std::vector<int>> Dijkstra(int root) //��dijkstra�㷨����С���룬Ҫ�����бߵ�Ȩ�ض�Ϊ�Ǹ�ֵ����prim�㷨��˼·���ƣ���쳲���������������
	{
		Fibonacci_heap<std::tuple<int, int, double>> vertex_heap; //���涥�㼰����Ϣ��һ��쳲������ѣ�tuple�еĵ�һ��Ԫ�ش������ţ��ڶ���Ԫ�ش������ı�ţ�����������ֻʹ���Ѿ�ȡ���Ķ���������ɵ����·����Ȩ��
		std::vector<std::shared_ptr<Fibonacci_heap_node<std::tuple<int, int, double>>>> node;
		for (int i = 0;i < number;i++) //��node�����������Ա�decrease_key��ʱ��ʹ�ã�ͬʱ��Ԫ�ز��뵽fibonacci����
		{
			node.push_back(std::shared_ptr<Fibonacci_heap_node<std::tuple<int, int, double>>>(new Fibonacci_heap_node<std::tuple<int, int, double>>(NULL, NULL, NULL, NULL, 0, std::make_tuple(i, -1, LDBL_MAX), false, true)));
			vertex_heap.insert_node(node[i]);
		}
		vertex_heap.decrease_key(node[root], std::make_tuple(root, -1, 0.0)); //�������ľ�������Ϊ0
		while (vertex_heap.get_size())
		{
			auto temp = vertex_heap.extract_min()->key;
			for (auto i = 0;i < number;i++)
			{
				if (matrix[std::get<0>(temp)][i] && node[i]->inside && std::get<2>(temp) + matrix[std::get<0>(temp)][i]->weight < std::get<2>(node[i]->key)) //���ҵ���
				{
					vertex_heap.decrease_key(node[i], std::make_tuple(i, std::get<0>(temp), std::get<2>(temp) + matrix[std::get<0>(temp)][i]->weight)); //���ָ��̵�·���������޸�
				}
			}
		}
		std::vector<double> distance(number, 0); //���ڱ�����ľ��룬����Ϊ0���Ǹ������ʼ�����ĵ�
		std::vector<int> parent(number, -1);
		for (auto i = 0;i < number;i++)
		{
			distance[i] = std::get<2>(node[i]->key);
			parent[i] = std::get<1>(node[i]->key);
		}
		return std::make_pair(distance, parent);
	}
	std::pair<std::vector<std::vector<double>>,std::vector<std::vector<int>>> all_pairs_shortest_paths() //����С·������Ϊ0��ʼ��һֱ����С��v-1
	{
		std::vector<std::vector<double>> distance(number, std::vector<double>(number,LDBL_MAX)); //i,jλ�ô���ӵ�i����j�����Ȩ��·��
		std::vector<std::vector<int>> parent(number, std::vector<int>(number, -1)); //i,jλ�ô���ӵ�i����j�����Ȩ��·���ϣ�j��ǰ�����
		for (int i = 0;i < number;i++) //�ȸ�distance�����ʼ��
		{
			for (int j = 0;j < number;j++)
			{
				if (i == j)
				{
					distance[i][j] = 0;
				}
				else if (matrix[i][j])
				{
					distance[i][j] = matrix[i][j]->weight;
					parent[i][j] = i;
				}
			}
		}
		for (int i = 0;i < ceil(log(number));i++) //ÿ��ѭ����ʼʱi���ڼ����ʹ�����С·��ֻ����2^i���м��
		{
			auto temp_distance = distance; //��Ҫ���м�����������������еõ���ֵ
			auto temp_parent = parent;
			for (int x = 0;x < number;x++)
			{
				for (int y = 0;y < number;y++)
				{
					for (int k = 0;k < number;k++)
					{
						if (temp_distance[x][y]>distance[x][k] + distance[k][y])
						{
							temp_distance[x][y] = distance[x][k] + distance[k][y];
							temp_parent[x][y] = parent[k][y];
						}
					}
				}
			}
			distance = temp_distance;
			parent = temp_parent;
		}
		for (int x = 0;x < number;x++) //��⸺Ȩ�ػ�·
		{
			for (int y = 0;y < number;y++)
			{
				for (int k = 0;k < number;k++)
				{
					if (distance[x][y]>distance[x][k] + distance[k][y])
					{
						std::cout << "The graph contains negative weight circult." << std::endl;
						return std::make_pair(distance, parent);
					}
				}
			}
		}
		return std::make_pair(distance, parent);
	}
	std::pair<std::vector<std::vector<double>>, std::vector<std::vector<int>>> Floyd_Warshall() //��С·���������Ϊ0-i�Ķ��㣬һֱѭ����ȥ��ֱ��i=v
	{
		std::vector<std::vector<double>> distance(number, std::vector<double>(number, LDBL_MAX)); //i,jλ�ô���ӵ�i����j�����Ȩ��·��
		std::vector<std::vector<int>> parent(number, std::vector<int>(number, -1)); //i,jλ�ô���ӵ�i����j�����Ȩ��·���ϣ�j��ǰ�����
		for (int i = 0;i < number;i++) //�ȸ�distance�����ʼ��
		{
			for (int j = 0;j < number;j++)
			{
				if (i == j)
				{
					distance[i][j] = 0;
				}
				else if (matrix[i][j])
				{
					distance[i][j] = matrix[i][j]->weight;
					parent[i][j] = i;
				}
			}
		}
		for (int i = 0;i < number;i++) //ÿ��ѭ����ʼʱi���ڼ����ʹ�����С·��ֻ�������Ϊ0-i�ĵ�
		{
			for (int x = 0;x < number;x++)
			{
				for (int y = 0;y < number;y++) //ѭ�������У��Ѿ����޸ĵ�ֵ����Ӱ�����õ���ֵ����˿��Բ�����ʱ������������
				{
					if (distance[x][y]>distance[x][i] + distance[i][y])
					{
						distance[x][y] = distance[x][i] + distance[i][y];
						parent[x][y] = parent[i][y];
					}
				}
			}
		}
		for (int x = 0;x < number;x++) //����Ƿ��и�Ȩ�صĻ�·
		{
			for (int y = 0;y < number;y++)
			{
				for (int k = 0;k < number;k++)
				{
					if (distance[x][y]>distance[x][k] + distance[k][y])
					{
						std::cout << "The graph contains negative weight circult." << std::endl;
						return std::make_pair(distance, parent);
					}
				}
			}
		}
		return std::make_pair(distance, parent);
	}
	std::pair<std::vector<std::vector<double>>, std::vector<std::vector<int>>> Johnson() //���¸���Ȩ��֮�󲻸ı����·�������ô����ʿ������޸����бߵ�Ȩ��Ϊ�Ǹ���Ȼ����޸�Ȩ�غ��ͼ��ÿ�����㶼����һ��Dijkstra�㷨���ɣ�����ʹ��ʱ�临�Ӷȵ���V^3
	{
		graph newgraph = *this;
		newgraph.insert_vertex(T()); //Ϊԭͼ���һ���µĶ��㣬�Լ����Ӹ���ԭ�����Ȩ��Ϊ0�ı�
		for (int i = 0;i < number;i++)
		{
			newgraph.insert_edge(number, i);
		}
		std::vector<double> dis = newgraph.Bellman_Ford(number).first; //���¶��㵽������������·�������ڸı�ߵ�Ȩ�أ�ʹȨ�طǸ�
		newgraph = *this;
		for (int i = 0;i < number;i++)
		{
			for (int j = 0;j < number;j++)
			{
				if (newgraph.matrix[i][j])
				{
					newgraph.matrix[i][j]->weight += (dis[i] - dis[j]); //�������ǲ���ʽ��d[i]+w[i][j]>d[j]�����ݴ����ʶ�ÿ���ߵ�Ȩ�����޸ģ�ʹ���Ϊ�Ǹ���Ȩ��
				}
			}
		}
		std::vector<std::vector<double>> distance;
		std::vector<std::vector<int>> parent;
		for (int i = 0;i < number;i++)
		{
			auto temp = newgraph.Dijkstra(i); //�������бߵ�Ȩ�طǸ����Ϳ���ֱ�Ӷ�ÿ�����㶼����һ��Dijkstra�㷨
			distance.push_back(temp.first);
			parent.push_back(temp.second);
		}
		for (int i = 0;i < number;i++)
		{
			for (int j = 0;j < number;j++)
			{
				distance[i][j] -= (dis[i] - dis[j]); //֮ǰ����������޸�Ȩ��֮������·������Ҫ����w'[p]=w[p]+h[i]-h[j]�����·�����л�ԭ
			}
		}
		return std::make_pair(distance, parent);
	}
	void Ford_Fulkerson() //���������������Ford_fulkerson�㷨�����е�ʱ��Ҫ���һ�������Դ�ڵ�s�����һ������ǻ��t
	{
		graph residual_network = *this;//����һ��graph������Ϊ�д����磬ÿһ������֮�󶼶Բд���������Ӧ���޸ģ������ͱ�����ÿ�ζ������Ʋ�������capacity������ʾ�����߿������͵�����
		while (true)
		{
			residual_network.clear_bfs_status();
			residual_network.bfs(0, 0);
			if (residual_network.vertex[number-1].parent_bfs < 0) //��������s��t������·�������Ѿ����һ�������������ֱ���˳�ѭ��
			{
				break;
			}
			int max_flow = INT_MAX;
			int point = number - 1;
			while (residual_network.vertex[point].parent_bfs >= 0) //�����Ǵ�Դ�ڵ�s��ʼ�����ģ����ֻҪ��bfsʱ���t�ĸ���㲻Ϊ�գ���������һ��s��t������·������Ѱ������·���ϵ�������Сֵ
			{
				if (residual_network.matrix[residual_network.vertex[point].parent_bfs][point]->capacity < max_flow)
				{
					max_flow = residual_network.matrix[residual_network.vertex[point].parent_bfs][point]->capacity;
				}
				point = residual_network.vertex[point].parent_bfs;
			}
			point = number - 1;
			while (residual_network.vertex[point].parent_bfs >= 0) //�ҵ�����·��֮�󣬶�ԭͼ������·���ϵ����������޸ģ����޸Ĳд�����
			{
				if (residual_network.matrix[residual_network.vertex[point].parent_bfs][point]) //��ԭͼ��·���ϵ����������޸�
				{
					matrix[residual_network.vertex[point].parent_bfs][point]->flow += max_flow;
				}
				else
				{
					matrix[point][residual_network.vertex[point].parent_bfs]->flow -= max_flow;
				}

				residual_network.matrix[residual_network.vertex[point].parent_bfs][point]->capacity -= max_flow;
				if (residual_network.matrix[residual_network.vertex[point].parent_bfs][point]->capacity == 0) //���ĳ���ߵ������Ѿ�������Ҫ�ڲд�������ɾ����������
				{
					residual_network.delete_edge(residual_network.vertex[point].parent_bfs, point);
				}
				if (residual_network.matrix[point][residual_network.vertex[point].parent_bfs]) //����д��������Ѿ����ڷ���ߣ���ֱ���޸ĸñߵ���������
				{
					residual_network.matrix[point][residual_network.vertex[point].parent_bfs]->capacity += max_flow;
				}
				else //��������ڸ÷���ߣ�����Ҫ���һ���±ߣ����޸�����
				{
					residual_network.insert_edge(point, residual_network.vertex[point].parent_bfs);
					residual_network.matrix[point][residual_network.vertex[point].parent_bfs]->capacity += max_flow;
				}
				point = residual_network.vertex[point].parent_bfs;
			}
		}
	}
	void maxflow_push_relabel() //�������������������-������ǩ�㷨�����е�ʱ��Ҫ���һ�������Դ�ڵ�s�����һ������ǻ��t
	{
		graph residual_network = *this; //�д����磬��capacity������ʾ�����߿������͵��������
		std::vector<int> height(number, 0); //����ÿ�����ĸ߶ȣ�����Դ�ڵ�s�ĸ߶���ΪV
		height[0] = number;
		std::queue<int> overflow_vertex; //���ڱ����г��������Ķ���
		std::vector<int> overflow(number, 0); //����ÿ�����Ķ�������
		std::vector<std::list<int>> high(number - 1, std::list<int>()); //����д�������ÿ���������ڽ���У��߶�С���������ߡ����listΪ�գ���ô��˵����Ҫ��relabel������������Ҫ��push����
		for (int i = 0;i < number;i++) //��ʼ������
		{
			if (matrix[0][i])
			{
				overflow[i] = matrix[0][i]->capacity;
				matrix[0][i]->flow = matrix[0][i]->capacity;
				residual_network.delete_edge(0, i);
				residual_network.insert_edge(i, 0);
				residual_network.matrix[i][0]->capacity = matrix[0][i]->flow;
				if (i != number - 1)
				{
					overflow_vertex.push(i);
				}
			}
		}
		while (overflow_vertex.size()) //ֻҪ���������н�㣬�ͽ�ѭ��������ȥ
		{
			int node = overflow_vertex.front(); //ȡ�������е�Ԫ��
			auto ptr = high[node].begin();
			if (high[node].size()) //�������Ӧ��list��Ϊ�գ�����push����
			{
				while (ptr!=high[node].end() && overflow[node]) //���������ж��������������п������͵Ľ�㣬��һֱѭ����ȥ
				{
					int temp = *ptr;
					if (residual_network.matrix[node][temp])
					{
						int push_flow = residual_network.matrix[node][temp]->capacity < overflow[node] ? residual_network.matrix[node][temp]->capacity : overflow[node];
						overflow[node] -= push_flow;
						if (temp != 0 && temp != number - 1) //������͵�Ŀ���㲻��Դ�ڵ�ͻ�㣬��Ҫ��һЩ�޸�
						{
							overflow[temp] += push_flow;
							overflow_vertex.push(temp);
						}
						if (matrix[node][temp]) //��ԭͼ��·���ϵ����������޸�
						{
							matrix[node][temp]->flow += push_flow;
						}
						else
						{
							matrix[temp][node]->flow -= push_flow;
						}

						residual_network.matrix[node][temp]->capacity -= push_flow;
						if (residual_network.matrix[node][temp]->capacity == 0) //���ĳ���ߵ������Ѿ�������Ҫ�ڲд�������ɾ����������
						{
							residual_network.delete_edge(node, temp);
						}
						if (residual_network.matrix[temp][node]) //����д��������Ѿ����ڷ���ߣ���ֱ���޸ĸñߵ���������
						{
							residual_network.matrix[temp][node]->capacity += push_flow;
						}
						else //��������ڸ÷���ߣ�����Ҫ���һ���±ߣ����޸�����
						{
							residual_network.insert_edge(temp, node);
							residual_network.matrix[temp][node]->capacity += push_flow;
						}
						ptr++;
					}
					else 
					{
						auto temp = ptr;
						ptr++;
						high[node].erase(temp);
					}
				}
				if (!overflow[node]) //ѭ������֮�����û�ж�����������ʹӶ�����ɾ��������
				{
					overflow_vertex.pop();
				}
			}
			else //��relabel����
			{
				std::list<int> height_node;
				int min_height = INT_MAX;
				for (int i = 0;i < number;i++) //Ѱ�Ҳд������п��Ե���ĸ߶���͵Ľ�㣬�Լ�����߶�ֵ
				{
					if (residual_network.matrix[node][i])
					{
						if (height[i] < min_height)
						{
							height_node.clear();
							min_height = height[i];
							height_node.push_back(i);
						}
						if (height[i] == min_height)
						{
							height_node.push_back(i);
						}
					}
				}
				height[node] = min_height + 1;
				high[node] = height_node;
				for (auto i : high[node])
				{
					if (i != number - 1)
					{
						auto j = high[i].begin();
						while (j != high[i].end())
						{
							if (*j == node)
							{
								auto temp = j;
								j++;
								high[i].erase(temp);
							}
							else
							{
								j++;
							}
						}
					}
				}
			}
		}
	}
	void maxflow_relabel_to_front()
	{
		std::vector<int> height(number, 0); //����ÿ�����ĸ߶�
		height[0] = number;
		std::vector<int> overflow(number, 0); //����ÿ�����Ķ�������
		for (int i = 0;i < number;i++)
		{
			if (matrix[0][i])
			{
				overflow[i] = matrix[0][i]->capacity;
				matrix[0][i]->flow = matrix[0][i]->capacity;
			}
		}
		std::vector<std::list<int>> neighbor(number - 1, std::list<int>()); //����ÿ���������ڽ��
		std::list<int> sequence; //�������й����н����з��ʵ�˳���ڳ������й����п��ܻᱻ����
		for (int i = 1;i < number - 1;i++) 
		{
			for (int j = 0;j < number;j++)
			{
				if (matrix[i][j] || matrix[j][i])
				{
					neighbor[i].push_back(j);
				}
			}
			sequence.push_back(i);
		}
		std::vector<std::list<int>::iterator> current(number, sequence.begin()); //ÿһ��λ�ö���Ӧ��neighbor��ͬ��λ���ϵ�list
		for (auto i = 1;i < number - 1;i++)
		{
			current[i] = neighbor[i].begin();
		}
		auto label = sequence.begin(); //��Ϊsequence��Ŀǰָ��λ�õı��
		while (label != sequence.end()) //��������ָ��neighbor����ĩ��ʱ������ȫͼ��һ�������
		{
			int old_height = height[*label];
			while (overflow[*label]) //���ĳ����ж������������Ըý��������ֱ������������ʧ
			{
				auto ptr = current[*label]; 
				if (ptr == neighbor[*label].end())
				{
					int min_height = INT_MAX;
					for (auto i : neighbor[*label]) //Ѱ�Ҳд������п��Ե���ĸ߶���͵Ľ�㣬�Լ�����߶�ֵ
					{
						if (matrix[i][*label]) //���i��Ӧ�ı���һ�����
						{
							if (matrix[i][*label]->flow&&height[i] < min_height) //��ʱ������ϱ����������Ŷ�����п��ǣ������޷�����
							{
								min_height = height[i];
							}
						}
						else if(matrix[*label][i]) //���i��Ӧһ������
						{
							if (matrix[*label][i]->flow<matrix[*label][i]->capacity&&height[i] < min_height) //����������ж��������������ͣ��Ŷ�����п���
							{
								min_height = height[i];
							}
						}
					}
					height[*label] = min_height + 1; //�޸Ľ���Ӧ�ĸ߶�
					current[*label] = neighbor[*label].begin(); //�޸�current
				}
				else if (height[*label] == height[*ptr] + 1) //Ѱ�ҽ�������Ͳ���
				{
					int flow = overflow[*label];
					if (matrix[*ptr][*label]) //���ptr��Ӧ�ı���һ�����
					{
						if (matrix[*ptr][*label]->flow < flow)
						{
							flow = matrix[*ptr][*label]->flow;
						}
						matrix[*ptr][*label]->flow -= flow;
						overflow[*label] -= flow;
						if (*ptr != 0 && *ptr != number - 1)
						{
							overflow[*ptr] += flow;
						}
					}
					else  //���ptr��Ӧһ������
					{
						if (matrix[*label][*ptr]->capacity - matrix[*label][*ptr]->flow < flow)
						{
							flow = matrix[*label][*ptr]->capacity - matrix[*label][*ptr]->flow;
						}
						matrix[*label][*ptr]->flow += flow;
						overflow[*label] -= flow;
						if (*ptr != 0 && *ptr != number - 1)
						{
							overflow[*ptr] += flow;
						}
					}
					if (overflow[*label] > 0) //���������֮�����ж������������current�д洢��ָ������
					{
						current[*label]++;
					}
				}
				else
				{
					current[*label]++;
				}
			}
			if (height[*label] > old_height) //����߶ȷ����仯���Ͱ��������Ƶ����е���ǰ��
			{
				auto temp = *label;
				sequence.erase(label);
				sequence.push_front(temp);
				label = sequence.begin();
			}
			label++;
		}
	}
	template<typename U> void traverse_vertex(U op_vertex)
	{
		for (auto i : vertex)
		{
			op_vertex(i);
		}
	}
	template<typename U> void traverse_edge(U op_edge)
	{
		for (int i = 0;i < number;i++)
		{
			for (int j = 0;j < number;j++)
			{
				if (matrix[i][j])
				{
					op_edge(i, j, matrix[i][j]);
				}
			}
		}
	}
};

/*
��clrs22.3.2����ϰ��Ϊ������bfs/dfs/scc�����㷨��
template<typename T> struct print_vertex_bfs
{
	void operator()(graph_vertex<T> v)
	{
		std::cout << v.key << " Start time: " << v.discover_time_bfs << '\t' << "Region:" << v.region_bfs << '\t' << "Parent: " << v.parent_bfs << '\t' << std::endl;
	}
};
template<typename T> struct print_vertex_dfs
{
	void operator()(graph_vertex<T> v)
	{
		std::cout << v.key << " Start time: " << v.discover_time_dfs << '\t' << " Finish time: " << v.finish_time_dfs << '\t' << "Parent: " << v.parent_dfs << '\t' << std::endl;
	}
};
template<typename U> struct print_edge_bfs
{
	void operator()(int i, int j, std::shared_ptr<graph_edge<U>> e)
	{
		std::cout << "Edge from vertex " << i << " to " << j << ":" << e->stat_bfs << std::endl;
	}
};
template<typename U> struct print_edge_dfs
{
	void operator()(int i, int j, std::shared_ptr<graph_edge<U>> e)
	{
		std::cout << "Edge from vertex " << i << " to " << j << ":" << e->stat_dfs << std::endl;
	}
};

int main()
{
	graph<char, int> test(directed);
	print_edge_bfs<int> e_bfs;
	print_edge_dfs<int> e_dfs;
	print_vertex_bfs<char> v_bfs;
	print_vertex_dfs<char> v_dfs;
	test.insert_vertex('v');
	test.insert_vertex('s');
	test.insert_vertex('w');
	test.insert_vertex('q');
	test.insert_vertex('t');
	test.insert_vertex('x');
	test.insert_vertex('z');
	test.insert_vertex('y');
	test.insert_vertex('r');
	test.insert_vertex('u');
	test.insert_edge(1, 0, 0);
	test.insert_edge(0, 2, 0);
	test.insert_edge(2, 1, 0);
	test.insert_edge(3, 1, 0);
	test.insert_edge(3, 2, 0);
	test.insert_edge(3, 4, 0);
	test.insert_edge(4, 7, 0);
	test.insert_edge(4, 5, 0);
	test.insert_edge(5, 6, 0);
	test.insert_edge(6, 5, 0);
	test.insert_edge(7, 3, 0);
	test.insert_edge(9, 7, 0);
	test.insert_edge(8, 7, 0);
	test.insert_edge(8, 9, 0);
	test.broadth_first_search();
	std::cout << "BFS:" << std::endl;
	test.traverse_edge(e_bfs);
	test.traverse_vertex(v_bfs);
	test.depth_first_search();
	std::cout << "DFS:" << std::endl;
	test.traverse_edge(e_dfs);
	test.traverse_vertex(v_dfs);
	auto p = test.strongly_connected_components();
	std::cout << "Strongly connect components:" << std::endl;
	for (auto i : p)
	{
		for (auto j : i)
		{
			std::cout << j.key << '\t';
		}
		std::cout << std::endl;
	}
	return 0;
}
*/

/*
������С�������Ĵ��루��clrs23.2��ʾ��ͼ��ͬ��
int main()
{
	graph<char, int> test(undirected);
	test.insert_vertex('a');
	test.insert_vertex('b');
	test.insert_vertex('c');
	test.insert_vertex('d');
	test.insert_vertex('e');
	test.insert_vertex('f');
	test.insert_vertex('g');
	test.insert_vertex('h');
	test.insert_vertex('i');
	test.insert_edge(0, 1, int(), 4);
	test.insert_edge(1, 2, int(), 8);
	test.insert_edge(2, 3, int(), 7);
	test.insert_edge(3,4, int(), 9);
	test.insert_edge(4,5, int(), 10);
	test.insert_edge(3,5, int(), 14);
	test.insert_edge(5,6, int(), 2);
	test.insert_edge(6,7, int(), 1);
	test.insert_edge(7,8, int(), 7);
	test.insert_edge(6,8, int(), 6);
	test.insert_edge(2,8, int(), 2);
	test.insert_edge(2,5, int(), 4);
	test.insert_edge(1,7, int(), 11);
	test.insert_edge(0, 7, int(), 8);
	auto result = test.MST_Kruskal();
	auto result2 = test.MST_Prim();
	std::cout << "MST Kruskal algorithm:" << std::endl;
	for (int i = 0;i < result.size();i++)
	{
		for (int j = 0;j < result.size();j++)
		{
			if (result[i][j])
			{
				std::cout << 1 << " ";
			}
			else
			{
				std::cout << 0 << " ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << "MST Prim algorithm:" << std::endl;
	for (int i = 0;i < result.size();i++)
	{
		for (int j = 0;j < result.size();j++)
		{
			if (result2[i][j])
			{
				std::cout << 1 << " ";
			}
			else
			{
				std::cout << 0 << " ";
			}
		}
		std::cout << std::endl;
	}
	return 0;
}
*/

/*
���Ե�Դ���·���Ĵ��루clrsͼ24-6����
int main()
{
	graph<char, int> test(directed);
	test.insert_vertex('a');
	test.insert_vertex('b');
	test.insert_vertex('c');
	test.insert_vertex('d');
	test.insert_vertex('e');
	test.insert_edge(0,1,int(),10);
	test.insert_edge(0,4, int(), 5);
	test.insert_edge(1,4, int(), 2);
	test.insert_edge(1,2, int(), 1);
	test.insert_edge(1,4, int(), 2);
	test.insert_edge(2,3, int(), 4);
	test.insert_edge(3,2,int(), 6);
	test.insert_edge(3,0, int(), 7);
	test.insert_edge(4,1, int(), 3);
	test.insert_edge(4,2, int(), 9);
	test.insert_edge(4,3, int(), 2);
	auto j = test.Dijkstra(0).first;
	auto i = test.Bellman_Ford(0).first;
	for (auto x : i)
	{
		std::cout << x << " ";
	}
	std::cout << std::endl;
	for (auto x : j)
	{
		std::cout << x << " ";
	}
	std::cout << std::endl;
	auto m = test.Bellman_Ford(0).second;
	auto n = test.Dijkstra(0).second;
	for (auto x : m)
	{
		std::cout << x << " ";
	}
	std::cout << std::endl;
	for (auto x : n)
	{
		std::cout << x << " ";
	}
	std::cout << std::endl;
	return 0;
}
*/

/*
DAGͼ�Ĳ��Դ��루��clrsͼ24-5Ϊ������
int main()
{
	graph<char, int> test(directed);
	test.insert_vertex('a');
	test.insert_vertex('b');
	test.insert_vertex('c');
	test.insert_vertex('d');
	test.insert_vertex('e');
	test.insert_vertex('f');
	test.insert_edge(0,1,int(),5);
	test.insert_edge(0,2, int(), 3);
	test.insert_edge(1,2, int(), 2);
	test.insert_edge(1,3, int(), 6);
	test.insert_edge(2,3, int(), 7);
	test.insert_edge(2,4, int(), 4);
	test.insert_edge(2,5, int(), 2);
	test.insert_edge(3,4, int(), -1);
	test.insert_edge(3,5, int(), 1);
	test.insert_edge(4,5, int(), -2);
	auto dis = test.DAG_shortest_path(1).first;
	auto sequence = test.DAG_shortest_path(1).second;
	for (auto i : dis)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;
	for (auto i : sequence)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;
	return 0;
}
*/

/*
�������н������·����Դ���루��clrsͼ25-6Ϊ������
int main()
{
	graph<char, int> test(directed);
	test.insert_vertex('a');
	test.insert_vertex('b');
	test.insert_vertex('c');
	test.insert_vertex('d');
	test.insert_vertex('e');
	test.insert_edge(0, 1, int(), 3);
	test.insert_edge(0,2, int(), 8);
	test.insert_edge(0,4, int(), -4);
	test.insert_edge(1,3, int(), 1);
	test.insert_edge(1,4, int(), 7);
	test.insert_edge(2,1, int(), 4);
	test.insert_edge(3,2, int(), -5);
	test.insert_edge(3,0, int(), 2);
	test.insert_edge(4,3, int(), 6);
	auto result = test.all_pairs_shortest_paths(); 
	auto distance = result.first;
	auto parent = result.second;
	std::cout << "all-pair shortest path by matrix multiplication:" << std::endl;
	for (auto i : distance)
	{
		for (auto j : i)
		{
			std::cout << j << '\t';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	for (auto i : parent)
	{
		for (auto j : i)
		{
			std::cout << j << '\t';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	result = test.Floyd_Warshall();
	distance = result.first;
	parent = result.second;
	std::cout << "all-pair shortest path by Floyd-warshall algorithm:" << std::endl;
	for (auto i : distance)
	{
		for (auto j : i)
		{
			std::cout << j << '\t';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	for (auto i : parent)
	{
		for (auto j : i)
		{
			std::cout << j << '\t';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	result = test.Johnson();
	distance = result.first;
	parent = result.second;
	std::cout << "all-pair shortest path by Johnson algorithm:" << std::endl;
	for (auto i : distance)
	{
		for (auto j : i)
		{
			std::cout << j << '\t';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	for (auto i : parent)
	{
		for (auto j : i)
		{
			std::cout << j << '\t';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	return 0;
}
*/

/*
����������㷨�ĳ�����clrsͼ26-6Ϊ������
template<typename U> struct print_edge_maximum_flow
{
	void operator()(int i, int j, std::shared_ptr<graph_edge<U>> e)
	{
		std::cout << "Edge from " << i << " to " << j << ": " << e->flow << "/" << e->capacity << std::endl;
	}
};

int main()
{
	print_edge_maximum_flow<int> print;
	graph<std::string, int> test(directed);
	test.insert_vertex("s");
	test.insert_vertex("v1");
	test.insert_vertex("v2");
	test.insert_vertex("v3");
	test.insert_vertex("v4");
	test.insert_vertex("t");
	test.insert_edge(0,1, int(),0,16 );
	test.insert_edge(0,2 ,int(), 0,13 );
	test.insert_edge(1,3 ,int(), 0,12 );
	test.insert_edge(2,1 ,int(), 0,4 );
	test.insert_edge(2,4 ,int(), 0,14 );
	test.insert_edge(3,2 ,int(), 0,9 );
	test.insert_edge(3,5 ,int(), 0,20 );
	test.insert_edge(4,3 ,int(), 0,7 );
	test.insert_edge(4,5 ,int(), 0,4 );
	test.Ford_Fulkerson();
	test.maxflow_push_relabel();
	test.maxflow_relabel_to_front();
	test.traverse_edge(print);
	return 0;
}
*/