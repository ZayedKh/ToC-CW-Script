#include <iostream>
#include <regex>
using namespace std;

const string EPSILON = "ε";
const string PHI = "ɸ";

// Edge cases for epsilon and PHI are largely irrelevant considering Q1 contains no epsilon or PHI transitions - JIC tho
class RegexHelper
{
public:
    static string concat(string &R1, string &R2)
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

    static string star(const string &R)
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

    static string unionOp(const string &R1, const string &R2)
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
    const string transitions[3] = {EPSILON, "a", "c"};

    for (vector<float>::size_type i = 0; i < 3; ++i)
    {
        cout << "Add transition " << states[i] << " ---" << transitions[i] << "---> " << static_cast<int>(vec[i]) << endl;
    }
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

    generateTransitions(loginNumbers);

    return 0;
}