#include "../include/tokens.hpp"

int main(int argc, char** argv){
    std::vector<Token> tokens = tokenize();
    std::string source, args, libs, output, compiler;
    for (int i=0; i < tokens.size(); i++){
        switch(tokens.at(i).t){
            case src:
                source += tokens.at(i).data + " ";
                std::cout << "source" << " : " << tokens.at(i).data << std::endl; break;
            case arg:
                args += tokens.at(i).data + " ";
                std::cout << "arg" << " : " << tokens.at(i).data << std::endl; break;
            case lib:
                libs += tokens.at(i).data + " ";
                std::cout << "lib" << " : " << tokens.at(i).data << std::endl; break;
            case out:
                output += tokens.at(i).data + " ";
                std::cout << "out" << " : " << tokens.at(i).data << std::endl; break;
            case gxx:
                compiler += tokens.at(i).data + " ";
                std::cout << "gxx" << " : " << tokens.at(i).data << std::endl; break;
        }
    }
    std::string cmd = compiler + source + args + libs + "-o " + output;
    system(cmd.data());
}