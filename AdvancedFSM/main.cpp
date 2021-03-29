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

void produceOutput(const State* cur, const State* next, outputBuffer* buf)
{
    buf->push_back("Transition: from " + cur->name + " to " + next->name);
}

int main()
{

    State stateQ0("q0", FSMBool::FSM_FALSE);
    State stateQ1("q1", FSMBool::FSM_TRUE);
    State stateQ2("q2", FSMBool::FSM_TRUE);
    State stateD("D",  FSMBool::FSM_FALSE);

    stateQ0.insertNewEntry("1", &stateQ1, produceOutput);
    stateQ0.insertNewEntry("0", &stateQ2, produceOutput);

    stateQ1.insertNewEntry("1", &stateD, produceOutput);
    stateQ1.insertNewEntry("0", &stateQ2, produceOutput);

    stateQ2.insertNewEntry("1", &stateQ1, produceOutput);
    stateQ2.insertNewEntry("0", &stateD, produceOutput);

    stateD.insertNewEntry("0", &stateD, produceOutput);
    stateD.insertNewEntry("1", &stateD, produceOutput);

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