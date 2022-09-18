#pragma once
#ifndef _IMTREE_HPP_
#define _IMTREE_HPP_

#include "../IMC.h"

#include "IMDS.hpp"

namespace IM { namespace DS {
// Namespace use captial at every single word
// Class objs use camelCase
// Members use underline

// Declaration of template class
template <typename T> class treeNode;
template <typename T> class tree;
template <typename T> class binTree;
template <typename T> class heap;
template <typename T> class rbTree;

template <typename T> 
	binTree<T> add_tree(const binTree<T> &,const binTree<T> &);

template <typename T> class treeNode
{
	friend class tree<T>;
	friend class binTree<T>;

	friend binTree<T> add_tree(const binTree<T> &tree_add,const binTree<T> &tree_toadd);
	//template <typename T> friend binTree<T> add_tree(const binTree<T> &tree_add,const binTree<T> &tree_toadd);
public:
	int has_nodes = 0;
	int self_layer = 1; //default layer is 1;be careful while in for 
	treeNode()
	{
		this->ordinal=0;
		this->has_nodes=0;
		this->data;
	}
	treeNode(int has_nodes,T data)
	{
		this->has_nodes = has_nodes;
		this->data = data;
	}
	treeNode(int has_nodes,int self_layer,T data)
	{
		this->has_nodes = has_nodes;
		this->self_layer = self_layer;
		this->data = data;
	}
protected:
	treeNode* sibling = nullptr;
	std::vector<treeNode*>ptrs;
};

/* IMTree */
// @Members:
//		* node_sum:the num of total nodes of IMtree
//		* layer_num: current layer
//		* head_ptr:the root node
//		* ope_ptr:current operation node
//	    * parent_ptr:ptrs of ope_ptr
//      * map<int, tuple<int, treeNode<T>*>>layer:key is layernum.data is a tuple (layer_nodesum,layer_ptr)
template <typename T> class tree
{
public:

	int nodes_sum; // dynamically change as the struct change.
	int layer_num = 1;	// current existing layer num.
	int sum_nodes(unsigned int layer_num)
	{
		auto nodes_sum_lambda = [this](int layer_num) ->int {int sum = 0; for (int i = 0; i < layer_num; i++)sum += std::get<0>(layer[i]); return sum; };
		return nodes_sum_lambda(layer_num);
	}

// struct
	//1.defalut,without head data.
	tree()
	{
		this->head_ptr = new treeNode<T>();
		this->ope_ptr = this->head_ptr;
		this->parent_ptr = this->head_ptr;
		this->head_ptr->has_nodes = 0;
		this->head_ptr->self_layer = 1;
		this->layer.insert(std::pair<int, std::tuple<int, treeNode<T>*>>(1, { 1, head_ptr }));
		this->nodes_sum = 1;
	}

	//2.head data.
	tree(T data):tree()
	{
		this->head_ptr->data = data;
	}

	//3.initialize the nodes with maximum  number of has_nodes children.
	//use an array to construct a n-way tree.
	tree(std::vector<T>list_node,int has_nodes,int n):tree(list_node[0])
	{
		treeNode<T>* save_ptr=nullptr;
		this->add_node(list_node[1], 1);
		for (int i = 2; i < n; i++)
		{
			if (this->parent_ptr->has_nodes < has_nodes)
			{
				this->add_node(list_node[i], 0);
			}
			else
			{
				std::tuple<int, int>temp = this->return_ptr(this->parent_ptr);
				this->change_ope(std::get<0>(temp), std::get<1>(temp));
				if (this->ope_ptr->sibling == nullptr)
				{
					std::tuple<int, int>temp_next = this->return_ptr(std::get<1>(this->layer[this->ope_ptr->self_layer+1]));
					this->change_ope(std::get<0>(temp_next),std::get<1>(temp_next));
					this->add_node(list_node[i], 1);
				}
				else
				{
					temp = this->return_ptr(this->ope_ptr->sibling);
					this->change_ope(std::get<0>(temp), std::get<1>(temp));
					this->add_node(list_node[i], 1);
				}
			}
		}
	}
	// destruct
	~tree()
	{
		
	}

	T operator!()
	{
		return this->ope_ptr->data;
	}


	T operator[](int serial_num)
	{
		return locate(serial_num)->data;
	}

	void operator>>(int serial_num)
	{
		this->ope_ptr = locate(serial_num);
	}

	// operation overload
	tree& operator=(tree& other)//problem
	{
		delete this->head_ptr;
		this->layer.clear();
		this->head_ptr = new treeNode<T>(other.head_ptr->has_nodes,other.head_ptr->self_layer,other.head_ptr->data);
		treeNode<T>* ope_ptr = this->head_ptr;
		this->layer.insert({ ope_ptr->self_layer,{(std::get<0>(other.layer[1])),ope_ptr} });
		for (int i=2;i<=other.layer_num;i++)
		{
			ope_ptr = std::get<1>(this->layer[i - 1]);
			while (1)
			{
				if (ope_ptr->sibling == nullptr)
				{
					break;
				}
				if (ope_ptr->has_nodes == 0)
				{
					ope_ptr = ope_ptr->sibling;
				}
				if (ope_ptr->has_nodes != 0)
				{
					break;
				}
			}
			treeNode<T>* temp=std::get<1>(other.layer[i]);
			treeNode<T>* cat_ptr;
			int current_node = 1;
			int sibling_times = 0;
			while (1)
			{
				treeNode<T>* add_ptr = new treeNode<T>(temp->has_nodes,temp->self_layer,temp->data);
				ope_ptr->ptrs.push_back(add_ptr);
				if (current_node == 1 && sibling_times == 0)
				{
					this->layer.insert({add_ptr->self_layer,{std::get<0>(other.layer[i]),add_ptr} });
				}
				if (current_node==1&&sibling_times!=0)
				{
					cat_ptr->ptrs[cat_ptr->has_nodes - 1]->sibling = ope_ptr->ptrs[0];
				}
				else
				{
					if (current_node==1)
					{

					}
					else
					{
						ope_ptr->ptrs[current_node-2]->sibling = add_ptr;
					}
				}
				current_node++;
				if (temp->sibling==nullptr)
				{
					break;
				}
				if (current_node!=ope_ptr->has_nodes+1)
				{
					temp = temp->sibling;
					sibling_times++;
				}
				else
				{
					cat_ptr = ope_ptr;
					while (1)
					{
						if (ope_ptr->sibling==nullptr)
						{
							break;
						}
						if (ope_ptr->has_nodes==0)
						{
							ope_ptr = ope_ptr->sibling;
						}
						if(ope_ptr->has_nodes!=0)
						{
							break;
						}
					}
					current_node = 1;
				}
			}
		}
		this->nodes_sum = other.nodes_sum;
		this->layer_num = other.layer_num;
		std::tuple<int, int>temp = return_ptr(other.ope_ptr);
		this->change_ope(std::get<0>(temp), std::get<1>(temp));
		return *this;
	}
	
	// pure conversion
	std::vector<T> tree2arr()
	{
		treeNode<T>* ptr_temp = this->ope_ptr;
		std::vector<T>v;
		for (int i=1;i<=this->layer_num;i++)
		{
			this->ope_ptr = std::get<1>(this->layer[i]);
			while (this->ope_ptr != nullptr)
			{
				v.push_back(this->ope_ptr->data);
				if (this->ope_ptr->sibling==nullptr)
				{
					break;
				}
				this->ope_ptr=this->ope_ptr->sibling;
			}
		}
		this->ope_ptr = ptr_temp;
		return v;
	}

	void assign_val(T data)
	{
		this->ope_ptr->data = data;
	}

	treeNode<T>* tail_end(treeNode<T>* ptrs)//return the first branch ptrs(without children)
	{
		while (1)
		{
			if (ptrs->has_nodes == 0)
			{
				return ptrs;
			}
			ptrs = ptrs->ptrs[0];
		}
	}
// locate: locate the node from a layer_num and layer serial num , change ope_ptr to the node. 
// only can find existing nodes
// para:layer_num,list_num
// return: treeNode<T>*
	treeNode<T>* locate(unsigned int layer_num, unsigned int list_num)
	{
		try
		{
			if (layer_num > this->layer_num)
			{
				throw - 1;
			}
			else
			{
				if (this->sum_nodes(layer_num) + list_num > this->nodes_sum)
				{
					throw - 1;
				}
				else
				{
					treeNode<T>* temp;
					treeNode<T>* layer_ptr = std::get<1>(this->layer[layer_num]);
					temp = layer_ptr;
					if (temp->sibling == nullptr)
					{
						return temp;
					}
					else
					{
						for (int i = 1; i < list_num; i++)
						{

							temp = temp->sibling;
						}
						return temp;
					}
				}
			}
		}
		catch (...)
		{
			std::cout << "out of range!" << std::endl;
		}
	}

	treeNode<T>* locate(unsigned int serial_num)
	{
		try
		{
			if (serial_num > this->nodes_sum)
			{
				throw - 1;
			}
			else
			{
				int sum = 0;
				int i = 1;
				for (i = 1; i < this->layer_num; i++)
				{
					sum = this->sum_nodes(i);
					if (sum >= serial_num)
					{
						break;
					}
				}
				int layer_num = i - 1;
				sum = this->sum_nodes(i - 1);
				int list_num = serial_num - sum;
				locate(layer_num, list_num);
			}
		}
		catch (...)
		{
			std::cout << "out of range!" << std::endl;
			return this->ope_ptr;
		}
	}

	std::tuple<int, int> return_ope()
	{
		treeNode<T>* ptrs = this->ope_ptr;
		int layer_num = ptrs->self_layer;
		int list_num = 0;
		while (ptrs != nullptr)
		{
			if (ptrs->sibling == nullptr)
			{
				break;
			}
			ptrs = ptrs->sibling;
			list_num += 1;
		}
		list_num = std::get<0>(this->layer[ptrs->self_layer]) - list_num;
		std::tuple<int, int>temp{ layer_num,list_num };
		printf("-----------------------------\n");
		printf(">>ope_ptr\n");
		printf("Layer:%10d\n",layer_num);
		printf("List: %10d\n",list_num);
		printf("Value:%10d\n", this->ope_ptr->data);
		printf("-----------------------------\n");
		return temp;
	}

	treeNode<T>* return_ope_ptr()
	{
		return this->ope_ptr;
	}

	treeNode<T>* return_parent()
	{
		return this->parent_ptr;
	}

	treeNode<T>* return_ancestor()//return ancestor of the ope_ptr
	{
		if (this->ope_ptr==this->parent_ptr)
		{
			return this->ope_ptr;
		}
		else
		{
			treeNode<T>* temp = std::get<1>(this->layer[this->parent_ptr->self_layer - 1]);
			while (1)
			{
				for (int i=0;i<temp->has_nodes;i++)
				{
					if (temp->ptrs[i]==this->parent_ptr)
					{
						return temp;
					}
				}
				temp = temp->sibling;
			}
		}
	}

	treeNode<T>* return_head_ptr()
	{
		return this->head_ptr;
	}

	std::tuple<int, int> return_ptr(treeNode<T>* ptrs)
	{
		int layer_num = ptrs->self_layer;
		int list_num = 0;
		while (ptrs != nullptr)
		{
			if (ptrs->sibling == nullptr)
			{
				break;
			}
			ptrs = ptrs->sibling;
			list_num += 1;
		}
		list_num = std::get<0>(this->layer[ptrs->self_layer]) - list_num;
		std::tuple<int, int>temp{ layer_num,list_num };
		return temp;
	}

	treeNode<T>* return_front(treeNode<T>* ptrs)
	{
		treeNode<T>* previous_ope = this->ope_ptr;
		treeNode<T>* previous_parent = this->parent_ptr;
		std::tuple<int, int>temp = this->return_ptr(ptrs);
		this->change_ope(std::get<0>(temp), std::get<1>(temp));
		if (std::get<1>(temp) == 1)
		{
			return this->locate(ptrs->self_layer - 1, std::get<0>(this->layer[ptrs->self_layer - 1]));
		}
		else
		{
			return this->parent_ptr->ptrs[std::get<1>(temp) - 2];
		}
		this->ope_ptr = previous_ope;
		this->parent_ptr = previous_parent;
	}

	treeNode<T>* return_back(treeNode<T>* ptrs)
	{
		if (ptrs->sibling!=nullptr)
		{
			return ptrs->sibling;
		}
		else
		{
			if (ptrs->self_layer==this->layer_num)
			{
				return ptrs->sibling;
			}
			else
			{
				return std::get<1>(this->layer[ptrs->self_layer + 1]);
			}
		}
	}
	

	void jumpto_sibling()
	{
		if (this->ope_ptr->sibling==nullptr)
		{
			try
			{
				throw "error!";
			}
			catch (...)
			{
				std::cout << "The sibling is nullptr!" << std::endl;
				return;
			}
		}
		else
		{
			this->change_ope(this->ope_ptr->sibling);
		}
	}

	void jumpto_layerptr(int layer)//choose a layer
	{
		std::tuple<int, int>temp = this->return_ptr(std::get<1>(this->layer[layer]));
		change_ope(std::get<0>(temp), std::get<1>(temp));
	}

	void jumpto_layerlast(int layer)//choose a layer
	{
		treeNode<T>* temp_ptr = std::get<1>(this->layer[layer]);
		while (temp_ptr->sibling!=nullptr)
		{
			temp_ptr = temp_ptr->sibling;
		}
		std::tuple<int, int>temp = this->return_ptr(temp_ptr);
		change_ope(std::get<0>(temp), std::get<1>(temp));
	}

	void jumpto_layerptr()//default layer is tree`s layer_num
	{
		std::tuple<int, int>temp = this->return_ptr(std::get<1>(this->layer[this->layer_num]));
		change_ope(std::get<0>(temp), std::get<1>(temp));
	}

	void jumpto_layerlast()//default layer is tree`s layer_num
	{
		treeNode<T>* temp_ptr = std::get<1>(this->layer[this->layer_num]);
		while (temp_ptr->sibling!=nullptr)
		{
			temp_ptr = temp_ptr->sibling;
		}
		std::tuple<int, int>temp = this->return_ptr(temp_ptr);
		change_ope(std::get<0>(temp), std::get<1>(temp));
	}

	void change_ope(unsigned int layer_num, unsigned int list_num)
	//change ope_ptr and parent_ptr
	//para:layer_num,list_num
	//usage other method:locate
	{
		
		if (layer_num > this->layer_num)
		{
			throw "out of range!";
		}
		else
		{
			if (this->sum_nodes(layer_num) + list_num > this->nodes_sum)
			{
				throw "out of range!";
			}
			else
			{
				if (layer_num == 1)
				{
					this->ope_ptr = this->head_ptr;
					this->parent_ptr = this->ope_ptr;
				}
				else
				{
					this->ope_ptr = this->locate(layer_num, list_num);
					treeNode<T>* upper_layer_ptr = std::get<1>(this->layer[layer_num - 1]);
					bool out_of_while = 0;
					while (upper_layer_ptr != nullptr && !out_of_while)
					{
						if (upper_layer_ptr->sibling == nullptr)
						{
							this->parent_ptr = upper_layer_ptr;
							break;
						}
						upper_layer_ptr = upper_layer_ptr->sibling;
						for (int i = 0; i < upper_layer_ptr->ptrs.size(); i++)
						{
							if (upper_layer_ptr->ptrs[i] == this->ope_ptr)
							{
								this->parent_ptr = upper_layer_ptr;
								out_of_while = 1;
								break;
							}
						}
					}
				}
			}
		}
	}

	void change_ope(treeNode<T>* ptrs)
	{
		auto temp=this->return_ptr(ptrs);
		change_ope(std::get<0>(temp), std::get<1>(temp));
	}

	std::vector<std::tuple<int, int>> find_bfs(T data)
	{
		treeNode<T>* ptrs;
		std::vector<std::tuple<int, int>>save_ptrs;
		int num = 0;
		printf("----------------------------------\n");
		printf("BFS Matched nodes:\n");
		printf("%7s  %9s|%-9s\n","Num","Layer","Serial");
		for (int i = 0; i < this->layer_num; i++)
		{
			ptrs = std::get<1>(this->layer[i + 1]);
			int list_num = 1;
			while (ptrs != nullptr)
			{
				if (ptrs->data == data)
				{
					save_ptrs.push_back({ i + 1,list_num });
				}
				ptrs = ptrs->sibling;
				list_num += 1;
			}
		}
		for (int i = 0; i < save_ptrs.size(); i++)
		{
			printf("|%6d  (%8d,%-8d)\n", i + 1, std::get<0>(save_ptrs[i]), std::get<1>(save_ptrs[i]));
		}
		printf("----------------------------------\n");
		return save_ptrs;
	}

	std::vector<std::tuple<int, int>> find_dfs(T data)
	{
		std::vector<std::tuple<int, int>>save_ptrs;
		printf("----------------------------------\n");
		printf("DFS Matched nodes:\n");
		printf("%7s  %9s|%-9s\n","Num","Layer","Serial");
		if (this->head_ptr->data == data)
		{
			save_ptrs.push_back({ 1,1 });
		}
		find_dfs_branch(save_ptrs, this->head_ptr, data);
		for (int i = 0; i < save_ptrs.size(); i++)
		{
			printf("|%6d  (%8d,%-8d)\n", i + 1, std::get<0>(save_ptrs[i]), std::get<1>(save_ptrs[i]));
		}
		printf("----------------------------------\n");
		return save_ptrs;
	}

	int find_child_value(treeNode<T>* ptrs,const T& data)
	{
		for (int i=0;i<ptrs->ptrs.size();i++)
		{
			if (ptrs->ptrs[i]->data==data)
			{
				return i + 1;
			}
		}
		return -1;
	}

	void add_node(T data, int direction)
	{
		if (direction == 1)
		{
			if (!this->ope_ptr->has_nodes)
			{
				this->nodes_sum += 1;
				treeNode<T>* save_ptr = this->parent_ptr;
				treeNode<T>* temp;
				this->parent_ptr = this->ope_ptr;
				this->ope_ptr->has_nodes += 1;
				this->ope_ptr->ptrs.push_back(temp);
				this->ope_ptr->ptrs[this->ope_ptr->has_nodes - 1] = new treeNode<T>;
				this->ope_ptr->ptrs[this->ope_ptr->has_nodes - 1]->data = data;
				this->ope_ptr = this->parent_ptr->ptrs[this->parent_ptr->has_nodes - 1];
				if (this->layer.count(this->parent_ptr->self_layer+1))
				{	
					std::get<0>(this->layer[this->parent_ptr->self_layer+1]) += 1;
					std::tuple<int, int>get_pos = return_ptr(this->parent_ptr);
					this->ope_ptr->self_layer = this->layer_num;
					int i = 1;
					while (1)
					{
						if (save_ptr->ptrs[std::get<1>(get_pos) - 1 - i]->ptrs.size() == 0)
						{
							i++;
						}
						else
						{
							break;
						}
					}
					save_ptr->ptrs[std::get<1>(get_pos) - i-1]->ptrs[(save_ptr->ptrs[std::get<1>(get_pos) - i-1]->has_nodes) - 1]->sibling = this->ope_ptr;
					if (std::get<1>(get_pos) < save_ptr->has_nodes && save_ptr->ptrs[std::get<1>(get_pos)]->ptrs.size() != 0)
					{
						this->ope_ptr->sibling = save_ptr->ptrs[std::get<1>(get_pos)]->ptrs[0];
					}
				}
				else
				{
					this->layer_num += 1;
					this->ope_ptr->self_layer = this->parent_ptr->self_layer+1;
					this->layer.insert(std::pair<int, std::tuple<int, treeNode<T>*>>({ this->layer_num, { 1,this->ope_ptr } }));
				}
			}
			else
			{
				try
				{
					throw "error!";
				}
				catch (...)
				{
					std::cout << "The root already has a child,use direction 0 to add." << std::endl;
					return;
				}
			}
		}
		else if (direction == 0)
		{
			if (this->parent_ptr->has_nodes!=0)
			{
				this->nodes_sum += 1;
				this->parent_ptr->has_nodes += 1;
				treeNode<T>* temp;
				this->parent_ptr->ptrs.push_back(temp);
				this->parent_ptr->ptrs[this->parent_ptr->has_nodes - 1] = new treeNode<T>;
				this->parent_ptr->ptrs[this->parent_ptr->has_nodes - 1]->data = data;
				this->ope_ptr->sibling = this->parent_ptr->ptrs[this->parent_ptr->has_nodes - 1];
				this->ope_ptr = this->parent_ptr->ptrs[this->parent_ptr->has_nodes - 1];
				std::get<0>(this->layer[this->layer_num]) += 1;
				this->ope_ptr->self_layer = this->parent_ptr->self_layer + 1;
				if (this->parent_ptr->sibling != nullptr)
				{
					if (this->parent_ptr->sibling->ptrs.size() == 0)
					{
						return;
					}
					else
					{
						this->ope_ptr->sibling = this->parent_ptr->sibling->ptrs[0];
					}
				}
			}
			else
			{
				try
				{
					throw "error!";
				}
				catch (...)
				{
					std::cout << "The root already has no child!direction 0 must use with no child root." << std::endl;
					return;
				}
			}
		}
		else if (direction==-1)
		{
			if (this->parent_ptr != this->ope_ptr)
			{
				this->change_ope(this->parent_ptr);
				this->change_ope(this->parent_ptr);
				treeNode<T>* new_ptr = new treeNode<T>(0,data);
				new_ptr->self_layer = this->ope_ptr->self_layer + 1;
				this->ope_ptr->ptrs.push_back(new_ptr);
				this->ope_ptr->has_nodes++;
				std::get<0>(this->layer[this->ope_ptr->ptrs[this->ope_ptr->has_nodes - 1]->self_layer])++;
				this->ope_ptr->ptrs[this->ope_ptr->has_nodes - 2]->sibling = this->ope_ptr->ptrs[this->ope_ptr->has_nodes - 1];
				this->nodes_sum++;
				this->change_ope(new_ptr);
			}
			else			
			{
				treeNode<T>* temp_head = this->head_ptr;
				this->head_ptr = new treeNode<T>(1, data);
				this->head_ptr->ptrs.push_back(temp_head);
				this->layer_num++;
				this->nodes_sum++;
				this->layer.insert({ this->layer_num,{std::get<0>(this->layer[this->layer_num-1]),std::get<1>(this->layer[this->layer_num-1])} });
				int temp_former = std::get<0>(this->layer[1]);
				treeNode<T>* ptr_temp_former = std::get<1>(this->layer[1]);
				std::get<0>(this->layer[1]) = 1;
				std::get<1>(this->layer[1]) = this->head_ptr;
				int count = 2;
				int temp_lat;
				treeNode<T>* ptr_temp_lat;
				while (count!=this->layer_num)
				{
					temp_lat = std::get<0>(this->layer[count]);
					ptr_temp_lat = std::get<1>(this->layer[count]);
					std::get<0>(this->layer[count]) = temp_former;
					std::get<1>(this->layer[count]) = ptr_temp_former;
					count++;
					temp_former = temp_lat;
					ptr_temp_former = ptr_temp_lat;
				}
				for (int i = 2; i <= this->layer_num; i++)
				{
					std::get<1>(this->layer[i])->self_layer++;
					treeNode<T>* ope_temp = std::get<1>(this->layer[i]);
					while (1)
					{
						if (ope_temp->sibling == nullptr)
						{
							break;
						}
						ope_temp = ope_temp->sibling;
						ope_temp->self_layer++;
					}
				}
				this->parent_ptr = this->head_ptr;
				this->ope_ptr = this->head_ptr;
			}
		}
		else
		{
			try
			{
				throw "error!";
			}
			catch (...)
			{
				std::cout << "error add mode!" << std::endl;
				return;
			}
		}
	}
	void insert(int serial_num, T data)
	{
		if (serial_num > this->nodes_sum)
		{
			throw "out of range!";
		}
		else
		{
			int sum = 0;
			int i = 1;
			for (i = 1; i < this->layer_num; i++)
			{
				sum = this->sum_nodes(i);
				if (sum >= serial_num)
				{
					break;
				}
			}
			int layer_num = i - 1;
			sum = this->sum_nodes(i - 1);
			int list_num = serial_num - sum;
			insert(layer_num, list_num, data);
		}
	}

	void insert(int layer_num, int list_num, T data)
	{
		if (layer_num > this->layer_num)
		{
			throw "out of range";
		}
		else
		{
			if (list_num > std::get<0>(this->layer[layer_num]))
			{
				throw "out of range";
			}
			else
			{
				treeNode<T>* moving_ptr = std::get<1>(this->layer[layer_num]);
				treeNode<T>* insert_ptr = new treeNode<T>;
				if (list_num == 1)
				{
					insert_ptr->sibling = moving_ptr;
					insert_ptr->self_layer = layer_num;
					insert_ptr->data = data;
					std::get<1>(this->layer[layer_num]) = insert_ptr;
					std::get<0>(this->layer[layer_num]) += 1;
					this->change_ope(layer_num, list_num);
					this->parent_ptr->ptrs.insert(this->parent_ptr->ptrs.begin(), insert_ptr);
					this->parent_ptr->has_nodes += 1;
					std::cout << std::endl;
					this->return_ptr(this->parent_ptr);
				}
				else
				{
					for (int i = 0; i < list_num - 2; i++)
					{
						moving_ptr = moving_ptr->sibling;
					}
					treeNode<T>* next_ptr = moving_ptr->sibling;
					moving_ptr->sibling = insert_ptr;
					insert_ptr->sibling = next_ptr;
					insert_ptr->self_layer = layer_num;
					insert_ptr->data = data;
					std::get<0>(this->layer[layer_num]) += 1;
					this->change_ope(layer_num, list_num);
					this->parent_ptr->ptrs.insert(this->parent_ptr->ptrs.begin() + list_num - 1, insert_ptr);
					this->parent_ptr->has_nodes += 1;
				}
			}
		}
	}

	void del_node()//delete ope_ptr`s treeNode<T> problem
	{
		this->parent_ptr->has_nodes -= 1;
		int i = 0;
		for (i = 0; i < this->parent_ptr->ptrs.size(); i++)
		{
			if (this->parent_ptr->ptrs[i] == this->ope_ptr)
			{
				break;
			}
		}
		this->nodes_sum -= 1;
		std::get<0>(this->layer[this->ope_ptr->self_layer]) -= 1;
		if (this->parent_ptr->ptrs.size() == 1)
		{
			for (int j = 0; j < this->ope_ptr->ptrs.size(); j++)
			{
				this->parent_ptr->ptrs.push_back(this->ope_ptr->ptrs[j]);
				this->parent_ptr->has_nodes++;
			}
			this->layer.erase(this->ope_ptr->self_layer);
			this->layer_num -= 1;
			std::map<int, std::tuple<int, treeNode<T>*>>temp;
			for (int j = 1; j < this->ope_ptr->self_layer; j++)
			{
				temp.insert(std::pair<int, std::tuple<int, treeNode<T>*>>(j, this->layer[j]));
			}
			for (int j = this->ope_ptr->self_layer; j <= this->layer_num; j++)
			{
				temp.insert(std::pair<int, std::tuple<int, treeNode<T>*>>(j, { std::get<0>(this->layer[j + 1]), std::get<1>(this->layer[j + 1]) }));
			}
			this->layer = temp;
			delete this->ope_ptr;
			std::tuple<int, int>temp_ope_ptr = this->return_ptr(parent_ptr);
			this->change_ope(std::get<0>(temp_ope_ptr), std::get<1>(temp_ope_ptr));
		}
		else
		{
			if (this->parent_ptr->ptrs.size() < i + 2)
			{
				this->parent_ptr->ptrs[i - 1]->sibling = nullptr;
			}
			else
			{
				this->parent_ptr->ptrs[i - 1]->sibling = this->parent_ptr->ptrs[i + 1];
			}
			for (int j = 0; j < this->ope_ptr->ptrs.size(); j++)
			{
				this->parent_ptr->ptrs[i - 1]->ptrs.push_back(this->ope_ptr->ptrs[j]);
				this->parent_ptr->ptrs[i - 1]->has_nodes++;
			}
			delete this->ope_ptr;
			std::tuple<int, int>temp_ope_ptr = this->return_ptr(parent_ptr);
			this->change_ope(std::get<0>(temp_ope_ptr), std::get<1>(temp_ope_ptr));
		}
	}

	void del_node(unsigned int layer_num, unsigned int list_num)
	{
		if (layer_num > this->layer_num)
		{
			throw "out of range!";
		}
		else
		{
			if (list_num > std::get<0>(this->layer[layer_num]))
			{
				throw "out of range!";
			}
			else
			{
				treeNode<T>* temp = this->locate(layer_num, list_num);
				if (temp == this->ope_ptr)
				{
					del_node();
				}
				else
				{
					std::tuple<int, int>temp_tuple = this->return_ope();
					this->change_ope(layer_num, list_num);
					del_node();
					this->change_ope(std::get<0>(temp_tuple), std::get<1>(temp_tuple));
				}
			}
		}
	}

	void del_node(unsigned int serial_num)
	{
		if (serial_num > this.nodes_sum)
		{
			throw "out of range!";
		}
		else
		{
			std::tuple<int, int>temp_tuple = this->return_ptr(this->locate(serial_num));
			del_node(std::get<0>(temp_tuple), std::get<1>(temp_tuple));
		}
	}

	void del_ptr_tree(treeNode<T>* ptrs)//problem
	{
		if (sub_tree_layer(ptrs) == this->layer_num)
		{
			int layer = ptrs->self_layer;
			treeNode<T>* temp = ptrs;
			while (1)
			{
				if (std::get<1>(this->layer[layer]) == temp)
				{
					break;
				}
				temp = temp->ptrs[0];
				layer++;
				if (layer == this->layer_num+1)
				{
					break;
				}
			}
			if (layer==this->layer_num+1)
			{
				return;
			}
			else
			{
				for (int i=layer;i<=this->layer_num;i++)
				{
					this->layer.erase(i);
				}
			}
		}
		else
		{
			if (ptrs==std::get<1>(this->layer[ptrs->self_layer]))
			{
				if (ptrs->sibling!=nullptr)
				{

				}
				for (int i=ptrs->self_layer;i<=this->layer_num;i++)
				{
					this->layer.erase(i);
				}
			}
		}
	}
	int get_tree_nodes(treeNode<T>* ptrs, int sum)
	{
		if (ptrs->ptrs.size() == 0)
		{
			return 0;
		}
		else
		{
			sum += ptrs->has_nodes;
			for (int i = 0; i <= ptrs->has_nodes - 1; i++)
			{
				sum += get_tree_nodes(ptrs->ptrs[i], 0);
			}
			return sum;
		}
	}

	int sub_layer_nodes(treeNode<T>* ptrs, unsigned int layer_num, int sum)
	{
		static int head_layer = ptrs->self_layer;
		if (layer_num > this->layer_num - head_layer)
		{
			try
			{
				throw "out of range!";
			}
			catch (...)
			{
				std::cout << "out of range!" << std::endl;
				return -1;
			}
		}
		else
		{
			if (ptrs->self_layer == layer_num + head_layer)
			{
				return 1;
			}
			else
			{
				for (int i = 0; i <= ptrs->has_nodes - 1; i++)
				{
					sum += sub_layer_nodes(ptrs->ptrs[i], layer_num, 0);
				}
				return sum;
			}
		}
	}

	int sub_tree_layer(treeNode<T>* ptrs)
	{
		return tail_end(ptrs)->self_layer;
	}


	void describe(treeNode<T>* ptrs)
	{
		std::tuple<int, int>temp = this->return_ptr(ptrs);
		printf("-----------------------------\n");
		printf(">>ptrs\n");
		printf("Layer:%10d\n",ptrs->self_layer);
		printf("List :%10d\n",std::get<1>(temp));
		printf("Value:%10d\n", ptrs->data);
		printf("-----------------------------\n");
	}

	void describe_tree() //general description
	{
		std::vector<T>array = this->tree2arr();
		printf("--------------------------------------------\n");
		printf(">>Tree Info:\n");
		printf("Layer number: %-3d\n", this->layer_num);
		printf("Nodes sum: %-7d\n\n", this->nodes_sum);
		printf("--Values listed by serial num:\n");
		int sum = 0;
		for (int i=0;i<this->nodes_sum;i++)
		{
			printf("%-7d\t",array[i]);
			sum++;
			if (sum == 5)
			{
				printf("\n");
			}
		}
		printf("\n");
		printf("--------------------------------------------\n\n");
	}
	
	void route_front(std::string name)
	{
		printf("-----------------------------------------------------------------------------------\n");
		printf(">>Tree %s lists:\n",name);
		printf("    Layer|Serial\n");
		printf("(       1,1       )  ||  data=%-20f\n",this->head_ptr->data);
		route_front_branch(head_ptr);
		printf("-----------------------------------------------------------------------------------\n");
	}

	void route_front()
	{
		printf("-----------------------------------------------------------------------------------\n");
		printf("    Layer|Serial\n");
		printf("(       1,1       )  ||  data=%-20f\n",this->head_ptr->data);
		route_front_branch(head_ptr);
		printf("-----------------------------------------------------------------------------------\n");
	}

	void route_back(std::string name)
	{
		printf("-----------------------------------------------------------------------------------\n");
		printf(">>Tree %s lists:\n",name);
		printf("    Layer|Serial\n");
		printf("(       1,1       )  ||  data=%-20f\n", this->head_ptr->data);
		route_back_branch(head_ptr);
		printf("-----------------------------------------------------------------------------------\n");
	}

	void route_back()
	{
		printf("-----------------------------------------------------------------------------------\n");
		printf("    Layer|Serial\n");
		printf("(       1,1       )  ||  data=%-20f\n", this->head_ptr->data);
		route_back_branch(head_ptr);
		printf("-----------------------------------------------------------------------------------\n");
	}

	void route_front_assign(treeNode<T>* ptrs,int layer)
	{
		route_front_assign_branch(ptrs,layer);
	}

	void route_back_assign(treeNode<T>* ptrs,int layer)
	{
		route_back_assign_branch(ptrs,layer);
	}

	void exhaust_all()//exhausion
	{

	}

	void draw_tree()
	{


	}
	
	void clear()
	{
		this->del_node_clear();
		this->ope_ptr = this->head_ptr;
		this->parent_ptr = this->head_ptr;
		this->nodes_sum = 1;
		this->layer_num = 1;
		this->layer.clear();
		this->layer.insert(std::pair<int, std::tuple<int, treeNode<T>*>>(1, { 1, head_ptr }));
	}

protected:
	treeNode<T>* head_ptr;
	treeNode<T>* ope_ptr;
	treeNode<T>* parent_ptr;
	std::map<int, std::tuple<int, treeNode<T>*>>layer;

	// hide the implication
	void del_node_clear()
	{
		static int layer_num = this->layer_num;
		if (layer_num == 1)// if is at the headptr layer
		{
			return;
		}
		this->ope_ptr = std::get<1>(this->layer[layer_num]); //ope_ptr's layer is always the last layer
		for (int i = 1; i <= std::get<0>(this->layer[layer_num]); i++)
		{
			treeNode<T>* temp = this->ope_ptr->sibling;
			delete this->ope_ptr;
			this->ope_ptr = temp;
		}
		layer_num -= 1;
		del_node_clear();
	}

	void find_dfs_branch(std::vector<std::tuple<int, int>>& save_ptrs, treeNode<T>* ptrs, T data)
	{
		if (ptrs->has_nodes == 0)
		{
			return;
		}
		else
		{
			for (int i = 0; i < ptrs->has_nodes; i++)
			{
				if (ptrs->ptrs[i]->data == data)
				{
					save_ptrs.push_back(this->return_ptr(ptrs->ptrs[i]));
				}
				find_dfs_branch(save_ptrs, ptrs->ptrs[i], data);
			}
		}
	}

	void route_front_branch(treeNode<T>* ptrs)
	{
		if (ptrs->has_nodes == 0)
		{
			return;
		}
		else
		{

			std::tuple<int, int>temp_parent = this->return_ptr(ptrs);
			for (int i = 0; i < ptrs->has_nodes; i++)
			{
				std::tuple<int, int>temp = this->return_ptr(ptrs->ptrs[i]);
				printf("(%8d,%-8d)  ||  data=%-15f  ||  parent(%8d,%-8d)\n", std::get<0>(temp), std::get<1>(temp), ptrs->ptrs[i]->data, std::get<0>(temp_parent), std::get<1>(temp_parent));
				this->route_front_branch(ptrs->ptrs[i]);
			}
		}
	}

	void route_front_assign_branch(treeNode<T>* ptrs,int layer)
	{
		if (ptrs->has_nodes == 0)
		{
			return;
		}
		else
		{
			for (int i = 0; i < ptrs->has_nodes; i++)
			{
				ptrs->ptrs[i]->self_layer = layer;
				this->route_front_assign_branch(ptrs->ptrs[i],layer+1);
			}
		}
	}


	void route_back_branch(treeNode<T>* ptrs)
	{
		if (ptrs->has_nodes == 0)
		{
			return;
		}
		else
		{

			std::tuple<int, int>temp_parent = this->return_ptr(ptrs);
			for (int i = ptrs->has_nodes - 1; i != 0; i--)
			{
				std::tuple<int, int>temp = this->return_ptr(ptrs->ptrs[i]);
				printf("(%8d,%-8d)  ||  data=%-15f  ||  parent(%8d,%-8d)\n", std::get<0>(temp), std::get<1>(temp), ptrs->ptrs[i]->data, std::get<0>(temp_parent), std::get<1>(temp_parent));
				this->route_back_branch(ptrs->ptrs[i]);
			}
		}
	}

	void route_back_assign_branch(treeNode<T>* ptrs,int layer)
	{
		if (ptrs->has_nodes == 0)
		{
			return;
		}
		else
		{
			for (int i = ptrs->has_nodes - 1; i != 0; i--)
			{
				ptrs->ptrs[i]->self_layer = layer;
				this->route_back_assign_branch(ptrs->ptrs[i],layer+1);
			}
		}
	}

private:
};


template <typename T> class binTree :public tree<T>
{
	friend binTree<T> add_tree(const binTree<T> &tree_add,const binTree<T> &tree_toadd);
public:

	binTree() :tree<T>() {};
	binTree(T data) :tree<T>(data) {};
	binTree(T* array, int n) :tree<T>(array, 2, n) {};
	~binTree() {};

	//binTree& operator=(binTree& other)
	//{
	//	
	//}
	void add_node(T data,int direction)
	{
		if (direction == 1)
		{
			if (this->ope_ptr->has_nodes == 2)
			{
				throw "out of range!";
			}
			else
			{
			this->nodes_sum += 1;
			this->parent_ptr = this->ope_ptr;
			this->ope_ptr->has_nodes += 1;
			treeNode<T>* temp;
			this->ope_ptr->ptrs.push_back(temp);
			this->ope_ptr->ptrs[this->ope_ptr->has_nodes - 1] = new treeNode<T>;
			this->ope_ptr->ptrs[this->ope_ptr->has_nodes - 1]->data = data;
			this->ope_ptr = this->parent_ptr->ptrs[this->parent_ptr->has_nodes - 1];
			this->layer_num += 1;
			this->ope_ptr->self_layer = this->layer_num;
			this->layer.insert(std::pair<int, std::tuple<int, treeNode<T>*>>(this->layer_num, { 1,this->ope_ptr }));
			}
		}
		else if (direction == 0)
		{
			if (this->parent_ptr == this->ope_ptr)
			{
				try
				{
					throw "error!This is the root treeNode!";
				}
				catch (...)
				{
					std::cout << "error!This is the root treeNode!" << std::endl;
					return;
				}
			}
			else
			{
				treeNode<T>* memo_ptr = this->parent_ptr;
				treeNode<T>* neighbor_ptr = this->parent_ptr;
				if (this->parent_ptr->has_nodes == 2)
				{
					int added_node = 0;
					while (this->parent_ptr->sibling != nullptr && added_node == 0)
					{
						if (this->parent_ptr->sibling->has_nodes < 2)
						{
							added_node++;
							neighbor_ptr = this->parent_ptr;
							this->parent_ptr = this->parent_ptr->sibling;
							this->nodes_sum += 1;
							this->parent_ptr->has_nodes++;
							treeNode<T>* temp;
							this->parent_ptr->ptrs.push_back(temp);
							this->parent_ptr->ptrs[this->parent_ptr->has_nodes - 1] = new treeNode<T>;
							this->parent_ptr->ptrs[this->parent_ptr->has_nodes - 1]->data = data;
							if (this->parent_ptr->has_nodes == 1)
							{
								neighbor_ptr->ptrs[neighbor_ptr->has_nodes - 1]->sibling = this->parent_ptr->ptrs[0];
							}
							else if (this->parent_ptr->has_nodes == 2)
							{
								this->parent_ptr->ptrs[this->parent_ptr->has_nodes - 2]->sibling = this->parent_ptr->ptrs[this->parent_ptr->has_nodes - 1];
							}
							this->ope_ptr = this->parent_ptr->ptrs[this->parent_ptr->has_nodes - 1];
							std::get<0>(this->layer[this->layer_num]) += 1;
							this->ope_ptr->self_layer = this->layer_num;
							this->parent_ptr = memo_ptr;
						}
						else if (this->parent_ptr->sibling->has_nodes == 2)
						{
							this->parent_ptr = this->parent_ptr->sibling;
						}
						else if (this->parent_ptr->sibling == nullptr)
						{
							try
							{
								this->parent_ptr = memo_ptr;
								throw "out of range!";
							}
							catch (...)
							{
								std::cout << "out of range!" << std::endl;
								return;
							}
						}
					}
				}
				else
				{
					this->nodes_sum += 1;
					this->parent_ptr->has_nodes++;
					treeNode<T>* temp;
					this->parent_ptr->ptrs.push_back(temp);
					this->parent_ptr->ptrs[this->parent_ptr->has_nodes - 1] = new treeNode<T>;
					this->parent_ptr->ptrs[this->parent_ptr->has_nodes - 1]->data = data;
					if (this->parent_ptr->has_nodes - 1 == 1)
					{
						std::get<1>(this->layer[this->layer_num])->sibling = this->parent_ptr->ptrs[this->parent_ptr->has_nodes - 1];
					}
					else
					{
						this->ope_ptr->sibling = this->parent_ptr->ptrs[this->parent_ptr->has_nodes - 1];
					}
					this->ope_ptr = this->parent_ptr->ptrs[this->parent_ptr->has_nodes - 1];
					std::get<0>(this->layer[this->layer_num]) += 1;
					this->ope_ptr->self_layer = this->layer_num;
				}
			}
		}
		else
		{
			try
			{
				throw "error!";
			}
			catch (...)
			{
				std::cout << "error add mode!" << std::endl;
				return;
			}
		}
	}
	treeNode<T>* left_child(treeNode<T>* ptrs)
	{
		if (ptrs->has_nodes == 0)
		{
			try
			{
				throw "error!";
			}
			catch (...)
			{
				std::cout << "This treeNode<T> doesn`t have left child." << std::endl;
			}
		}
		else
		{
			return ptrs->ptrs[0];
		}
	}

	treeNode<T>* right_child(treeNode<T>* ptrs)
	{
		if (ptrs->has_nodes == 0 || ptrs->has_nodes == 1)
		{
			try
			{
				throw "error!";
			}
			catch (...)
			{
				std::cout << "This treeNode<T> doesn`t have right child." << std::endl;
			}
		}
		else
		{
			return ptrs->ptrs[1];
		}
	}

	void node_reverse(treeNode<T>* ptrs)
	{
		if (ptrs->has_nodes!=2)
		{
			try
			{
				throw "error!";
			}
			catch (...)
			{
				std::cout << "This treeNode<T> doesn`t have two nodes." << std::endl;
				return;
			}
		}
		else
		{
				ptrs->ptrs.insert(ptrs->ptrs.begin(), ptrs->ptrs[1]);
				std::get<1>(this->layer[ptrs->ptrs[0]->self_layer]) = ptrs->ptrs[0];
				treeNode<T>* temp = ptrs->ptrs[2]->sibling;
				ptrs->ptrs[0]->sibling = ptrs->ptrs[1];
				ptrs->ptrs.pop_back();
				ptrs->ptrs[1]->sibling = temp;
		}
	}

protected:
};

template <typename T> binTree<T> add_tree(const binTree<T>& tree_first, const binTree<T>& tree_second)
{
	binTree<T>tree_added = tree_first;
	binTree<T>tree_toadd = tree_second;
	tree_added.ope_ptr->ptrs.push_back(tree_toadd.head_ptr);
	tree_added.ope_ptr->has_nodes++;
	treeNode<T>* temp_head = tree_added.ope_ptr->ptrs[0];
	std::map<int, std::tuple<int, treeNode<T>*>>temp_map;
	for (int i = 1; i <= tree_toadd.layer_num; i++)
	{
		bool not_to_add = 0;
		temp_map.insert({ i,{std::get<0>(tree_toadd.layer[i]),temp_head} });
		while (1)
		{
			if (temp_head->ptrs.size() == 0)
			{
				if (temp_head->sibling == nullptr)
				{
					not_to_add = 1;
					break;
				}
				else
				{
					temp_head = temp_head->sibling;
				}
			}
			else
			{
				temp_head = temp_head->ptrs[0];
				break;
			}
		}
		if (not_to_add == 1)
		{
			break;
		}
	}
	tree_added.nodes_sum += tree_toadd.nodes_sum;
	tree_added.route_front_assign(tree_added.ope_ptr->ptrs[0], tree_added.ope_ptr->self_layer + 2);
	if (tree_added.ope_ptr->has_nodes == 1)
	{
		if (tree_added.ope_ptr == std::get<1>(tree_added.layer[tree_added.ope_ptr->self_layer]))
		{
			if (tree_added.ope_ptr->self_layer + tree_toadd.layer_num >= tree_added.layer_num)
			{
				for (int i = tree_added.ope_ptr->self_layer + 1; i <= tree_added.layer_num; i++)
				{
					std::get<1>(tree_added.layer[i]) = std::get<1>(temp_map[i - tree_added.ope_ptr->self_layer]);
					std::get<1>(tree_added.layer[i])->self_layer = i;
					std::get<0>(tree_added.layer[i]) = std::get<0>(tree_added.layer[i]) + std::get<0>(temp_map[i - tree_added.ope_ptr->self_layer]);
				}
				for (int i = tree_added.layer_num + 1; i <= tree_added.ope_ptr->self_layer + tree_toadd.layer_num; i++)
				{
					tree_added.layer.insert({ i, {std::get<0>(temp_map[i - tree_added.ope_ptr->self_layer]) ,std::get<1>(temp_map[i - tree_added.ope_ptr->self_layer]) } });
					std::get<1>(tree_added.layer[i])->self_layer = i;
				}
				tree_added.layer_num = tree_added.ope_ptr->self_layer + tree_toadd.layer_num;
			}
			else
			{
				for (int i = tree_added.ope_ptr->self_layer + 1; i <= tree_added.ope_ptr->self_layer + tree_toadd.layer_num; i++)
				{
					std::get<1>(tree_added.layer[i]) = std::get<1>(temp_map[i - tree_added.ope_ptr->self_layer]);
					std::get<1>(tree_added.layer[i])->self_layer = i;
					std::get<0>(tree_added.layer[i]) = std::get<0>(tree_added.layer[i]) + std::get<0>(temp_map[i - tree_added.ope_ptr->self_layer]);
				}
			}
		}
		else
		{
			if (tree_added.ope_ptr->self_layer + tree_toadd.layer_num > tree_added.layer_num)
			{
				for (int i = tree_added.layer_num + 1; i <= tree_added.ope_ptr->self_layer + tree_toadd.layer_num; i++)
				{
					tree_added.layer.insert({ i, { std::get<0>(temp_map[i - tree_added.ope_ptr->self_layer]) ,std::get<1>(temp_map[i - tree_added.ope_ptr->self_layer]) } });
					std::get<1>(tree_added.layer[i])->self_layer = i;
				}
				tree_added.layer_num = tree_added.ope_ptr->self_layer + tree_toadd.layer_num;
			}
		}
	}
	else if (tree_added.ope_ptr->has_nodes == 2)
	{
		if (tree_toadd.layer_num + tree_added.ope_ptr->self_layer > tree_added.layer_num)
		{
			for (int i = tree_added.layer_num + 1; i <= tree_added.ope_ptr->self_layer + tree_toadd.layer_num; i++)
			{
				tree_added.layer.insert({ i, { std::get<0>(temp_map[i - tree_added.layer_num + 1]),std::get<1>(temp_map[i - tree_added.layer_num + 1]) } });
				std::get<1>(tree_added.layer[i])->self_layer = i;
			}
			tree_added.layer_num = tree_added.ope_ptr->self_layer + tree_toadd.layer_num;
		}
	}
	while (1)
	{
		treeNode<T>* toadd_node;
		treeNode<T>* added_node = tree_added.ope_ptr;
		if (added_node->sibling == nullptr)
		{
			treeNode<T>* row_ptr = std::get<1>(tree_added.layer[tree_added.layer_num]);
			while (1)
			{
				if (row_ptr->sibling == nullptr)
				{
					break;
				}
				row_ptr = row_ptr->sibling;
			}
			tree_added.change_ope(row_ptr);
			return tree_added;
		}
		else
		{
			int temp_layer = tree_added.ope_ptr->self_layer + 1;
			while (1)
			{
				added_node = added_node->sibling;
				if (added_node->has_nodes == 0)
				{
					if (added_node->sibling == nullptr)
					{
						break;
					}
					else
					{
						added_node = added_node->sibling;
					}
				}
				treeNode<T>* temp_sibling_ptr = added_node;
				while (added_node->ptrs.size() != 0)
				{
					added_node = added_node->ptrs[0];
					if (added_node->self_layer == temp_layer)
					{
						toadd_node = tree_added.ope_ptr->ptrs[tree_added.ope_ptr->has_nodes - 1];
						while (toadd_node->ptrs.size() != 0)
						{
							if (toadd_node->self_layer == temp_layer)
							{
								toadd_node->sibling = added_node;
								temp_layer++;
								break;
							}
							toadd_node = toadd_node->ptrs[toadd_node->has_nodes - 1];
						}
						if (added_node->ptrs.size() == 0)
						{
							break;
						}
					}
				}
				if (temp_layer == tree_added.layer_num)
				{
					break;
				}
				added_node = temp_sibling_ptr;
			}
		}
	}
}
/*
* heap
* @brief:A[PARENT(i)]>=A[i] || A[PARENT(i)]<=A[i]
*/
template <typename T> class heap :public binTree<T>
{
public:
	bool heap_ini = 0;
	bool heap_type;//max or min,1 is max,0 is min
	//useless,if you initialize with heap type 
	void heap_examine()
	{
		if (this->heap_ini != 1)
		{
			try {
				throw typename heap<T>::heap_exception();
			}
			catch (typename heap<T>::heap_exception& exception_tree)
			{
				exception_tree.list_all_warnings();
				return;
			}
		}
	}
	/*
	* @brief:heap array model,which is one of ways to carry out heap
	* @param:array position
	*/
	static int arr_leftnode(int i)
	{
		return 2 * i;
	}

	static int arr_rightnode(int i)
	{
		return 2 * i + 1;
	}

	static int arr_parentnode(int i)
	{
		return floor(i/2);
	}

	//1.vec_heaplify; 2.heaplify(dispose on tree model)
	//merely for heap array
	void vec_heaplify(int i)
	{
		if (this->heap_type == 1) //max_heap_heaplify
		{
			T l = arr_leftnode(i);
			T r = arr_rightnode(i);
			T largest;
			if (l<=this->heap_arr.size()&&this->heap_arr[l]>this->heap_arr[i])
			{
				largest = l;
			}
			else
			{
				largest = i;
			}
			if (r<=this->heap_arr.size()&&this->heap_arr[r]>this->heap_arr[largest])
			{
				largest = r;
			}
			if (largest!=i)
			{
				//exchange method
				T temp = this->heap_arr[i];
				this->heap_arr[i] = this->heap_arr[largest];
				this->heap_arr[largest] = temp;
				vec_heaplify(largest);
			}
		}
		else //min_heap_heaplify
		{

		}
	}

	/*
	* @brief:heap tree model heaplify
	* @param:null
	*/
	void tree_heaplify()
	{
		tree<T>::assign_val(this->heap_arr[0]);
		for (int i=1;i<this->heap_arr.size();i++)
		{

		}
	}

	std::vector<T> get_heap_arr()
	{
		return this->heap_arr;
	}

	heap(bool heap_type) :binTree<T>() { this->heap_type = heap_type; this->heap_ini = 1; };
	heap(bool heap_type, T data) :binTree<T>(data) { this->heap_type = heap_type; this->heap_ini = 1; };
	/*
	* @brief:heap build function with an array
	* @param:array
	*/
	heap(bool heap_type,T* heap_array, int heap_size) :binTree<T>() {
		this->heap_type = heap_type;
		this->heap_ini=1;
		for (int i = 0; i < heap_size; i++)
		{
			this->heap_arr.push_back(heap_array[i]);
		}
		//build heap for vector
		for (int i=floor(this->heap_arr.size()/2);i>=0;i--)
		{
			vec_heaplify(i);
		}
		//heaplify for tree
		tree_heaplify();
	};
	~heap() {};
private:
	std::vector<T>heap_arr;
};

}}
#endif
