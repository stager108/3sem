
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

struct HazardPointer
{
    std::atomic<std::thread::id> id;
    std::atomic<void*> pointer;
};
HazardPointer g_hazardPointers[MAX_HAZARD_POINTERS];

class HPOwner
{
public:
    HPOwner(const HPOwner&) = delete;
    HPOwner operator=(const HPOwner&) = delete;

    HPOwner()
        : hp_(nullptr)
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

    std::atomic<void*>& getPointer()
    {
        return hp_->pointer;
    }

    ~HPOwner()
    {
        hp_->pointer.store(nullptr);
        hp_->id.store(std::thread::id());
    }

private:
    HazardPointer* hp_;
};

std::atomic<void*>& getHazardPointerForCurrentThread()
{
    thread_local static HPOwner hazard;
    return hazard.getPointer();
}

bool outstandingHazardPointersFor(void* p)
{
    for (unsigned i = 0; i < MAX_HAZARD_POINTERS; ++i) {
        if (g_hazardPointers[i].pointer.load() == p) {
            return true;
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
    struct Node {
        T* value;
        std::atomic<Node*> next;
        Node(T* v){
            value = v;
            next.store(nullptr);
        }
    };

public:
	lock_free_queue()
	{
        head.store(new Node(nullptr));
        tail.store(head.load());
	}

    void enqueue(T new_value){
        T* val = new T(new_value);
        Node* new_node = new Node(val);
        Node* curr_tail;
        while (true) {
            curr_tail = tail.load();
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
    }


    bool dequeue(T& new_value){
        while (true) {
            Node* curr_head = head.load();
            Node* curr_tail = tail.load();
            Node* curr_head_next = curr_head->next.load();
            if (curr_head == curr_tail) {
                if (!curr_head_next){
                    return false;
                }
                else{
                    tail.compare_exchange_strong(curr_head, curr_head_next);
                }
            }
            else {
                if (head.compare_exchange_strong(curr_head, curr_head_next)) {
                    new_value = *(curr_head_next->value);
                    return true;
                }
            }
        }
    }
private:

    std::atomic<Node*> head;
    std::atomic<Node*> tail;
};
