#include "../include/tokens.hpp"

int main(int argc, char** argv){
    std::vector<std::string> calls;
    for (int i=0; i < argc; i++) calls.insert(calls.end(),std::string(argv[i]));
    std::vector<Token> tokens = tokenize(calls);
    bool comp=true;
    std::string source, args, libs, output, compiler;
    for (int i=0; i < tokens.size(); i++){
        switch(tokens.at(i).t){
            case src: source += tokens.at(i).data + " "; break;
            case arg: args += tokens.at(i).data + " ";  break;
            case lib: libs += tokens.at(i).data + " "; break;
            case out: output += tokens.at(i).data + " "; break;
            case gxx: compiler += tokens.at(i).data + " "; break;
            case nocomp: comp=false; break;
        }
    }
    if (comp){
        std::string cmd = compiler + source + args + libs + "-o " + output;
        system(cmd.data());
    }
}