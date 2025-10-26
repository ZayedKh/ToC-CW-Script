#include <iostream>
#include <regex>
#include <map>
#include <set>

using namespace std;
using stateId = uint32_t;
using regEx = string;

const regEx EPSILON = "ε";
const regEx PHI = "ɸ";
map<pair<stateId, stateId>, regEx> transitions;

/*
    min(x) = 2, max(x) = 3
    min(y) = 2, max(y) = 5
    min(z) = 2, max(z) = 4

    ∴ we know states will always be in range 1 - 5
*/

set<stateId> allStates = {1, 2, 3, 4, 5};

// Edge cases for epsilon and PHI are largely irrelevant considering Q1 contains no epsilon or PHI transitions - JIC tho
class RegexHelper
{
public:
    static regEx concat(regEx &R1, regEx &R2)
    {
        // ε º ε = ε
        if (R1 == EPSILON && R2 == EPSILON)
        {
            return EPSILON;
        }

        // R1 º ɸ = ɸ º R1 = ɸ
        if (R1 == PHI || R2 == PHI)
        {
            return PHI;
        }

        // ε º R2 = R2 º ε = R2
        if (R1 == EPSILON)
        {
            return R2;
        }
        else if (R2 == EPSILON)
        {
            return R1;
        }

        // Preserve union precedence
        if (R1.find("∪") != string::npos)
        {
            R1 = "(" + R1 + ")";
        }
        if (R2.find("∪") != string::npos)
        {
            R2 = "(" + R2 + ")";
        }

        return R1 + R2;
    }

    static regEx star(const regEx &R)
    {
        if (R == EPSILON || R == PHI)
        {
            return EPSILON;
        }

        // (R*)* = R*
        if (R.length() > 2 && R.back() == '*' && R.front() == '(')
        {
            return R;
        }

        return "(" + R + ")*";
    }

    static regEx unionOp(const regEx &R1, const regEx &R2)
    {
        if (R1 == PHI)
        {
            return R2;
        }
        if (R2 == PHI)
        {
            return R1;
        }

        if (R1 == R2)
        {
            return R1;
        }

        return "(" + R1 + "∪" + R2 + ")";
    }
};

struct State
{
    uint32_t id;
    bool is_intermediate;
};

bool isValidLoginString(const string &login)
{
    const regex loginPattern("^[a-z]{4}\\d{3}$");
    return regex_match(login, loginPattern);
}

vector<float> getLoginNumbers(const string &login)
{
    vector<float> nums;
    for (vector<float>::size_type i = login.length() - 1; i > login.length() - 4; --i)
    {
        nums.push_back(static_cast<float>(login[i] - '0'));
    }
    reverse(nums.begin(), nums.end());
    return nums;
}

void generateInts(vector<float> &vec)
{
    vec[0] = ceil((vec[0] + 1) / 5) + 1;
    vec[1] = ceil((vec[1] + 1) / 3) + 1;
    vec[2] = ceil((vec[2] + 1) / 4) + 1;
}

void generateTransitions(const vector<float> &vec)
{
    const int states[3] = {1, 4, static_cast<int>(vec[vec.size() - 1])};
    const string regTransitions[3] = {EPSILON, "a", "c"};

    for (vector<float>::size_type i = 0; i < 3; ++i)
    {
        stateId from = static_cast<stateId>(states[i]);
        stateId to = static_cast<stateId>(vec[i]);
        if (transitions.count({from, to}))
        {
            cout << "Pair found " << from << " to " << to << "regex: " << transitions[{from, to}] << endl;
            regEx updatedRegex = RegexHelper::unionOp(transitions[{from, to}], regTransitions[i]);
            cout << "New: " << updatedRegex << endl;
            transitions[{from, to}] = updatedRegex;
            continue;
        }
        transitions[{from, to}] = regTransitions[i];
    }
}

void generateDefaultStates()
{
    State q2 = {2, true};
    State q3 = {3, true};
    State q4 = {4, true};
    State q5 = {5};

    transitions.insert({{q2.id, q4.id}, "b"});
    transitions.insert({{q4.id, q5.id}, "a"});
    transitions.insert({{q4.id, q3.id}, "b"});
    transitions.insert({{q3.id, q4.id}, EPSILON});
}

regEx getRegEx(stateId fromState, stateId toState)
{
    if (transitions.count({fromState, toState}))
    {
        return transitions[{fromState, toState}];
    }

    return PHI;
}

regEx generateFinalRegEx()
{
    vector<int> eliminationList = {
        2,
        3,
        4,
    };

    for (stateId removeState : eliminationList)
    {
        for (stateId inState : allStates)
        {
            if (inState == removeState)
            {
                continue;
            }

            for (stateId outState : allStates)
            {
                if (outState == removeState)
                {
                    continue;
                }

                regEx rDirect = getRegEx(inState, outState);
                regEx rIn = getRegEx(inState, removeState);
                regEx rLoop = getRegEx(removeState, removeState);
                regEx rOut = getRegEx(removeState, outState);

                regEx starLoop = RegexHelper::star(rLoop);

                regEx rBypassPath = RegexHelper::concat(rIn, starLoop);

                rBypassPath = RegexHelper::concat(rBypassPath, rOut);
                regEx rFinal = RegexHelper::unionOp(rDirect, rBypassPath);

                transitions[{inState, outState}] = rFinal;
            }
        }
    }

    return getRegEx(1, 5);
}

int main()
{
    using namespace std;
    string itLogin;

    cout << "Enter your IT login: (e.g. abcd036):" << endl;
    cin >> itLogin;

    const bool validLogin = isValidLoginString(itLogin);

    if (!validLogin)
    {
        cout << "Invalid login, double check your input";
        return 0;
    }

    vector<float> loginNumbers = getLoginNumbers(itLogin);

    generateInts(loginNumbers);

    cout << "Generated numbers: ";
    for (const auto &num : loginNumbers)
    {
        cout << num << " ";
    }
    cout << endl;

    // populate map with default transitions
    generateDefaultStates();
    generateTransitions(loginNumbers);

    for (const auto &transition : transitions)
    {
        cout << "Transition from state " << transition.first.first << " to state " << transition.first.second << " with regex: " << transition.second << endl;
    }

    regEx finalRegex = generateFinalRegEx();
    cout << "Final Regex: " << finalRegex << endl;

    return 0;
}