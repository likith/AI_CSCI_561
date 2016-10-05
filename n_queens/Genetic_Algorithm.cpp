#include <iostream>
#include <vector>
#include <random>
#include <unordered_map>
#include <algorithm>
#include <thread>
#include <chrono>

#include "Genetic_Algorithm.h"

void Queens::print_solution(){
    std::cout << "\nSolution\n";
    for(auto it = board[result].begin(); it != board[result].end(); it++){
        std::cout << *it << "\n";
    }
}

void Queens::retain_fn(){
    retain = population/2;
}

void Queens::initialize_board(){

    //std::cout << "\nIn initialize_board() \n";

    std::random_device  rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(1, n);

    for(int i = 0; i < population; i++){
        for(int j = 0; j < n; j++){
            board[i][j] = dis(gen);
        }
    }
}

float Queens::utility(int b){

    //std::cout << "\nIn Utility() \n";

    int clashes = 0;
    std::unordered_map<int, int> column_pos;

    for(int i = 0; i < n; i++){
        column_pos[board[b][i]]++;
    }

    std::unordered_map<int, int>::iterator it;
    for(it = column_pos.begin(); it != column_pos.end(); it++){
        if(it->second > 0){
            clashes += it->second - 1;
        }
    }

    for(int i = n-1; i >= 0; i--){        
        int col = board[b][i];
        int k = 1;
        while(i+k < n){
            if(col+k <= n){
                if(col == (board[b][i+k]-k)){
                    clashes++;
                }                    
            }
            if(col-k > 0){
                if(col == board[b][i+k]+k){
                    clashes++;
                }
            }
            k++;
        }
    }

    return (1.0/(1.0+clashes));
}

void Queens::mutate(std::vector<int>& child){

    //std::cout << "\nIn Mutate() \n";

    std::random_device  rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis_real(0, 1);
    std::uniform_int_distribution<int> dis(0, n-1);

    if(dis_real(gen) > 0.3){
        return;
    }

    int index = dis(gen);
    int change_to = dis(gen) + 1;
    child[index] = change_to;

    return;
}

void Queens::crossover(){

    //std::cout << "\nIn Crossover() \n";

    std::random_device  rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, retain-1);
    std::uniform_int_distribution<int> dis_crossover(1, n-1);

    for(int i = 0; i < population - retain; i += 2){
        int r_num = dis(gen);
        std::vector<int> parent_1 = board[r_num];
        r_num = dis(gen);
        std::vector<int> parent_2 = board[r_num];

        int crossover_point = dis_crossover(gen);
        std::vector<int> child_1;
        std::vector<int> child_2;
        for(int i_cross = 0; i_cross < crossover_point; i_cross++){
            child_1.push_back(parent_1[i_cross]);
            child_2.push_back(parent_2[i_cross]);
        }
        for(int i_cross = crossover_point; i_cross < n; i_cross++){
            child_1.push_back(parent_2[i_cross]);
            child_2.push_back(parent_1[i_cross]);
        }
        mutate(child_1);
        mutate(child_2);
        board[retain +i] = child_1;
        if((retain +i +1) < population){
            board[retain +i +1] = child_2;
        }
        
    }
    return;

}

int Queens::fitness(){

    //std::cout << "\nIn Fitness() \n";

    std::vector<std::pair<float, std::vector<int> > > fitnesses;

    for(int i = 0; i < population; i++){
        float util = utility(i);
        if(util == 1){
            std::cout << "Found a solution\n";
            return i;
        }
        fitnesses.push_back(std::make_pair(util, board[i]));
    }

    std::sort(fitnesses.begin(), fitnesses.end(), 
        [](const std::pair<float, std::vector<int> >& p1, const std::pair<float, std::vector<int> >& p2){
                return p1.first > p2.first;
    });

    for(int i = 0; i < retain; i++){
        board[i] = fitnesses[i].second;
    }

    std::cout << "\nPrint best 10 fitness of generation: " << generation++ << "\n";
    for(int i = 0; i < retain; i++){
        std::cout << i+1 << "  Fitness:  " << fitnesses[i].first << "\n";
    }
    std::cout << "\n";

    //std::this_thread::sleep_for(std::chrono::seconds(3));

    return -1;
}

int Queens::run(){

    std::cout << "\nInside Run()\n";

    initialize_board();
    retain_fn();

    int f = -1;
    while(f == -1){
        f = fitness();
        if(f != -1){
            break;
        }
        crossover();
    }

    result = f;
    return result;

}

Queens::Queens(int _n): n(_n), population(6*_n), mutation_rate(0.4), generation(0){

    std::cout << "\nQueens constructor\n";

    board.resize(population);
    for(int i = 0; i < population; i++){
        board[i].resize(n);
    }
}

int main()
{
    int n;
    std::cout << "Enter the number of Queens to solve\n";
    std::cin >> n;

    Queens q = Queens(n);
    q.run();
    q.print_solution();

    return 0;
}