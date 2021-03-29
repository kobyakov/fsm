#include <fsm.h>
#include <iostream>

using namespace SML;

#define CHECK_FSM_RESULT(res)                           \
do {                                                    \
    if ((res) == FSMError::FSM_NO_ERROR)                \
            sm.result == FSMBool::FSM_TRUE ?            \
                printf("string is accepted.\n") :       \
                printf("string is not accepted.\n");    \
        else                                            \
            printf("FSM execution error\n");            \
} while(0)


int main()
{

    State stateQ0(
        "q0", FSMBool::FSM_FALSE, 
        [](const State* cur, const State* next, outputBuffer* buf){
            buf->push_back("Transition: from " + cur->name + " to " + next->name);
        });
    State stateQ1(
        "q1", FSMBool::FSM_TRUE,          
        [](const State* cur, const State* next, outputBuffer* buf){
            buf->push_back("Transition: from " + cur->name + " to " + next->name);
        }
    );
    State stateQ2(
        "q2", FSMBool::FSM_TRUE,  
        [](const State* cur, const State* next, outputBuffer* buf){
            buf->push_back("Transition: from " + cur->name + " to " + next->name);
        }
    );
    State stateD(
        "D",  FSMBool::FSM_FALSE,
        [](const State* cur, const State* next, outputBuffer* buf){
            buf->push_back("Transition: from " + cur->name + " to " + next->name);
        }
    );

    stateQ0.insertNewEntry("1", &stateQ1);
    stateQ0.insertNewEntry("0", &stateQ2);

    stateQ1.insertNewEntry("1", &stateD);
    stateQ1.insertNewEntry("0", &stateQ2);

    stateQ2.insertNewEntry("1", &stateQ1);
    stateQ2.insertNewEntry("0", &stateD);

    stateD.insertNewEntry("0", &stateD);
    stateD.insertNewEntry("1", &stateD);

    FSM sm("BitFlipper", &stateQ0);

    {
        std::string test("01010101");
        printf("Check string %s: ", test.c_str());
        CHECK_FSM_RESULT(sm.execute(test));
        for (auto s : sm.output) std::cout << s << std::endl;
    }
    {
        std::string test("01010111");
        printf("Check string %s: ", test.c_str());
        CHECK_FSM_RESULT(sm.execute(test));
        for (auto s : sm.output) std::cout << s << std::endl;
    }   
    return 0;
}