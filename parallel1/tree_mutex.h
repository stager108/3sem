
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
			std::this_thread::yield();
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
