#ifndef SKIP_LIST_H
#define SKIP_LIST_H
#include <iostream> 
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <mutex>
#include <fstream>
std::mutex mtx;//互斥锁
template<typename K,typename V>
class Node{
    private:
        K key;
        V value;
        int node_level;
    public:
        Node<K,V> **next;
        Node(){};
        Node(K,V,int);
        ~Node();
        K getKey(){return this->key;};
        V getValue(){return this->value;};
        void setValue(V value){this->value = value;};
};

template<typename K,typename V>
Node<K,V>::Node(const K key,const V value,int level){
    this->key = key;
    this->value = value;
    this->node_level = level;
    this->next = new Node<K,V>*[level+1];
    memset(this->next,0,sizeof(Node<K,V>*)*(level+1));
}
template<typename K,typename V>
Node<K,V>::~Node(){
    delete[] next;
}

template<typename K,typename V>
class SkipList{
    private:
        int max_level;
        int current_level;
        Node<K,V>* head;
    public:
        int element_count;
        SkipList(int);
        ~SkipList();
        int random_level();
        Node<K,V>* createNode(K,V,int);
        void insertElement(K,V);
        void deleteElement(K);
        void searchElement(K);
        void displayList();
};

template<typename K,typename V>
SkipList<K,V>::SkipList(int max_level){
    this->max_level = max_level;
    this->current_level = 0;
    this->element_count = 0;
    K k;
    V v;
    this->head = new Node<K,V>(k,v,max_level);
}
template<typename K,typename V>
SkipList<K,V>::~SkipList(){
    delete head;
}
template<typename K,typename V>
int SkipList<K,V>::random_level(){
    int level = 0;
    while(rand()%2){
        level++;
    }
    return level;
}
template<typename K,typename V>
Node<K,V>* SkipList<K,V>::createNode(K key,V value,int level){
    Node<K,V> *n = new Node<K,V>(key,value,level);
    return n;
}

template<typename K,typename V>
void SkipList<K,V>::insertElement(K key,V value){
    //互斥锁加锁
    mtx.lock();
    Node<K,V> *current = this->head;
    Node<K,V> *update[max_level+1];
    memset(update,0,sizeof(Node<K,V>*)*(max_level+1));
    for(int i=current_level;i>=0;i--){
        while(current->next[i]!=NULL && current->next[i]->getKey()<key){
            current = current->next[i];
        }
        update[i] = current;
    }
    current = current->next[0];
    if(current!=NULL&&current->getKey()==key){
        if(current->getValue()!=value){
            current->setValue(value);//覆盖旧值
        }
        std::cout<<"key: "<<key<<" already exists"<<std::endl;
        mtx.unlock();
        return;
    }
    //需要插入key
    if(current==NULL||current->getKey()!=key){
        int randomlevel = random_level();
        if(randomlevel>current_level){
        //把超出current_level的update进行更新
            for(int i=current_level+1;i<=randomlevel;i++){
                update[i]=head;
            }
            current_level = randomlevel;
        }
        Node<K,V>* insertNode = createNode(key,value,randomlevel);
        //更新该节点的next数组
        for (int i = 0; i <= randomlevel; i++)
        {
            insertNode->next[i] = update[i]->next[i];
            update[i]->next[i] = insertNode;
        }
        std::cout << "Successfully inserted key:" << key << ", value:" << value << std::endl;
        element_count ++;
    }
    mtx.unlock();
    return;
}


template<typename K,typename V>
void SkipList<K,V>::displayList(){
    std::cout<<"---------display skiplist-----------"<<std::endl;
    for (int i = 0; i <= current_level; i++)
    {
        std::cout<<"Level"<<i<<":";
        Node<K,V>* node = this->head->next[i];
        while(node!=NULL){
            std::cout<<node->getKey()<<":"<<node->getValue()<<" ";
            node = node->next[i];
        }
        std::cout<<std::endl;
    }
}
#endif