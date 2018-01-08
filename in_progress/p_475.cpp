#include <iostream>
#include <array>
#include <vector>
#include <ctime>
#include <unordered_map>
#include <string>
#include <cmath>

/*
 * 12n musicians participate at a music festival. On the first day, 
 * they form 3n quartets and practice all day. It is a disaster.
 * At the end of the day, all musicians decide they will never again agree
 * to play with any member of their quartet. On the second day, they form
 * 4n trios, each musician avoiding his previous quartet partners.
 * Let f(12n) be the number of ways to organize the trios amongst the
 * 12n musicians. You are given
 * f(12) = 576 and f(24) mod 1 000 000 007 = 509089824.
 * Find f(600) mod 1 000 000 007.
 */

using huge = long long;
constexpr int NumQuartetStates = 5; // 0, 1, 2, 3, 4 people in quartet
using state = std::array<int, NumQuartetStates>; 
using map_of_states = std::unordered_map<std::string, huge>;
using vec_of_states = std::vector<state>;

constexpr int ProblemN = 1;
constexpr int NumQuartets = 3 * ProblemN;
constexpr int NumTriplets = 4 * ProblemN;
constexpr huge Mod = 1000000007;

huge answer = 1;

void multiply_answer(huge x)
{
    x %= Mod; answer *= x; answer %= Mod;
}

huge multiply_by_mod(huge v1, huge v2)
{
    v1 %= Mod; v2 %= Mod;
    return (v1 * v2) % Mod;
}

huge get_inverse(huge a, huge b)
{
    huge b0 = b, t, q;
    huge x0 = 0, x1 = 1;
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

huge find_inverse()
{
    // Calculate 4n! mod the problem mod.
    huge four_n_fact_mod_mod = 1;
    for (huge i = 2; i <= 4 * n; i++)
    {
        four_n_fact_mod_mod *= i;
        four_n_fact_mod_mod %= MOD;
    }

    // Now we have to find the inverse
    huge inverse = get_inverse(four_n_fact_mod_mod, MOD);
    return inverse;
}

std::string state_to_string(const state &s) {
    // Assume that the number of quartets is <1000.
    // max 3 digits per array entry
    std::string converted;
    for (auto const &v : s)
    {
        if (v < 10) converted.append("00");
        else if (v < 100) converted.append("0");
        converted.append(std::to_string(v));
    }
    return converted;
}

void add_state_to_map(map_of_states &map, const state &st,
                      vec_of_states &unique)
{
    std::string key = state_to_string(st);
    map.count(key) ? map[key] += 1 : map.emplace(key, 1), unique.push_back(st);
}

void adjust(state &s, const int index, const int amt_adjust)
{
    s[index] -= amt_adjust; s[index - 1] += amt_adjust;
}

huge choose_n(const huge x, const int n)
{
    if (n == 1) { return x; }
    else if (n == 2) { return (x * (x - 1)) / 2; }
    else if (n == 3) { return (x * (x - 1) * (x - 2)) / 6; }
    else { std::cout << "fml\n"; return x; }
}

void combo_times_mult(huge &combos, const huge mult)
{
    combos %= Mod; combos *= mult; combos %= Mod;
}

void add_possible_next_states(const state &starting_config,
                              const huge multiplier, map_of_states &map_hold,
                              vec_of_states &vec_hold)
{
    // Method 1.
    // 1) remove 3 from quartets with 1, 2, 3, or 4 people

    for (int i = 1; i < NumQuartetStates; i++)
    {
        x = starting_config[i];
        if (x < 3) continue;
        state possible_state = starting_config;
        adjust(possible_state, i, 3);
        huge combos = choose_n(x, 3)*std::pow(i, 3);
        combo_times_mult(combos, multiplier);
        add_state_to_map(map_hold, possible_state, vec_hold);
    }

    // Method 2.
    // 2) remove 2 from quartets with 1, 2, 3, or 4 people and 1 from
    // any of the remaining

    for (int i = 1; i < NumQuartetStates; i++)
    {
        // Must remove 2 people from the ith index.
        int x = starting_config[i];
        if (x < 2) continue;
        for (int j = 1; j < num_states; j++)
        {
            int y = starting_config[j];
            // Must remove 1 element from the jth index.
            if (y < 1 || j == i) continue;
            state possible_state = starting_config;
            adjust(possible_state, i, 2);
            adjust(possible_state, j, 1);
            multiply_answer(
                ((starting_config[i] * starting_config[i-1]) / 2) 
                * ipow(i, 2));
            multiply_answer(starting_config[j] * j);
            add_state_to_map(map_hold, possible_state, vec_hold);
        }
    }

    // Method 3.
    // 3) remove 1 from three groups of quartets of different sizes

    for (int i = 1; i < 3; i++) {
        if (starting_config[i] < 1) continue;
        for (int j = i + 1; j < 4; j++) {
            if (starting_config[j] < 1) continue;
            for (int k = j + 1; k < 5; k++) {
                if (starting_config[k] < 1) continue;
                state possible_state = starting_config;
                adjust(possible_state, i, 1);
                adjust(possible_state, j, 1);
                adjust(possible_state, k, 1);
                multiply_answer(starting_config[i] * i);
                multiply_answer(starting_config[j] * j);
                multiply_answer(starting_config[k] * k);
                add_state_to_map(map_hold, possible_state, vec_hold);
            }
        }
    }
}


void solve()
{
    map_of_states state_and_count_start;
    map_of_states state_and_count_end;
    vec_of_states unique_at_start;
    vec_of_states unique_at_end;
    state initial = { 0, 0, 0, 0, NumQuartets };
    add_state_to_map(state_and_count_start, initial, unique_states_start)
    while (true)
    {
        std::cout << "Unique states to test : " <<
            state_and_count_start.size() << " .\n";

        for (auto &possible : unique_at_start)
        {
            state st = possible;
            huge num_combos = state_and_count_start[state_to_string(st)];
            add_possible_next_states(st, possible.second, state_and_count_end,
                                     unique_at_end);
        }

        state_and_count_start = state_and_count_end;
        unique_at_start = unique_at_end;
        if (unique_at_start.empty()) break;
    }

    // Have to adjust for the number of ways 4n trios could have been
    // formed to begin with, which is : (4n)!.
    // Use division i.e. multiplication by modular inverse.
    huge inv_4nfact_mod = find_inverse();
    answer = (inv_4nfact_mod*answer) % Mod;
}

int main()
{
    clock_t starting_time, ending_time;
    starting_time = clock();
    solve();
    ending_time = clock();
    std::cout << "Answer -> " << answer << " .\n";
    std::cout << "Time to run simulation -> " <<
        static_cast<double>(ending_time - starting_time) /
        CLOCKS_PER_TICK << " .\n";
    return 0;
}