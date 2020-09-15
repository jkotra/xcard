

class xcard
{

protected:
    std::string prefix;
    int debug;

    bool LuhnCheck(const std::string &cardNo);

public:
    float VERSION = 0.02;
    
    xcard(std::string prefix = "-1", int debug = false);
    bool validateCard(const std::string &cardNo);
    std::string generateCard();
};

class xcardMT : xcard
{

protected:
    int max_threads = 1;

    void runnerLS(long long int start, long long int end, std::vector<std::string>  *result);
    void runner(std::vector<std::string> cards, std::vector<bool>  *result);

public:
    xcardMT(std::string prefix, int threads, int debug);
    std::vector<bool> validateFromFile(std::ifstream &fp);
    std::vector<std::string> LinearSearch();
};
