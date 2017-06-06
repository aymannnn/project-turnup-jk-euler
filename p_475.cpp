#include <iostream>
#include <array>
#include <vector>
#include <ctime>
#include <algorithm>

// f(a) = f(12n), looking for f(600) => n = 50
constexpr bool PRINT_STATES = true;
constexpr int n = 2;
constexpr int problem_mod = 1000000007;
constexpr int num_quartets = 3 * n;
constexpr int num_triplets = 4 * n;
constexpr int num_states = 5;       // [0 people, 1 person, ... 4 people]
                                    // Note: State of quartets.

                                    // Some typedefs to make life easier...
using uint = long long;
using state = std::array<uint, num_states>;
static uint final_answer = 1;

void mult_the_final(uint v1)
{
    if (v1 > problem_mod) v1 %= problem_mod;
    final_answer *= v1;
    if (final_answer > problem_mod) final_answer %= problem_mod;
}

void print_state(state const &s)
{
    char *ofs = ", ";
    std::cout <<
        "[" << s[0] << ofs << s[1] << ofs << s[2] <<
        ofs << s[3] << ofs << s[4] <<
        "]\n";
}

bool is_state_already_used(state const &st, std::vector<state> const &holder)
{
    return (std::find(holder.begin(), holder.end(), st) != holder.end());
}

void adjust(state &s, uint const &index, uint const &amt_adjust)
{
    s[index] -= amt_adjust; s[index - 1] += amt_adjust;
}

uint choose_3(uint const &n)
{
    return (n*(n - 1)*(n - 2)) / 6;
}

uint choose_2(uint const &n)
{
    return (n*(n - 1)) / 2;
}

// As usual, stolen from stack over. Fast integer powers by squaring.
uint ipow(uint base, uint exp)
{
    uint result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }
    return result;
}

void add_possible_next_states(std::vector<state> &next_state_container,
                              state const &starting_config)
{
    // Method 1.
    // 1) remove 3 from quartets with 1, 2, 3, or 4 people

    for (uint i = 1; i < num_states; i++)
    {
        if (starting_config[i] < 3) continue;
        state possible_state = starting_config;
        adjust(possible_state, i, 3);
        mult_the_final(choose_3(starting_config[i]) * ipow(i, 3));
        if (!is_state_already_used(possible_state, next_state_container))
        {
            next_state_container.push_back(possible_state);
        }
    }

    // Method 2.
    // 2) remove 2 from quartets with 1, 2, 3, or 4 people and 1 from
    // any of the remaining

    for (uint i = 1; i < num_states; i++)
    {
        // Must remove 2 people from the ith index.
        if (starting_config[i] < 2) continue;
        for (uint j = 1; j < num_states; j++)
        {
            // Must remove 1 element from the jth index.
            if (starting_config[j] < 1 || j == i) continue;
            state possible_state = starting_config;
            adjust(possible_state, i, 2);
            adjust(possible_state, j, 1);
            mult_the_final(choose_2(starting_config[i]) * ipow(i, 2));
            mult_the_final(starting_config[j] * j);
            if (!is_state_already_used(possible_state, next_state_container))
            {
                next_state_container.push_back(possible_state);
            }
        }
    }

    // Method 3.
    // 3) remove 1 from three groups of quartets of different sizes

    for (uint i = 1; i < 3; i++) {
        if (starting_config[i] < 1) continue;
        for (uint j = i + 1; j < 4; j++) {
            if (starting_config[j] < 1) continue;
            for (uint k = j + 1; k < 5; k++) {
                if (starting_config[k] < 1) continue;
                state possible_state = starting_config;
                adjust(possible_state, i, 1);
                adjust(possible_state, j, 1);
                adjust(possible_state, k, 1);
                mult_the_final(starting_config[i] * i);
                mult_the_final(starting_config[j] * j);
                mult_the_final(starting_config[k] * k);
                if (!is_state_already_used(possible_state,
                                           next_state_container))
                {
                    next_state_container.push_back(possible_state);
                }
            }
        }
    }


}

// Stole this function from likely stackoverflow :x
uint mul_inv(uint a, uint b)
{
    uint b0 = b, t, q;
    uint x0 = 0, x1 = 1;
    if (b == 1) return 1;
    while (a > 1)
    {
        q = a / b;
        t = b, b = a % b, a = t;
        t = x0, x0 = x1 - q * x0, x1 = t;
    }
    if (x1 < 0) x1 += b0;
    return x1;
}

uint find_inverse()
{
    // Calculate 4n! mod the problem mod.
    uint four_n_fact_mod_mod = 1;
    for (uint i = 2; i <= 4 * n; i++)
    {
        four_n_fact_mod_mod *= i;
        four_n_fact_mod_mod %= problem_mod;
    }

    // Now we have to find the inverse
    uint inverse = mul_inv(four_n_fact_mod_mod, problem_mod);
    return inverse;
}

void solve(state &initial, state &last_state)
{
    std::vector<state> states_current;
    std::vector<state> next_states;
    states_current.push_back(initial);
    while (true)
    {
        std::cout << "States to iterate : " << states_current.size();
        if (PRINT_STATES)
        {
            std::cout << ".\nPrinting individual states run:\n";
        }
        else
        {
            std::cout << ". Status of trivual state : ";
            print_state(states_current[0]);
        }
        next_states.clear();
        for (auto &possible : states_current)
        {
            if (PRINT_STATES) print_state(possible);
            add_possible_next_states(next_states, possible);
        }

        states_current = next_states;

        if (next_states.empty()) break;
    }
    // Have to adjust for the number of ways 4n trios could have been
    // formed to begin with, which is : (4n)!.
    // Use division i.e. multiplication by modular inverse.
    uint inv_4nfact_mod = find_inverse();
    final_answer = (inv_4nfact_mod*final_answer) %
        problem_mod;

}

int main()
{
    clock_t starting_time, ending_time;
    starting_time = clock();

    state initial_state, last_state; // [0 people, 1 person, 2 ... ]
    initial_state.fill(0);
    last_state.fill(0);
    initial_state[4] = num_quartets;
    last_state[0] = num_quartets;

    solve(initial_state, last_state);

    ending_time = clock();

    std::cout <<
        "Solution mod " << problem_mod << " is " <<
        final_answer << ".\n";
    std::cout <<
        "Time to run solution for n = " << n << " : " <<
        double(ending_time - starting_time) / CLOCKS_PER_SEC << " seconds.\n";

    return 0;
}