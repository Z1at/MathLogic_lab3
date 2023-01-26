#include <algorithm>
#include <iostream>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <sstream>

using namespace std;

string first_axiom(string &alpha, string &beta) {
    string axiom = alpha;
    axiom += "->";
    axiom += beta;
    axiom += "->";
    axiom += alpha;
    return axiom;
}

string second_axiom(string &alpha, string &beta, string &gamma) {
    string axiom = "(";
    axiom += alpha;
    axiom += "->";
    axiom += beta;
    axiom += ")->(";
    axiom += alpha;
    axiom += "->";
    axiom += beta;
    axiom += "->";
    axiom += gamma;
    axiom += ")->(";
    axiom += alpha;
    axiom += "->";
    axiom += gamma;
    axiom += ")";
    return axiom;
}

string merge_two_formulas(string &left, string &right) {
    string left_and_right = left;
    left_and_right += "->";
    left_and_right += right;
    return left_and_right;
}

void remove_whitespaces(string &s) {
    s.erase(remove(s.begin(), s.end(), ' '), s.end());
}

void check_if_var_exists(string &var, string &output) {
    if (var.size()) {
        output.append(var);
    }
    var = "";
}

string polish_notation(string &s) {
    string output;
    vector<string> stack;
    string var;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '(') {
            check_if_var_exists(var, output);
            stack.push_back("(");
        }
        else if (s[i] == '!') {
            check_if_var_exists(var, output);
            output.append("!");
        }
        else if (s[i] == '&') {
            check_if_var_exists(var, output);
            while (stack.size() and stack.back() == "&") {
                output.append("&");
                stack.pop_back();
            }
            stack.push_back("&");
        }
        else if (s[i] == '|') {
            check_if_var_exists(var, output);
            while (stack.size() and (stack.back() == "&" or stack.back() == "|")) {
                output.append(stack.back());
                stack.pop_back();
            }
            stack.push_back("|");
        }
        else if (s[i] == '-') {
            check_if_var_exists(var, output);
            while (stack.size() and stack.back() != "->" and stack.back() != "(") {
                output.append(stack.back());
                stack.pop_back();
            }
            stack.push_back("->");
            i++;
        }
        else if (s[i] == ')') {
            check_if_var_exists(var, output);
            while (stack.back() != "(") {
                output.append(stack.back());
                stack.pop_back();
            }
            stack.pop_back();
        }
        else {
            var.push_back(s[i]);
        }
    }

    check_if_var_exists(var, output);
    while (stack.size()) {
        output.append(stack.back());
        stack.pop_back();
    }

    return output;
}

void solve() {
    string seq;
    getline(cin, seq);

    remove_whitespaces(seq);

    stringstream ss(seq);

    string substr;
    while(true)
    {
        getline( ss, substr, ',' );
        if (ss.good()) {
            cout << substr;
        }
        else {
            break;
        }
        cout << ',';
    }
    size_t ind = substr.find("|-");
    string alpha = "(" + substr.substr(0, ind) + ")";
    string polish_alpha = polish_notation(alpha);
    string statement = substr.substr(ind + 2);

    cout << "|-" << alpha << "->" << statement << '\n';

    unordered_map<string, string> polish_and_original_line;
    string line;

    for (int i = 0; getline(cin, line); i++) {
        remove_whitespaces(line);

        string polish_notation_line = polish_notation(line);
        if (polish_notation_line == polish_alpha) {
            cout << first_axiom(alpha, alpha) << '\n';

            string beta = "(" + merge_two_formulas(alpha, alpha) + ")";
            cout << second_axiom(alpha, beta, alpha) << '\n';

            string first_axiom_alpha_beta = "(" + first_axiom(alpha, beta) + ")";
            cout << merge_two_formulas(first_axiom_alpha_beta, beta) << '\n';

            cout << first_axiom_alpha_beta << '\n';

            cout << beta << '\n';

            polish_and_original_line[polish_notation_line] = line;
            continue;
        }
        bool flag = true;
        for (auto &pair: polish_and_original_line) {
            string delta_j_arrow_delta_i = pair.first;
            delta_j_arrow_delta_i += polish_notation_line;
            delta_j_arrow_delta_i += "->";

            if (polish_and_original_line.find(delta_j_arrow_delta_i) != polish_and_original_line.end()) {
                string beta = "(" + pair.second + ")";

                string escaped_line = "(" + line + ")";

                cout << second_axiom(alpha, beta, escaped_line) << '\n';

                string alpha_arrow_beta_arrow_gamma = "(";
                alpha_arrow_beta_arrow_gamma += alpha;
                alpha_arrow_beta_arrow_gamma += "->";
                alpha_arrow_beta_arrow_gamma += beta;
                alpha_arrow_beta_arrow_gamma += "->";
                alpha_arrow_beta_arrow_gamma += escaped_line;
                alpha_arrow_beta_arrow_gamma += ")";

                string alpha_arrow_gamma = merge_two_formulas(alpha, escaped_line);

                cout << merge_two_formulas(alpha_arrow_beta_arrow_gamma, alpha_arrow_gamma) << '\n';

                cout << alpha_arrow_gamma << '\n';

                flag = false;
                break;
            }
        }

        polish_and_original_line[polish_notation_line] = line;

        if (flag) {
            line = "(" + line + ")";
            cout << first_axiom(line, alpha) << '\n';

            cout << line << '\n';

            cout << merge_two_formulas(alpha, line) << '\n';
        }


    }

}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    solve();

    return 0;
}
