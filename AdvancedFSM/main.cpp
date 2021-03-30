#include <fsm.h>
#include <iostream>

using namespace SML;

#define CHECK_FSM_RESULT(res)                           \
do {                                                    \
    if ((res) == FSMError::FSM_NO_ERROR)                \
            sm.result == true ?                         \
                printf("input is accepted.\n") :       \
                printf("input is not accepted.\n");    \
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

void bitFlipperExample()
{
    State stateQ0("q0");
    State stateQ1("q1", true);
    State stateQ2("q2", true);
    State stateD("D",  false);

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
}

bool matchFor_09(const std::string_view& input, size_t& read)
{
    if (input.at(0) >= '0' && input.at(0) <= '9')
    {
        read = 1;
        return true;
    }
    return false;
}

bool matchFor_az(const std::string_view& input, size_t& read)
{
    if (input.at(0) >= 'a' && input.at(0) <= 'z')
    {
        read = 1;
        return true;
    }
    return false;
}

bool matchFor_AZ(const std::string_view& input, size_t& read)
{
    if (input.at(0) >= 'A' && input.at(0) <= 'Z')
    {
        read = 1;
        return true;
    }
    return false;
}

bool matchFor_at(const std::string_view& input, size_t& read)
{
    if (input.at(0) == '@')
    {
        read = 1;
        return true;
    }
    return false;
}

bool matchForAddress(const std::string_view& input, size_t& read)
{
    std::string address0 = "gmail.com";
    std::string address1 = "icloud.com";

    if (input.compare(0, address0.length(), address0) == 0)
    {
        read = address0.length();
        return true;
    }
    if (input.compare(0, address1.length(), address1) == 0)
    {
        read = address1.length();
        return true;
    }
    return false;
}

bool matchAny(const std::string_view& input, size_t& read)
{
    read = 1;
    return true;
}

void emailValidatorExample()
{
    State q0("INITIAL");
    State q1("FIRST SYMBOL");
    State q2("SECOND SYMBOL");
    State q3("THIRD SYMBOL");
    State q4("FOURTH SYMBOL");
    State q5("FIFTH AN MORE SYMBOL");
    State q6("@ SYMBOL");
    State Fin("FINAL", true);
    State D("D");

    q0.insertNewEntry(&q1, {matchFor_09, matchFor_AZ, matchFor_az}, produceOutput);
    q0.insertNewEntry(&D, {matchAny}, produceOutput);

    q1.insertNewEntry(&q2, {matchFor_09, matchFor_AZ, matchFor_az}, produceOutput);
    q1.insertNewEntry(&D, {matchAny}, produceOutput);

    q2.insertNewEntry(&q3, {matchFor_09, matchFor_AZ, matchFor_az}, produceOutput);
    q2.insertNewEntry(&D, {matchAny}, produceOutput);

    q3.insertNewEntry(&q4, {matchFor_09, matchFor_AZ, matchFor_az}, produceOutput);
    q3.insertNewEntry(&D, {matchAny}, produceOutput);

    q4.insertNewEntry(&q5, {matchFor_09, matchFor_AZ, matchFor_az}, produceOutput);
    q4.insertNewEntry(&D, {matchAny}, produceOutput);

    q5.insertNewEntry(&q5, {matchFor_09, matchFor_AZ, matchFor_az}, produceOutput);
    q5.insertNewEntry(&q6, {matchFor_at}, produceOutput);
    q5.insertNewEntry(&D, {matchAny}, produceOutput);

    q6.insertNewEntry(&Fin, {matchForAddress}, produceOutput);
    q6.insertNewEntry(&D, {matchAny}, produceOutput);

    Fin.insertNewEntry(&D, {matchAny}, produceOutput);
    D.insertNewEntry(&D, {matchAny}, produceOutput);

    FSM sm("emailValidator", &q0);

    {
        std::string test("testemail@gmail.com");
        printf("Check string %s: ", test.c_str());
        CHECK_FSM_RESULT(sm.execute(test));
    }
    {
        std::string test("1@gmail.com");
        printf("Check string %s: ", test.c_str());
        CHECK_FSM_RESULT(sm.execute(test));
    }
    {
        std::string test("123456@icloud.com");
        printf("Check string %s: ", test.c_str());
        CHECK_FSM_RESULT(sm.execute(test));
    }
    {
        std::string test("123456@icloud.comggf");
        printf("Check string %s: ", test.c_str());
        CHECK_FSM_RESULT(sm.execute(test));
    }

    {
        std::string test("testemail@yahoo.com");
        printf("Check string %s: ", test.c_str());
        CHECK_FSM_RESULT(sm.execute(test));
    }


}


int main()
{
    //bitFlipperExample();
    emailValidatorExample();
    return 0;
}