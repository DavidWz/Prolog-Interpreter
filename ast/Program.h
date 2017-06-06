#pragma once

#include "../Common.h"
#include <iostream>
#include "Rule.h"

/**
 * A program is a collection of rules.
 */
class Program {
private:
    std::vector<std::shared_ptr<Rule>> mRules;
public:
    Program(const std::vector<std::shared_ptr<Rule>>& rules) :
        mRules(rules)
    {
    }

    Program() :
        mRules()
    {
    }

    std::vector<std::shared_ptr<Rule>> getRules() const {
        return mRules;
    }

    friend inline std::ostream& operator <<(std::ostream& os, const Program& program) {
        if (program.mRules.empty()) {
            os << "{Empty Program}";
        }
        else {
            for (int i = 0; i < (int) program.mRules.size(); i++) {
                os << *(program.mRules[i]);
                if (i < (int)program.mRules.size()-1) {
                    os << "\n";
                }
            }
        }
        return os;
    }
};