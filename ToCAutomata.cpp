#include <iostream>
using namespace std;

vector<int> getLoginNumbers(const string &login)
{
    vector<int> nums;
    for (vector<int>::size_type i = login.length() - 1; i > login.length() - 4; --i)
    {
        nums.push_back(static_cast<int>(login[i] - '0'));
    }
    reverse(nums.begin(), nums.end());
    return nums;
}

void generateInts(vector<int> &vec)
{
    if (vec.size() < 3)
    {
        cout << "Vector must have at least 3 elements." << endl;
        return;
    }

    vec[0] = ceil(vec[0] / 5) + 1;
    vec[1] = ceil((vec[1] + 1) / 3) + 1;
    vec[2] = ceil((vec[2] + 1) / 4) + 1;
}

int main()
{
    using namespace std;
    string itLogin;

    cout << "Enter your IT login: (e.g. abcd036):" << endl;
    cin >> itLogin;

    vector<int> loginNumber = getLoginNumbers(itLogin);

    for (const auto &num : loginNumber)
    {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}