#include <iostream>

#include "Queue.hpp"
#include <vector>
#include <queue>
using namespace std;

int simTime, regNum, maxLen;

vector<int> regWaitingTime;
string modeLine;

queue<int> times;

vector<queue<int>> lines;
vector<int> registers;

void input_data()
{
    freopen("examples/sample.in", "r", stdin);


    cin >> simTime >> regNum >> maxLen;
    cin >> modeLine;
    simTime *= 60;

    regWaitingTime.resize(regNum);
    for (int i = 0; i < regNum; i++)
        cin >> regWaitingTime[i];

    while (1)
    {
        int _mem, _time;
        cin >> _mem;
        if (cin.fail() || _mem <= 0)
        {
            break;
        }
        cin >> _time;

        while (_mem--)
            times.push(_time);
    }

    registers.resize(regNum, -1);
    lines.resize(regNum);
}

int secs = 0;

double total_time = 0.0;
int stat_entered_line = 0;
int stat_exited_line = 0;
int stat_exit_register = 0;
double avg_wait_time = 0;
int stat_left_in_line = 0;
int stat_left_in_reg = 0;
int stat_lost = 0;

void put_men_to_Register()
{
    int i;

    while (1)
    {

        bool flag = false;

        for (i = 0; i < regNum; i++)
        {
            if (registers[i] == 0)
            {
                printf("%d exited register %d\n", secs, i + 1); // cout<< secs << " exited register " << i+1 << endl;
                stat_exit_register++;
            }
        }
        for (i = 0; i < regNum; i++)
        {
            if (registers[i] <= 0 && lines[i].size() > 0) //////// exit line and enter _register.
            {
                registers[i] = regWaitingTime[i]; // put one person to register.
                total_time += (secs - lines[i].front());
                printf("%d exited line %d length %lu wait time %d\n", secs, i + 1, lines[i].size() - 1, secs - lines[i].front()); // cout << secs<<" exited  line " << i+1 << " wait time " << secs - lines[i].front() << endl;
                stat_exited_line++;
                lines[i].pop();                                  // from line
                printf("%d entered register %d\n", secs, i + 1); // cout << secs<<" entered register " << i+1 << endl;
                flag = true;
            }
        }

        while (1)
        {
            int min_val = maxLen, index = 0;
            for (i = 0; i < regNum; i++)
            {
                if (min_val > lines[i].size())
                {
                    min_val = lines[i].size();
                    index = i;
                }
            }

            if (lines[index].size() < maxLen && secs == times.front()) //// enter _ lines.
            {
                lines[index].push(secs);
                times.pop();
                printf("%d entered line %d length %lu\n", secs, index + 1, lines[index].size()); // cout << secs<<" entered line " << i+1 << " length " << lines[i].size() << endl;

                stat_entered_line++;

                flag = true;
            }
            else
            {
                break;
            }
        }

        if (times.size() > 0)
        {
            while (secs > times.front()) // lost person
            {
                printf("%d lost\n", secs - 1);
                stat_lost++;
                times.pop();
                flag = true;
            }
        }

        if (!flag)
            break;
    }
}

void solve()
{
    cout << "LOG" << endl
         << "0 start" << endl;
    int i;

    for (secs = 0; secs <= simTime; secs++)
    {
        put_men_to_Register();
        for (i = 0; i < regNum; i++)
            registers[i]--;
    }
    cout << simTime << " end" << endl
         << endl;

    cout << "STAT" << endl
         << "Entered Line     : " << stat_entered_line << endl
         << "Exited Line      : " << stat_exited_line << endl
         << "Exited Register  : " << stat_exit_register << endl
         << "Avg Wait Time    : " << total_time / stat_exited_line << endl
         << "Left In Line     : " << stat_entered_line - stat_exited_line << endl
         << "Left In Register : " << stat_entered_line - stat_exit_register << endl
         << "Lost             : " << stat_lost << endl;
}

int main(int argc, char **argv)
{
    input_data();
    solve();

    return 0;
}
