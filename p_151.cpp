#include <vector>
#include <iostream>
#include <unordered_map>
#include <array>
#include <string>
#include <iomanip>
#include <ctime>

enum sheets {
    A1, A2, A3, A4, A5, NUM_SHEETS
};

using state = std::array<int, NUM_SHEETS>;

std::string state_to_string(state const &s) {
    std::string hash;
    for (auto &x : s) {
        hash.append(std::to_string(x));
    }
    return hash;
}

state get_next_state(state s, const int &index) {
    s[index] --;
    for (int i = index + 1; i < NUM_SHEETS; i++) {
        s[i]++;
    }
    return s;
}

void add_state(std::unordered_map<std::string, double> &states_and_count,
               std::vector<state> &states_at_end, state &s,
               double const &num_times) {
    using std::string;
    string key = state_to_string(s);
    if (states_and_count.count(key)) {
        states_and_count[key] += num_times;
    }
    else {
        states_at_end.push_back(s);
        states_and_count.emplace(key, num_times);
    }
}

int get_num_sheets(state const &s) {
    int num_ = 0;
    for (auto const &x : s) {
        num_ += x;
    }
    return num_;
}

int main() {

    using std::unordered_map;
    using std::string;
    using std::vector;
    using std::cout;

    auto start_time = clock();

    // The count is not an integer because sometimes it will account
    // for the probability of each sheet (i.e. if your state is
    // {0, 2, 2, 2, 1} and you pull sheet a5 then the next state
    // that results from that is 1 / 7 of the total states generated.
    // from that particular starting state.

    unordered_map<string, double> check_state_and_count_start;
    unordered_map<string, double> check_state_and_count_end;

    vector<state> states_start;
    vector<state> states_end;

    state start =   { 1, 0, 0, 0, 0 };
    state end   =   { 0, 0, 0, 0, 1 }; // not actually used...

    // batch 1
    state iteration_1 = get_next_state(start, 0);
    
    // note that we don't care about batch 1 and batch 16 don't count for
    // calculating the expected value
    add_state(check_state_and_count_start, states_start, iteration_1, 1);
    int batch = 2;
    char nl = '\n'; // I'm lazy

    double expected_one_sheet = 0;

    while (batch < 16) {

        cout << "Running batch -> " << batch << nl ;
        batch++;

        // Get the total number of starting states at each iteration
        double starting_num_states = 0;
        for (auto const &pair : check_state_and_count_start) {
            starting_num_states += pair.second;
        }

        // Our total number of starting states is the denominator for
        // this iteration. (i.e. how many states out of your stating states
        // have only one sheet)
        // Now count how many times that actually happens by iterating
        // through the states and checking the count of the number of
        // sheets. While you're at it, generate next states too

        for (auto &state_: states_start) {
                       
            int num_sheets = get_num_sheets(state_);
            double num_times_state = // Note, starting num states is a double
                check_state_and_count_start[state_to_string(state_)];
            if (num_sheets == 1) {
                expected_one_sheet += num_times_state / starting_num_states;
            }
            
            // Generate new states
            for (int j = 0; j < NUM_SHEETS; j++) {
                if (state_[j] == 0) continue;
                // Need to account for the probability of pulling each
                // specific sheet of paper
                double p_state = state_[j] / double(num_sheets);
                state next_ = get_next_state(state_, j);
                add_state(check_state_and_count_end, states_end, next_,
                          p_state*num_times_state);
            }
        }

        // And reset starting states for next iteration
        states_start = states_end;
        check_state_and_count_start = check_state_and_count_end;
        states_end.clear();
        check_state_and_count_end.clear();

    }
    
    auto end_time = clock();

    cout << "Expected number of times only one sheet of paper -> " <<
        std::setprecision(10) << expected_one_sheet << nl;
    
    cout << "Time to run simulation -> " <<
        double(end_time - start_time) / CLOCKS_PER_SEC << " seconds." <<
        nl << nl;

    return 0;
}