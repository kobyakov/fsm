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

bool matchFor0(const std::string_view& input, size_t& read)
{
    if (input.at(0) == '0')
    {
        read = 1;
        return true;
    }
    return false;
}

bool matchFor1(const std::string_view& input, size_t& read)
{
    if (input.at(0) == '1')
    {
        read = 1;
        return true;
    }
    return false;
}

int main()
{

    State stateQ0("q0", FSMBool::FSM_FALSE);
    State stateQ1("q1", FSMBool::FSM_TRUE);
    State stateQ2("q2", FSMBool::FSM_TRUE);
    State stateD("D",  FSMBool::FSM_FALSE);

    stateQ0.insertNewEntry(&stateQ1, {matchFor1}, produceOutput);
    stateQ0.insertNewEntry(&stateQ2, {matchFor0}, produceOutput);

    stateQ1.insertNewEntry(&stateD,  {matchFor1}, produceOutput);
    stateQ1.insertNewEntry(&stateQ2, {matchFor0}, produceOutput);

    stateQ2.insertNewEntry(&stateQ1, {matchFor1}, produceOutput);
    stateQ2.insertNewEntry(&stateD,  {matchFor0}, produceOutput);

    stateD.insertNewEntry(&stateD, {matchFor0}, produceOutput);
    stateD.insertNewEntry(&stateD, {matchFor1}, produceOutput);

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