/*
 * An XOR linked list is a more memory efficient doubly linked list. 
 * Instead of each node holding next and prev fields, it holds a field named both, 
 * which is an XOR of the next node and the previous node. Implement an XOR linked list; 
 * it has an add(element) which adds the element to the end, and a get(index) 
 * which returns the node at index.
*/

#include <iostream>

struct Node
{
    struct Node* both;
    int value;
};

class XORLinkedList
{
    public:
        Node* get(int index)
        {
            if(!head)
            {
                std::cout << "empty list!" << std::endl;
                // empty list!
                return nullptr;
            }

            Node* prev = nullptr;
            Node* curr = head;
            Node* next;

            for(int i = 0; i < index; ++i)
            {
                // the next node will be the previous XOR'd with the
                // current node's both value.
                next = XOR(prev, curr->both);

                // update for next iteration
                prev = curr;
                curr = next;
            }

            return curr;
        }

        void add(int data)
        {
            struct Node* node = (struct Node*) malloc(sizeof(struct Node));
            node->value = data;

            if(head)
            {
                // we have at least 1 node
                node->both = XOR(tail, nullptr);
                // 'node' is now the tail. Update the old tail
                tail->both = XOR(tail->both, node);
                // now point tail to the new tail
                tail = node;
            }
            else
            {
                // first node
                head = node;
                tail = head;
                // head and tail are the same and have no previous or next nodes
                head->both = nullptr;
            }
        }

    private:
        struct Node* XOR(struct Node* a, struct Node* b)
        {
            return (struct Node*) ((uintptr_t)a ^ (uintptr_t)b);
        }

        Node* head = nullptr;
        Node* tail = nullptr;
};

int main()
{
    XORLinkedList list;

    list.add(5);
    list.add(10);
    list.add(15);

    std::cout << "index 0: " << list.get(0)->value << std::endl;
    std::cout << "index 1: " << list.get(1)->value << std::endl;
    std::cout << "index 2: " << list.get(2)->value << std::endl;

    return 0;
}