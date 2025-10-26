#include <iostream>
#include <regex>
using namespace std;

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
    const string transitions[3] = {"ε", "a", "c"};

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