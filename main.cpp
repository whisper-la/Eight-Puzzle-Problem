#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <algorithm>
using namespace std;
vector<int> target = { 1, 2, 3, 4, 5, 6, 7, 8, 0 };//将结果定义为全局变量
//定义空格与四个方向的交换函数
vector<int> move_up(vector<int>current, int position, bool& flag)
{
    if (position < 3)					//第一行数字不能与上面交换
    {
        flag = false;
        return current;
    }
    else {								//非第一行，可向上交换
        vector<int> next_state = current;
        std::swap(next_state[position],next_state[position-3]);
        flag = true;
        return next_state;
    }
}

vector<int> move_down(vector<int> current, int position, bool& flag)
{
    if (position > 5)					//第三行数字不能与下面交换
    {
        flag = false;
        return current;
    }
    else {								//非第三行，可向下交换
        vector<int> next_state = current;
        std::swap(next_state[position],next_state[position+3]);
        flag = true;
        return next_state;
    }
}

vector<int> move_left(vector<int> current, int position, bool& flag)
{
    if (position % 3 == 0)				//第一列数字不能与左面交换
    {
        flag = false;
        return current;
    }
    else {								//非第一列，可向左交换
        vector<int> next_state = current;
        std::swap(next_state[position],next_state[position-1]);
        flag = true;
        return next_state;
    }
}

vector<int> move_right(vector<int> current, int position, bool& flag)
{
    if (position % 3 == 2)				//第三列数字不再与右面交换
    {
        flag = false;
        return current;
    }
    else {								//非第三列，可向右交换
        vector<int> next_state = current;
        std::swap(next_state[position],next_state[position+1]);
        flag = true;
        return next_state;
    }
}
//将九宫格写为一维的格式，求出除0之外所有数字的逆序数之和，也就是每个数字前面比它大的数字的个数的和，称为这个状态的逆序。如果与目标状态的逆序奇偶性相同，则有解，否则无解。目标状态是奇数逆序。
bool Reverse(vector<int> Init_state)//判断逆序对个数，若为奇数有解，偶数无解
{
    int i, j, count = 0;
    for (i = 0; i < 8; i++)
    {
        for (j = i + 1; j < 9; j++)
        {
            if (Init_state[i] > Init_state[j] && Init_state[j] != 0)
            {
                count++;
            }
        }
    }
    if ((count % 2) == 0)
    {
        return false;
    }
    else
        return true;
}
// the puzzle class
class AI_Puzzle
{
public:
    AI_Puzzle() { ; };
    AI_Puzzle(const vector<int>& init_state) {
        m_init_state.assign(init_state.begin(), init_state.end());
    };
    ~AI_Puzzle() { ; };

public:
    // set the init state
    void setInitState(const vector<int>& init_state) {
        m_init_state.assign(init_state.begin(), init_state.end());
    };

    //
    // please fill three types of solver to find the solution
    //
    // return true if the problem has solution
    // return false if the problem does not have solution
    // results: the output sequences of states
    //
    // We want the final state is
    //
    //		1	2	3
    //		4	5	6
    //		7	8	0
    //
    //计算UCS的代价cost
    int calc_Cost(vector<int> state)
    {
        int cost = 0;
        for (int i = 0; i < 9; i++)
        {
            if (state[i] != m_init_state[i])//将初始与当前状态的不同的元素个数当作代价
                cost++;
        }
        return cost;
    }

    bool solution(vector<vector<int>>& results)//用于计算并求解
    {
        // TODO: please fill in the code
        //
        // Yangang Wang, 2022/9/13, @SEU
        if (Reverse(m_init_state))//判断是否有解
        {
            return false;
        }
        int selection=0;//输入算法选择
        cout << "Please enter the serial number of the algorithm you want to select" << endl;
        cout << "1----BFS" << endl;
        cout << "2----DFS" << endl;
        cout << "3----UCS" << endl;
        cin>>selection;
        switch (selection) {
            case 1:
            {
                cout<<"Using BFS to solve the problem......"<<endl;
                return AI_Puzzle::BFS(results);
            }
            case 2:
            {
                cout<<"Using DFS to solve the problem......"<<endl;
                return AI_Puzzle::DFS(results);
            }
            case  3:
            {
                cout<<"Using DFS to solve the problem......"<<endl;
                return AI_Puzzle::UCS(results);
            }
            default:
            {
                cout << "Wrong number entered"<< endl;
                break;
            }
        }
        return true;
    }

private:
    // bfs to find the solution
    bool BFS(vector<vector<int>>& results)
    {
        // TODO: please fill in the code
        //
        // Yangang Wang, 2022/9/13, @SEU
        //表示可以交换的方向，分别是上下左右，其中-1表示无法交换，不是-1的数字表示可以被换到的位置，位置表示为
        //0 1 2
        //3 4 5
        //6 7 8
        int swap_m[9][4] =
                {
                        {-1, 3,  -1, 1 },
                        {-1, 4,  0,  2 },
                        {-1, 5,  1,  -1},
                        {0,  6,  -1, 4 },
                        {1,  7,  3,  5 },
                        {2,  8,  4,  -1},
                        {3,  -1, -1, 7 },
                        {4,  -1, 6,  8 },
                        {5,  -1, 7,  -1}
                };
        queue<vector<int>> Queue;
        stack<vector<int>> father;
        map<vector<int>, bool> my_map;
        map<vector<int>, vector<int>> parent;
        int zero_position = 0;
        Queue.push(m_init_state);
        my_map[m_init_state] = true;
        vector<int> Now;
        while (!Queue.empty())
        {
            Now = Queue.front();
            Queue.pop();
            zero_position = 0;
            while (Now[zero_position] != 0)
                zero_position++;
            if (Now == target)
            {

                while (Now!=m_init_state)
                {
                    father.push(parent[Now]);
                    Now = parent[Now];
                }
                while (!father.empty())
                {
                    results.push_back(father.top());
                    father.pop();
                }
                results.push_back(target);
                return true;
            }
            else
            {
                for (int i = 0; i < 4; i++)
                {
                    int destination = swap_m[zero_position][i];
                    if (destination != -1)
                    {
                        vector<int> temp;
                        vector<int> now_memory = Now;
                        swap(Now[zero_position], Now[destination]);
                        temp = Now;
                        if (my_map.count(temp) == 0)
                        {
                            Queue.push(temp);
                            my_map[temp] = true;
                            parent[temp] = now_memory;
                        }
                        swap(Now[zero_position], Now[destination]);
                    }
                }
            }

        }
        return false;
    }

    // dfs to find the solution
    bool DFS(vector<vector<int>>& results)
    {
        // TODO: please fill in the code
        //
        // Yangang Wang, 2022/9/13, @SEU
        stack<vector<int>> Stack;
        set<vector<int>> close;
        map<vector<int>, vector<int>> path;

        Stack.push(m_init_state);
        int zero_position = 0;
        bool judge = false;
        while (!Stack.empty())
        {
            vector<int> Now = Stack.top();
            Stack.pop();
            close.insert(Now);
            if (Now == target)
            {
                results.push_back(target);
                map<vector<int>, vector<int>> ::iterator iter = path.find(target);
                vector<int> New;
                while (iter != path.end())
                {
                    New = iter->second;
                    results.push_back(New);
                    iter = path.find(New);
                }
                reverse(results.begin(), results.end());
                return true;
            }
            while (Now[zero_position] != 0)
                zero_position++;
            vector<int> next;
            next = move_up(Now, zero_position, judge);
            if (judge)
            {
                if (close.count(next) == 0)
                {
                    Stack.push(next);
                    path.insert(pair<vector<int>, vector<int>>(next, Now));
                }
            }
            next = move_right(Now, zero_position, judge);
            if (judge)
            {
                if (close.count(next) == 0)
                {
                    Stack.push(next);
                    path.insert(pair<vector<int>, vector<int>>(next, Now));
                }
            }
            next = move_down(Now, zero_position, judge);
            if (judge)
            {
                if (close.count(next) == 0)
                {
                    Stack.push(next);
                    path.insert(pair<vector<int>, vector<int>>(next, Now));
                }
            }
            next = move_left(Now, zero_position, judge);
            if (judge)
            {
                if (close.count(next) == 0)
                {
                    Stack.push(next);
                    path.insert(pair<vector<int>, vector<int>>(next, Now));
                }
            }
            zero_position=0;
        }
        return true;
    }

    // ucs to find the solution
    bool UCS(vector<vector<int>>& results)
    {
        // TODO: please fill in the code
        //
        // Yangang Wang, 2022/9/13, @SEU
        priority_queue<pair<pair<int, vector<int>>, int>> Queue;//存储过程并通过优先级判别行走路线
        set<vector<int>> close;
        map<vector<int>, vector<int>> path;

        Queue.push(make_pair(make_pair(0, m_init_state), 0));
        int zero_position = 0;
        bool judge = false;
        while (!Queue.empty())
        {
            vector<int> Now = Queue.top().first.second;
            int depth = Queue.top().second;
            Queue.pop();
            close.insert(Now);
            if (Now == target)
            {
                results.push_back(target);
                map<vector<int>, vector<int>>::iterator iter = path.find(target);
                vector<int> New;
                while (iter != path.end())
                {
                    New = iter->second;
                    results.push_back(New);
                    iter = path.find(New);
                }
                reverse(results.begin(), results.end());
                return true;
            }
            while (Now[zero_position] != 0)
                zero_position++;
            vector<int> next;
            next = move_up(Now, zero_position, judge);
            if (judge)
            {
                if (close.count(next) == 0)
                {
                    Queue.push(make_pair(make_pair(-calc_Cost(next) - depth, next), depth + 1));
                    path.insert(pair<vector<int>, vector<int>>(next, Now));
                }
            }
            next = move_down(Now, zero_position, judge);
            if (judge)
            {
                if (close.count(next) == 0)
                {
                    Queue.push(make_pair(make_pair(-calc_Cost(next) - depth, next), depth + 1));
                    path.insert(pair<vector<int>, vector<int>>(next, Now));
                }
            }
            next = move_left(Now, zero_position, judge);
            if (judge)
            {
                if (close.count(next) == 0)
                {
                    Queue.push(make_pair(make_pair(-calc_Cost(next) - depth, next), depth + 1));
                    path.insert(pair<vector<int>, vector<int>>(next, Now));
                }
            }
            next = move_right(Now, zero_position, judge);
            if (judge)
            {
                if (close.count(next) == 0)
                {
                    Queue.push(make_pair(make_pair(-calc_Cost(next) - depth, next), depth + 1));
                    path.insert(pair<vector<int>, vector<int>>(next, Now));
                }
            }
            zero_position=0;
        }
        return true;
    }

private:
    vector<int> m_init_state;
};

// test function for 8-puzzle problem
void test(const vector<int>& init_state) {
    AI_Puzzle puzzle;
    puzzle.setInitState(init_state);
    vector<vector<int>> results;
    if (puzzle.solution(results)) {
        printf("solution for the initial state: ");
        for (int i = 0; i < init_state.size(); i++)
            printf("%d ", init_state[i]);
        printf("\n");
        for (int i = 0; i < results.size(); i++) {
            printf("step: %03d:", i);
            for (int j = 0; j < results[i].size(); j++) {
                printf(" %d", results[i][j]);
            }
            printf("\n");
        }
    }
    else {
        printf("no solution for the initial state: ");
        for (int i = 0; i < init_state.size(); i++)
            printf("%d ", init_state[i]);
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char** argv)
{
    // This is the code for testing the 8-puzzle problem in
    //
    //  'The first project homework for Artificial Intelligence'
    //
    // Please fill in the code of class 'AI_Puzzle'
    // and write your design, running tests, analysis and etc. into your report
    // Be careful: in each step, we can ONLY exchange with ONE neighbor of digit 0, horizontally or vertically
    //
    // Good Luck!
    //
    // Yangang Wang, 2022/9/13, @SEU

    // Test 1
    //	5   1   2
    //	6   3   0
    //	4   7   8
    vector<int> init_state1 = { 5, 1, 2, 6, 3, 0, 4, 7, 8 };
    test(init_state1);

    // Test 2
    //
    //	2   8   3
    //	1   6   4
    //	7   5   0
    vector<int> init_state_2 = { 2, 8, 3, 1, 6, 4, 7, 5, 0 };
    test(init_state_2);

    // Test 3
    //
    //	1   0   2
    //	3   4   5
    //	6   7   8
    vector<int> init_state_3 = { 1, 0, 2, 3, 4, 5, 6, 7, 8 };
    test(init_state_3);

    // Test 4
    //
    //	4   3   5
    //	6   0   8
    //	2   1   7
    vector<int> init_state_4 = { 4, 3, 5, 6, 0, 8, 2, 1, 7 };
    test(init_state_4);

    // Test 5
    //
    //	1   7   3
    //	0   5   4
    //	6   2   8
    vector<int> init_state_5 = { 1, 7, 3, 0, 5, 4, 6, 2, 8 };
    test(init_state_5);

    return 0;
}