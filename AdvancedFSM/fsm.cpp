#include <fsm.h>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <cassert>

#ifdef DEBUG
#define PRINT_DEBUG 1
#else
#define PRINT_DEBUG 0
#endif

#define debug_printf(fmt, ...)                  \
    do {                                        \
        if (PRINT_DEBUG)                        \
            fprintf(stderr, fmt, ##__VA_ARGS__);  \
    } while (0)


namespace SML
{
    State::State(std::string name, bool isFinal) 
        : name(name), 
          isFinal(isFinal),
          table()
    {}

    void State::insertNewEntry(State* nextState, std::vector<inputMatchFn> inputFunctions, outputFn outputFunction)
    {
        debug_printf("state: %s, insert transition  to %s\n", this->name.c_str(), nextState->name.c_str());
        this->table.push_back({inputFunctions, outputFunction, nextState});
    }

    FSM::FSM(std::string FSMName, State* initState)
    {
        this->name = FSMName;
        this->initialState = initState;
        this->currentState = initialState;
        this->result = false;
    }

    const transitionTableEntry* FSM::findTransition(const std::string& inputString)
    {
        for (const transitionTableEntry& row : this->currentState->table)
        {
            const std::vector<inputMatchFn> &inputFunctions = row.inputMatchFunctions;
            size_t read = 0;
            bool isMatched = false;
            //debug_printf("Current cursor: %zu\n", this->currentCursor);

            const std::string_view substring = std::string_view(inputString).substr(this->currentCursor);
            //std::cout << "String now: " << inputString << " Cursor now: " << this->currentCursor << std::endl;
            //std::cout << "Substring now: " << substring << std::endl;
            for (const inputMatchFn& fn : inputFunctions)
            {
                isMatched = fn(std::string_view(substring), read);
                if (isMatched)
                {
                    debug_printf("Matched: read = %zu\n", read);
                    break;
                }
            }
            if (isMatched)
            {
                this->currentCursor += read;
                debug_printf("Found transition with next state: %s\n", row.nextState->name.c_str());
                return &row;
            }
            debug_printf("Not found transition for next state: %s\n", row.nextState->name.c_str());
        }
        debug_printf("No transition for current state: %s\n", this->currentState->name.c_str());
        return nullptr;
    }

    void FSM::applyTransition(const transitionTableEntry* transition)
    {
        if (transition->outputFunction)
            transition->outputFunction(this->currentState, transition->nextState, &this->output);

        debug_printf("Apply transition: current: %s, new: %s\n", 
            this->currentState->name.c_str(), 
            transition->nextState->name.c_str());
        this->currentState = transition->nextState;

    }

    FSMError FSM::execute(const std::string& inputString)
    {
        assert(this->initialState);
        this->currentState = this->initialState;
        this->output.clear();
        this->currentCursor = 0;
        
        debug_printf("Process string: %s\n", inputString.c_str());

        while(this->currentCursor < inputString.length())
        {
            debug_printf("Current state: %s Cursor: %zu\n", this->currentState->name.c_str(), this->currentCursor);
            const transitionTableEntry* tr = this->findTransition(inputString);
            if (!tr)
                return FSMError::FSM_NO_TRANSITION;
            this->applyTransition(tr);
        }

        this->result = currentState->isFinal;

        return FSMError::FSM_NO_ERROR;

    }


};