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

void produceOutput(const State* cur, const State* next, const inputBuffer& input, outputBuffer* buf)
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

void converterOutput(const State* cur, const State* next, const inputBuffer& input, outputBuffer* buf)
{
    if (input == "0000")
    {
        buf->push_back("0");
        return;
    }
    if (input == "0001")
    {
        buf->push_back("1");
        return;
    }
    if (input == "0010")
    {
        buf->push_back("2");
        return;
    }
    if (input == "0011")
    {
        buf->push_back("3");
        return;
    }
    if (input == "0100")
    {
        buf->push_back("4");
        return;
    }
    if (input == "0101")
    {
        buf->push_back("5");
        return;
    }
    if (input == "0110")
    {
        buf->push_back("6");
        return;
    }
    if (input == "0111")
    {
        buf->push_back("7");
        return;
    }
    if (input == "1000")
    {
        buf->push_back("8");
        return;
    }
    if (input == "1001")
    {
        buf->push_back("9");
        return;
    }
    if (input == "1010")
    {
        buf->push_back("A");
        return;
    }
    if (input == "1011")
    {
        buf->push_back("B");
        return;
    }
    if (input == "1100")
    {
        buf->push_back("C");
        return;
    }
    if (input == "1101")
    {
        buf->push_back("D");
        return;
    }
    if (input == "1110")
    {
        buf->push_back("E");
        return;
    }
        if (input == "1111")
    {
        buf->push_back("F");
        return;
    }
}

bool matchFor4(const std::string_view& input, size_t& read)
{
    if (input.length() < 4)
        return false;
    if(input.at(0) != '0' && input.at(0) != '1')
        return false;
    if(input.at(1) != '0' && input.at(1) != '1')
        return false;
    if(input.at(2) != '0' && input.at(2) != '1')
        return false;
    if(input.at(3)!= '0' && input.at(3) != '1')
        return false;
    read = 4;
    return true;
}

void converter()
{
    State q0("0-3");
    State q1("3-7");
    State q2("8-11");
    State q3("12-15");
    State q4("16-19");
    State q5("20-23");
    State q6("24-27");
    State q7("28-31");
    State F("fin", true);
    State D("D");

    q0.insertNewEntry(&q1, {matchFor4}, converterOutput);
    q0.insertNewEntry(&D, {matchAny}, nullptr);

    q1.insertNewEntry(&q2, {matchFor4}, converterOutput);
    q1.insertNewEntry(&D, {matchAny}, nullptr);

    q2.insertNewEntry(&q3, {matchFor4}, converterOutput);
    q2.insertNewEntry(&D, {matchAny}, nullptr);

    q3.insertNewEntry(&q4, {matchFor4}, converterOutput);
    q3.insertNewEntry(&D, {matchAny}, nullptr);

    q4.insertNewEntry(&q5, {matchFor4}, converterOutput);
    q4.insertNewEntry(&D, {matchAny}, nullptr);

    q5.insertNewEntry(&q6, {matchFor4}, converterOutput);
    q5.insertNewEntry(&D, {matchAny}, nullptr);

    q6.insertNewEntry(&q7, {matchFor4}, converterOutput);
    q6.insertNewEntry(&D, {matchAny}, nullptr);

    q7.insertNewEntry(&F, {matchFor4}, converterOutput);
    q7.insertNewEntry(&D, {matchAny}, nullptr);

    F.insertNewEntry(&D, {matchAny}, nullptr);
    D.insertNewEntry(&D, {matchAny}, nullptr);

    FSM sm("converter", &q0);

    {
        std::string test("00000001001000110100010101100110");
        printf("Check string %s: ", test.c_str());
        CHECK_FSM_RESULT(sm.execute(test));
        for (auto s : sm.output) std::cout << s;
        std::cout << std::endl;
    }
}

void sixDigitsPasswordCheck()
{
    State q0("0");
    State q1("1");
    State q2("2");
    State q3("3");
    State q4("4");
    State F("fin", true);
    State D("D");

    q0.insertNewEntry(&q1, {matchFor_09}, produceOutput);
    q0.insertNewEntry(&D, {matchAny}, produceOutput);

    q1.insertNewEntry(&q2, {matchFor_09}, produceOutput);
    q1.insertNewEntry(&D, {matchAny}, produceOutput);

    q2.insertNewEntry(&q3, {matchFor_09}, produceOutput);
    q2.insertNewEntry(&D, {matchAny}, produceOutput);

    q3.insertNewEntry(&q4, {matchFor_09}, produceOutput);
    q3.insertNewEntry(&D, {matchAny}, produceOutput);

    q4.insertNewEntry(&F, {matchFor_09}, produceOutput);
    q4.insertNewEntry(&D, {matchAny}, produceOutput);

    F.insertNewEntry(&D, {matchAny}, produceOutput);
    D.insertNewEntry(&D, {matchAny}, produceOutput);

    FSM sm("6digits", &q0);

    {
        std::string test("050234");
        printf("Check string %s: ", test.c_str());
        CHECK_FSM_RESULT(sm.execute(test));
        for (auto s : sm.output) std::cout << s;
        std::cout << std::endl;
    }
}

bool matchDay(const std::string_view& input, size_t& read)
{
    if (input.length() < 2)
        return false;
    int day = (input.at(0) - '0') * 10 + (input.at(1) - '0');
    read = 2;
    return (day >= 1 && day <= 31);
}

bool matchMonth(const std::string_view& input, size_t& read)
{
    if (input.length() < 2)
        return false;
    int month = (input.at(0) - '0') * 10 + (input.at(1) - '0');
    read = 2;
    return (month >= 1 && month <= 12);
}

bool matchYear(const std::string_view& input, size_t& read)
{
    if (input.length() < 4)
        return false;
    read = 4;
    return true;
}

void specialPasswordCheck()
{
    State q0("0");
    State q1("1");
    State q2("2");
    State q3("3");
    State q4("45");
    State q5("67");
    State q6("89AB");
    State F("fin", true);
    State D("D");

    q0.insertNewEntry(&q1, {matchFor_az, matchFor_AZ}, produceOutput);
    q0.insertNewEntry(&D, {matchAny}, produceOutput);

    q1.insertNewEntry(&q2, {matchFor_az, matchFor_AZ}, produceOutput);
    q1.insertNewEntry(&D, {matchAny}, produceOutput);

    q2.insertNewEntry(&q3, {matchFor_az, matchFor_AZ}, produceOutput);
    q2.insertNewEntry(&D, {matchAny}, produceOutput);

    q3.insertNewEntry(&q4, {matchFor_az, matchFor_AZ}, produceOutput);
    q3.insertNewEntry(&D, {matchAny}, produceOutput);

    q4.insertNewEntry(&q5, {matchDay}, produceOutput);
    q4.insertNewEntry(&D, {matchAny}, produceOutput);

    q5.insertNewEntry(&q6, {matchMonth}, produceOutput);
    q5.insertNewEntry(&D, {matchAny}, produceOutput);

    q6.insertNewEntry(&q6, {matchYear}, produceOutput);
    q5.insertNewEntry(&D, {matchAny}, produceOutput);

    F.insertNewEntry(&D, {matchAny}, produceOutput);
    D.insertNewEntry(&D, {matchAny}, produceOutput);

    FSM sm("6digits", &q0);

    {
        std::string test("adbd01011991");
        printf("Check string %s: ", test.c_str());
        CHECK_FSM_RESULT(sm.execute(test));
        for (auto s : sm.output) std::cout << s;
        std::cout << std::endl;
    }
}



int main()
{
    bitFlipperExample();
    emailValidatorExample();
    converter();
    sixDigitsPasswordCheck();
    return 0;
}