
#include <iostream>
#include <thread>
#include <atomic>
#include <array>
#include <vector>
#include <stack>
class PetersonMutex
{
public:
	PetersonMutex()
	{
		want_[0].store(false);
		want_[1].store(false);
		victim_.store(0);
	}

	PetersonMutex(PetersonMutex &&other) {

		want_[0].store(other.want_[0].load());
		want_[1].store(other.want_[1].load());
		victim_.store(other.victim_.load());
	}

	void lock(int t)
	{
		want_[t].store(true);
		victim_.store(t);
		while (want_[1 - t].load() && victim_.load() == t) {
			// wait
		}
	}

	void unlock(int t) {
		want_[t].store(false);
	}

private:
	std::array<std::atomic<bool>, 2> want_;
	std::atomic<int> victim_;
};

class tree_mutex {
public:
	tree_mutex(size_t msize)
	{
		my_size_ = tree_size(msize);
		tree_.resize(my_size_);
	}

	void lock(int t){
		t = t + my_size_;
		while (t != 0) {
			tree_[ parent(t) ].lock( which_son(t) );
			t = parent(t);
		}
	}

	void unlock(int t) {
        std::stack<int> my_st;

		t = t + my_size_;
        while (t != 0) {
            my_st.push(t);//memorize our way
			t =  parent(t);
		}
		while (!my_st.empty()) {
		    t = my_st.top();
            my_st.pop();
			tree_[parent(t)].unlock( which_son(t) );
		}
	}

private:
	std::vector <PetersonMutex> tree_;
	std::atomic <size_t> my_size_;

	size_t tree_size(size_t t) {
		size_t e = 1;
		while (2 * e < t) {
			e = e * 2;
		}
		e = e * 2 - 1;
		return e;
	}

	int parent(int t){
	    return ( t - 1 ) / 2;
	}
	int which_son(int t){
	    return ( t - 1 ) % 2;
	}
};


void increment(int* x, tree_mutex* m, int thread)
{
	for (size_t i = 0; i < 100000; ++i) {
		m->lock(thread);
		++*x;
		m->unlock(thread);
	}
}
int main()
{

    size_t sz = 16;
    tree_mutex mut(sz);
    int x = 0;
    size_t i;

	std::vector < std::thread > my_th;
	my_th.clear();

    //this part is not right! but why?
	for (i = 0; i < sz; i++) {
        auto xptr = &x;
        auto mutptr = &mut;
		my_th.emplace_back(increment, xptr, mutptr, i);
	}
	//
	for (i = 0; i < sz; i++) {
		my_th[i].join();
	}
    /*std::thread t0(increment, &x, &mut, 0);
    std::thread t1(increment, &x, &mut, 1);
    std::thread t2(increment, &x, &mut, 2);
    std::thread t3(increment, &x, &mut, 3);
    std::thread t4(increment, &x, &mut, 4);
    std::thread t5(increment, &x, &mut, 5);
    t0.join();
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();*/
    std::cout << x << std::endl;
	return 0;
}
