class Queens{

private:
    int n;
    int population;
    int retain;
    int result;
    std::vector<std::vector<int> > board;
    float mutation_rate;
    int generation;

    int fitness();
    float utility(int);
    void initialize_board();
    void mutate(std::vector<int>&);
    void retain_fn();
    void crossover();

public:
    Queens(int);
    int run();
    void print_solution();

};
