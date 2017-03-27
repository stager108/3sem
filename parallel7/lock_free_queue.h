
#include <iostream>
#include <thread>
#include <atomic>
#include <array>
#include <vector>
#include <stack>
#include <memory>
#pragma once

#include <atomic>
#include <thread>

unsigned const MAX_HAZARD_POINTERS = 100;
unsigned const MAX_HAZARD_SIZE = 2;
struct HazardPointer
{
    std::atomic<std::thread::id> id;
    std::atomic<void*> pointer[MAX_HAZARD_SIZE];
};
HazardPointer g_hazardPointers[MAX_HAZARD_POINTERS];

class HPOwner
{
public:
    HPOwner(const HPOwner&) = delete;
    HPOwner operator=(const HPOwner&) = delete;

    HPOwner() : hp_(nullptr)
    {
        for (unsigned i = 0; i < MAX_HAZARD_POINTERS; ++i) {
            std::thread::id oldId;
            if (g_hazardPointers[i].id.compare_exchange_strong(
                    oldId, std::this_thread::get_id())) {
                hp_ = &g_hazardPointers[i];
                break;
            }
        }
        if (!hp_) {
            throw std::runtime_error("No hazard pointers available");
        }
    }

    std::atomic<void*>* getPointer()
    {
        return hp_->pointer;
    }

    ~HPOwner()
    {
        for(unsigned j = 0; j < MAX_HAZARD_SIZE; ++j){
            hp_->pointer[j].store(nullptr);
        }
        hp_->id.store(std::thread::id());
    }

private:
    HazardPointer* hp_;
};

std::atomic<void*>* getHazardPointerForCurrentThread()
{
    thread_local static HPOwner hazard;
    return hazard.getPointer();
}

bool outstandingHazardPointersFor(void* p)
{
    for (unsigned i = 0; i < MAX_HAZARD_POINTERS; ++i) {
        for(unsigned j = 0; j < MAX_HAZARD_SIZE; ++j){
            if (g_hazardPointers[i].pointer[j] == p) {
                return true;
            }
        }
    }
    return false;
}

template<typename T>
void doDelete(void* p)
{
    delete static_cast<T*>(p);
}

struct DataToReclaim
{
    void* data;
    std::function<void(void*)> deleter;
    DataToReclaim* next;

    template<typename T>
    DataToReclaim(T* p)
        : data(p)
        , deleter(&doDelete<T>)
        , next(nullptr)
    {}

    ~DataToReclaim()
    {
        deleter(data);
    }
};

std::atomic<DataToReclaim*> nodesToReclaim;

void addToReclaimList(DataToReclaim* node)
{
    node->next = nodesToReclaim.load();
    while (!nodesToReclaim.compare_exchange_weak(node->next, node));
}

template<typename T>
void reclaimLater(T* data)
{
    addToReclaimList(new DataToReclaim(data));
}

void deleteNodesWithNoHazards()
{
    DataToReclaim* current = nodesToReclaim.exchange(nullptr);
    while (current) {
        DataToReclaim* const next = current->next;
        if (!outstandingHazardPointersFor(current->data)) {
            delete current;
        } else {
            addToReclaimList(current);
        }
        current = next;
    }
}


template <typename T>
class lock_free_queue
{
private:
    struct Node {
        T* value;
        std::atomic<Node*> next;
        Node(T* v){
            value = v;
            next.store(nullptr);
        }

    };
    std::atomic<Node*> head;
    std::atomic<Node*> tail;

    void my_delete(Node* next_for_delete) {
        while (next_for_delete) {
            Node* next = next_for_delete->next;
            delete next_for_delete;
            next_for_delete = next;
        }
    }
public:
	lock_free_queue()
	{
        head.store(new Node(nullptr));
        tail.store(head.load());
	}

	~lock_free_queue() {
        my_delete(head.load());
	}

    void enqueue(T new_value){
        T* val = new T(new_value);
        Node* new_node = new Node(val);
        Node* curr_tail;
        auto hp = getHazardPointerForCurrentThread();
        while (true) {
            curr_tail = tail.load();
            hp[0].store(curr_tail);
            if (curr_tail != tail.load()) {
                continue;
            }
            Node* curr_tail_next = tail.load()->next;
            if (!curr_tail_next) {
                if ((tail.load()->next).compare_exchange_strong(curr_tail_next, new_node)) { // 1
                    break;
                }
            } else {
                tail.compare_exchange_strong(curr_tail, curr_tail_next); // helping
            }
        }
        tail.compare_exchange_strong(curr_tail, new_node);
        hp[0].store(nullptr);
    }


    bool dequeue(T& new_value){
        auto hp = getHazardPointerForCurrentThread();
        while (true) {
            Node* curr_head = head.load();
            hp[0].store(curr_head);
            if (curr_head != head.load()) {
                continue;
            }
            Node* curr_tail = tail.load();

            Node* curr_head_next = curr_head->next.load();
            hp[1].store(curr_head_next);
            if (curr_head_next != curr_head->next.load()) {
                continue;
            }

            if (curr_head == curr_tail) {
                if (!curr_head_next){
                    hp[0].store(nullptr);
                    return false;
                }
                else{
                    tail.compare_exchange_strong(curr_head, curr_head_next);
                }
            }
            else {
                if (head.compare_exchange_strong(curr_head, curr_head_next)) {
                    new_value = *(curr_head_next->value);
                    delete  curr_head_next->value;
                    curr_head_next->value = nullptr;
                    hp[0] = nullptr;
                    hp[1] = nullptr;
                    if (outstandingHazardPointersFor(curr_head)) {
                        reclaimLater(curr_head);
                    } else {
                        delete curr_head;
                    }
                    deleteNodesWithNoHazards();
                    return true;
                }
            }
        }
    }

};
