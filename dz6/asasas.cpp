#pragma once

#include <iostream>
#include <stack>
#include "SuffixArray.h"

class M
{
public:
    void solve() {
        int n, m;
        std::cin >> n >>m;
        std::string s;
        std::vector<int> input(n);
        for(int i = 0; i<n; ++i) {
            std::cin >>input[i];
            s.push_back((char)input[i]);
        }
        SuffixArray sa;
        std::vector<int> suffArray = sa.buildFromLecture(s);
        std::vector<int> lcp = sa.getLCP(s, suffArray);
        std::stack<StElem> stack;
        std::vector<int> numOfOccurences(suffArray.size(), 0);
        int maxH = 0;
        int maxW = 0;
        int start = 0;

        lcp[n-1] = 0;
        for (int index = 0; index < n; index++) {
            int newWidth = 0;
            while(!stack.empty() && stack.top() >= lcp[index]) {
                StElem e = stack.top();
                stack.pop();
                newWidth += e.w;
                if ((newWidth+1) * e.h > (maxW+1) * maxH) {
                    maxW = newWidth;
                    maxH = e.h;
                    start = suffArray[e.index];
                }
            }
            if(newWidth != 0){
                stack.push(StElem(newWidth, index , lcp[index]));
            }
            else{

                stack.push(StElem(1, index , lcp[index]));
            }
        }


        maxW;
        std::cout << maxH*(maxW+1) <<'\n' << maxW+1 <<'\n';
        // Ответ это подмассив данного массива [start, start + maxH)
        for (int i = start; i<= start+maxW; ++i) {
            std::cout << input[i] <<" ";
        }
    }

private:
    struct StElem
    {
        StElem(int w, int id, int h) :w(w), index(id), h(h) {}
        int w, index, h;
    };

};


